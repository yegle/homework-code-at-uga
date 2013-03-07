#include "hw.h"
#include "pci/pci.h"
#include "net/net.h"
#include "net/checksum.h"
#include "loader.h"
#include "sysemu/sysemu.h"
#include "sysemu/dma.h"

#include "trace.h"

// from http://www.pcidatabase.com/vendor_details.php?id=767
#define PCI_VENDOR_ID_BROADCOM 0x14E4
#define BCM5752_DEVICE_ID 0x1600

#define PNPMMIO_SIZE 0x20000
#define IOPORT_SIZE       0x40

typedef struct BCM5752State_st {
    PCIDevice dev;
    NICState *nic;
    NICConf conf;
    MemoryRegion mmio;
    MemoryRegion io;

    uint32_t mac_reg[0x8000];
    uint16_t phy_reg[0x20];
    uint16_t eeprom_data[64];

    uint32_t rxbuf_size;
    uint32_t rxbuf_min_shift;
    struct e1000_tx {
        unsigned char header[256];
        unsigned char vlan_header[4];
        /* Fields vlan and data must not be reordered or separated. */
        unsigned char vlan[4];
        unsigned char data[0x10000];
        uint16_t size;
        unsigned char sum_needed;
        unsigned char vlan_needed;
        uint8_t ipcss;
        uint8_t ipcso;
        uint16_t ipcse;
        uint8_t tucss;
        uint8_t tucso;
        uint16_t tucse;
        uint8_t hdr_len;
        uint16_t mss;
        uint32_t paylen;
        uint16_t tso_frames;
        char tse;
        int8_t ip;
        int8_t tcp;
        char cptse;     // current packet tse bit
    } tx;

    struct {
        uint32_t val_in;	// shifted in from guest driver
        uint16_t bitnum_in;
        uint16_t bitnum_out;
        uint16_t reading;
        uint32_t old_eecd;
    } eecd_state;

    QEMUTimer *autoneg_timer;
} BCM5752State;

static uint64_t bcm5752_io_read(void *opaque, hwaddr addr,
                              unsigned size)
{
    return 0;
}

static void bcm5752_io_write(void *opaque, hwaddr addr,
                           uint64_t val, unsigned size)
{
    return;
}

static const MemoryRegionOps bcm5752_io_ops = {
    .read = bcm5752_io_read,
    .write = bcm5752_io_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void
bcm5752_mmio_write(void *opaque, hwaddr addr, uint64_t val,
                 unsigned size)
{
    trace_bcm5752_mmio_write(addr, val, size);
    BCM5752State *s = opaque;
    unsigned int index = (addr & 0x1ffff) >> 2;

    s->mac_reg[index] = val;
}

static uint64_t
bcm5752_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    trace_bcm5752_mmio_read(addr, size);
    BCM5752State *s = opaque;
    unsigned int index = (addr & 0x1ffff) >> 2;

    return s->mac_reg[index];
}

static const MemoryRegionOps bcm5752_mmio_ops = {
    .read = bcm5752_mmio_read,
    .write = bcm5752_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .impl = {
        .min_access_size = 4,
        .max_access_size = 4,
    },
};

static void bcm5752_mmio_setup(BCM5752State *s){
    trace_bcm5752_mmio_setup(0);
    //int i;
    //const uint32_t excluded_regs[] = {
    //    E1000_MDIC, E1000_ICR, E1000_ICS, E1000_IMS,
    //    E1000_IMC, E1000_TCTL, E1000_TDT, PNPMMIO_SIZE
    //};

    memory_region_init_io(&s->mmio, &bcm5752_mmio_ops, s, "bcm5752-mmio",
                          PNPMMIO_SIZE);
    //memory_region_add_coalescing(&d->mmio, 0, excluded_regs[0]);
    //for (i = 0; excluded_regs[i] != PNPMMIO_SIZE; i++)
    //    memory_region_add_coalescing(&d->mmio, excluded_regs[i] + 4,
    //                                 excluded_regs[i+1] - excluded_regs[i] - 4);
    memory_region_init_io(&s->io, &bcm5752_io_ops, s, "bcm5752-io", IOPORT_SIZE);
}


static int pci_bcm5752_init(PCIDevice *pci_dev) {
    trace_pci_bcm5752_init(0);
    /* Power Management Capabilities */
    int cfg_offset = 0x48;
    pci_add_capability(pci_dev, PCI_CAP_ID_PM,
            cfg_offset, PCI_PM_SIZEOF);

    BCM5752State *s = DO_UPCAST(BCM5752State, dev, pci_dev);
    bcm5752_mmio_setup(s);
    pci_register_bar(&s->dev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
    pci_register_bar(&s->dev, 1, PCI_BASE_ADDRESS_SPACE_IO, &s->io);
    /* PCI-X capabilities */
    cfg_offset = 0x40;
    pci_add_capability(pci_dev, PCI_CAP_ID_PCIX,
            cfg_offset, 8);
    return 0;
}

static void pci_bcm5752_uninit(PCIDevice *dev) {
    trace_pci_bcm5752_uninit(0);
    return;
}

static void qdev_bcm5752_reset(DeviceState *dev) {
    trace_qdev_bcm5752_reset(0);
    return;
}

static void bcm5752_pre_save(void *opaque) {
    trace_bcm5752_pre_save(0);
    return;
}

static int bcm5752_post_load(void *opaque, int version_id) {
    trace_bcm5752_post_load(0);
    return 0;
}

static const VMStateDescription vmstate_bcm5752 = {
    .name = "bcm5752",
    .version_id = 1,
    .minimum_version_id = 0,
    .minimum_version_id_old = 0,
    .pre_save = bcm5752_pre_save,
    .post_load = bcm5752_post_load,
    .fields      = (VMStateField []) {
    }
};

static Property bcm5752_properties[] = {
    DEFINE_NIC_PROPERTIES(BCM5752State, conf),
    DEFINE_PROP_END_OF_LIST(),
};


static void bcm5752_class_init(ObjectClass *klass, void *data)
{
    trace_bcm5752_class_init(0);
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->init = pci_bcm5752_init;
    k->exit = pci_bcm5752_uninit;
    k->romfile = "BCM5752.eeprom";
    k->vendor_id = PCI_VENDOR_ID_BROADCOM;
    k->device_id = BCM5752_DEVICE_ID;
    k->revision = 0x01;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Broadcom BCM series";
    dc->reset = qdev_bcm5752_reset;
    dc->vmsd = &vmstate_bcm5752;
    dc->props = bcm5752_properties;
}

static const TypeInfo bcm5752_info = {
    .name          = "bcm5752",
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(BCM5752State),
    .class_init    = bcm5752_class_init,
};

static void bcm5752_register_types(void)
{
    type_register_static(&bcm5752_info);
}

type_init(bcm5752_register_types)

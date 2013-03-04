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

static int pci_bcm5752_init(PCIDevice *dev) {
    trace_pci_bcm5752_init(0);
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

static Property e1000_properties[] = {
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
    dc->props = e1000_properties;
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

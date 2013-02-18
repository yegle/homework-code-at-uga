/*
 * This is the buggy program that will give you some trouble.
 * 
 * Two bugs are there in this code. 
 * 
 * Please bring the fixes to class on Tuesday (Jan 22), and 
 * please exlain how you debugged.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/pci.h>

#include "e1000.h"
#include "e1000_hw.h"

#define BAR_0       0
#define BAR_1       1
#define BAR_5       5

MODULE_LICENSE("GPL");

char modname[] = "netdev";

struct net_device *my_netdev;

/* private field */
struct ethx_priv
{
    struct pci_dev *pdev;
    void *mmio_addr;
    unsigned long reg_len;
    struct e1000_tx_ring* tx_ring;
    struct e1000_rx_ring* rx_ring;
    void *bar;
};

static int ethx_init(struct net_device *net_dev);
static int ethx_open(struct net_device *net_dev);
static int ethx_stop(struct net_device *net_dev);
static int ethx_xmit_frame(struct sk_buff *skb, struct net_device *net_dev);
static int ethx_change_mtu(struct net_device *net_dev, int new_mtu);
static int ethx_ioctl(struct net_device *net_dev, struct ifreq *ifr, int cmd);
static void ethx_uninit(struct net_device *dev);
static struct net_device_stats* ethx_get_stats(struct net_device *dev);
static struct pci_dev* ethx_probe(struct pci_dev *pdev);
static void ethx_set_rx_mode(struct net_device *netdev);
static int __devinit ethx_alloc_queues(struct ethx_priv *pp);
static void ethx_configure_tx(struct ethx_priv *pp);

static const struct net_device_ops my_dev_ops = {
    .ndo_init = ethx_init,
    .ndo_open = ethx_open,
    .ndo_stop = ethx_stop,
    .ndo_do_ioctl = ethx_ioctl,
    .ndo_get_stats = ethx_get_stats,
    .ndo_start_xmit = ethx_xmit_frame,
    .ndo_uninit = ethx_uninit,
    .ndo_change_mtu = ethx_change_mtu,
    .ndo_set_rx_mode = ethx_set_rx_mode,
};

/*******************************************************
 * added 0206, check the hardware and register diver   *
 *******************************************************/
static struct pci_dev* ethx_probe(struct pci_dev *pdev)
{
    //unsigned char pci_rev;
    printk(KERN_INFO "-->INSIDE ethx_probe");
    printk(KERN_INFO "---->pci_get_device");
    pdev = pci_get_device (0x8086, 0x100E, pdev);
    if (!pdev)
    {
        printk (KERN_INFO "---->e1000 card not present");
        return pdev;
    }

    //pci_read_config_byte (pdev, PCI_REVISION_ID, &pci_rev);
    printk(KERN_INFO "---->pci_enable_device");
    if (pci_enable_device (pdev)) {
        printk (KERN_INFO "---->Error enabling the device");
        return pdev;
    }

    return pdev;
}

static int ethx_init(struct net_device *dev)
{
    struct ethx_priv *pp;
    printk(KERN_INFO "-->INSIDE ethx_init");
    pp = netdev_priv(dev);
    ethx_alloc_queues(pp);
    ethx_configure_tx(pp);
    return 0;
}

static void ethx_uninit(struct net_device *dev)
{
    printk(KERN_INFO "-->INSIDE ethx_uninit");
    return;
}

static struct net_device_stats *ethx_get_stats(struct net_device *dev)
{
    /* only return the current stats */
    //printk(KERN_INFO "-->INSIDE ethx_get_stats");
    return &dev->stats;
}


static int ethx_open(struct net_device * netdev)
{
    printk(KERN_INFO "-->INSIDE netdev open");
    return 0;
}

static int ethx_stop(struct net_device * netdev)
{
    printk(KERN_INFO "-->INSIDE netdev close");
    return 0;
}

static int ethx_xmit_frame(struct sk_buff *skb, struct net_device *net_dev)
{
	struct e1000_context_desc *context_desc;
	struct e1000_buffer *buffer_info;
	unsigned int i;
	u32 cmd_length = 0;
	u16 ipcse = 0, tucse, mss;
	u8 ipcss, ipcso, tucss, tucso, hdr_len;
	//int err;
    struct ethx_priv *pp;
    struct e1000_tx_ring *tx_ring;

    printk(KERN_INFO "-->INSIDE ethx_xmit_frame");

    pp = netdev_priv(net_dev);

    tx_ring = pp->tx_ring;


    /* update dev states */
    net_dev->stats.tx_packets += 1;
    net_dev->stats.tx_bytes += skb->len;


    ///* free skb */
    //if (skb)
    //    dev_kfree_skb(skb);

    //return NETDEV_TX_OK;

    hdr_len = skb_transport_offset(skb) + tcp_hdrlen(skb);
    mss = skb_shinfo(skb)->gso_size;
    if (skb->protocol == htons(ETH_P_IP)) {
        struct iphdr *iph = ip_hdr(skb);
        iph->tot_len = 0;
        iph->check = 0;
        tcp_hdr(skb)->check = ~csum_tcpudp_magic(iph->saddr,
                iph->daddr, 0,
                IPPROTO_TCP,
                0);
        cmd_length = E1000_TXD_CMD_IP;
        ipcse = skb_transport_offset(skb) - 1;
    } else if (skb->protocol == htons(ETH_P_IPV6)) {
        ipv6_hdr(skb)->payload_len = 0;
        tcp_hdr(skb)->check =
            ~csum_ipv6_magic(&ipv6_hdr(skb)->saddr,
                    &ipv6_hdr(skb)->daddr,
                    0, IPPROTO_TCP, 0);
        ipcse = 0;
    }
    ipcss = skb_network_offset(skb);
    ipcso = (void *)&(ip_hdr(skb)->check) - (void *)skb->data;
    tucss = skb_transport_offset(skb);
    tucso = (void *)&(tcp_hdr(skb)->check) - (void *)skb->data;
    tucse = 0;

    cmd_length |= (E1000_TXD_CMD_DEXT | E1000_TXD_CMD_TSE |
            E1000_TXD_CMD_TCP | (skb->len - (hdr_len)));

    i = tx_ring->next_to_use;
    context_desc = E1000_CONTEXT_DESC(*tx_ring, i);
    buffer_info = &tx_ring->buffer_info[i];

    context_desc->lower_setup.ip_fields.ipcss  = ipcss;
    context_desc->lower_setup.ip_fields.ipcso  = ipcso;
    context_desc->lower_setup.ip_fields.ipcse  = cpu_to_le16(ipcse);
    context_desc->upper_setup.tcp_fields.tucss = tucss;
    context_desc->upper_setup.tcp_fields.tucso = tucso;
    context_desc->upper_setup.tcp_fields.tucse = cpu_to_le16(tucse);
    context_desc->tcp_seg_setup.fields.mss     = cpu_to_le16(mss);
    context_desc->tcp_seg_setup.fields.hdr_len = hdr_len;
    context_desc->cmd_and_length = cpu_to_le32(cmd_length);

    buffer_info->time_stamp = jiffies;
    buffer_info->next_to_watch = i;

    if (++i == tx_ring->count) i = 0;
    tx_ring->next_to_use = i;

    return true;
}

static int ethx_change_mtu(struct net_device *net_dev, int new_mtu)
{
    printk(KERN_INFO "-->INSIDE mtu change not implemented!");
    return 0;
}

static int ethx_ioctl(struct net_device *net_dev, struct ifreq *ifr, int cmd)
{
    printk(KERN_INFO "-->INSIDE ethx_ioctl");
    return 0;
}

int ethx_get_info(char *buf, char **start, off_t off, int buf_len)
{
    int len =0;
    printk(KERN_INFO "-->INSIDE ethx_get_info");
    len += sprintf(buf+len, "Get state information");
    return len;
}

int get_mod_info(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
    printk(KERN_INFO "-->INSIDE get_mod_info");
    return 0;
}

struct proc_dir_entry *proc_file;

static void netdev_setup(struct net_device *dev)
{
    printk(KERN_INFO "-->INSIDE netdev_setup");
    ether_setup(dev);
    dev->netdev_ops = &my_dev_ops;
    //dev->destructor = free_netdev;

    dev->tx_queue_len = 100;
    dev->flags |= IFF_NOARP;
    dev->flags &= ~IFF_MULTICAST;
    //random_ether_addr(dev->dev_addr);
}

static int __init my_netdev_init(void)
{
    int ret = 0;
    unsigned long mmio_start, mmio_len, mmio_flags;
    struct pci_dev *pdev = NULL;
    struct ethx_priv *pp = NULL;
    void *ioaddr;
    int i = 0;

    printk(KERN_INFO "********************netdev start********************");
    /* detection & locate the device */
    pdev = ethx_probe(pdev);
    if(ret)
        return -1;
    /* locate the device */
    printk(KERN_INFO "alloc_netdev");
    my_netdev = alloc_netdev(sizeof(pp), "mynetdev%d", netdev_setup);
    /* what's the difference between alloc_netdev & alloc_etherdev */
    //my_netdev = alloc_etherdev(sizeof(pp));
    //printk(KERN_INFO "netdev_setup()");
    //netdev_setup(my_netdev);

    if (my_netdev) {
        printk(KERN_INFO "netdev_priv()");
        pp =  netdev_priv(my_netdev);
        printk(KERN_INFO "netdev_priv() finished");
        pp->pdev = pdev;
    } else {
        printk(KERN_INFO "alloc etherdev failed");
    }

    /* get PCI memory mapped I/O space base address from BAR1(should be 2 according to the manual ) */
    mmio_start = 0;
    for (i=BAR_1; i <= BAR_5; i++){
        mmio_len = pci_resource_len(pdev, i);
        mmio_flags = pci_resource_flags(pdev, i);
        /* make sure above region is MMI/O */
        if(mmio_len != 0 && (mmio_flags & IORESOURCE_IO)) {
            mmio_start = pci_resource_start(pdev, i);
            printk(KERN_INFO "i=%d", i);
            break;
        }
    }

    if (mmio_start == 0){
        printk(KERN_INFO "get mmio_start failed!");
        return -2;
    }

    printk(KERN_INFO "[mmio_start:%ld\t]",mmio_start);
    printk(KERN_INFO "[mmio_len:%ld\t\t]",mmio_len);
    printk(KERN_INFO "[mmio_flags:%ld\t]",mmio_flags);


    /* get PCI memory space */
    //printk(KERN_INFO "pci_request_regions()");
    //if(pci_request_regions(pdev, "E1000")) {
    //    printk(KERN_INFO "Could not get PCI region");
    //    return -2;
    //}
    printk(KERN_INFO "pci_set_master()");
    pci_set_master(pdev);
    printk(KERN_INFO "pci_save_state()");
    ret = pci_save_state(pdev);
    if (ret){
        printk(KERN_INFO "pci_save_state ERROR");
    }
    /* ioremap MMI/O region */
    printk(KERN_INFO "ioremap()");
    ioaddr = ioremap(mmio_start, mmio_len);
    if(!ioaddr) {
        printk(KERN_INFO "Could not ioremap");
        pci_release_regions(pp->pdev);
        pci_disable_device(pp->pdev);
        return -1;
    }

    my_netdev->base_addr = (long)ioaddr;
    printk(KERN_INFO "my_netdev->base_addr %lx",(long)ioaddr);
    pp->mmio_addr = ioaddr;
    pp->bar = pci_ioremap_bar(pdev, BAR_0);
    printk(KERN_INFO "pp->mmio_addr %p",ioaddr);
    pp->reg_len = mmio_len;

    /* UPDATE NET_DEVICE */
    for(i = 0; i < 6; i++) {  /* Hardware Address */
        my_netdev->dev_addr[i] = readb((void *)(my_netdev->base_addr+i));
        my_netdev->broadcast[i] = 0xff;
    }
    printk(KERN_INFO "dev->dev_addr is: [%02x:%02x:%02x:%02x:%02x:%02x]",
            my_netdev->dev_addr[0],my_netdev->dev_addr[1],my_netdev->dev_addr[2],
            my_netdev->dev_addr[3],my_netdev->dev_addr[4],my_netdev->dev_addr[5]);

    my_netdev->hard_header_len = 14;
    memcpy(my_netdev->name, "E1000", 5); /* Device Name */
    my_netdev->irq = pdev->irq;  /* Interrupt Number */

    /* register the device */
    printk(KERN_INFO "set my_netdev->netdev_ops");
    my_netdev->netdev_ops = &my_dev_ops;
    printk(KERN_INFO "register_netdev()");
    ret=register_netdev(my_netdev);
    if (ret) {
        pr_err("register_netdev %s failed %d\n", my_netdev->name, ret);
        /* some cleaning work if regist fails */
        iounmap(pp->mmio_addr);
        pci_release_regions(pp->pdev);
        pci_disable_device(pp->pdev);
        free_netdev(my_netdev);
        return -1;
    }


    proc_file = create_proc_entry(modname, 0644, NULL);
    proc_file->read_proc = get_mod_info;
    return ret;
}

static void __exit my_netdev_exit(void)
{
    struct ethx_priv *pp;
    pp =  netdev_priv(my_netdev);

    iounmap(pp->mmio_addr);
    pci_release_regions(pp->pdev);
    pci_disable_device(pp->pdev);

    if (my_netdev) {
        unregister_netdev(my_netdev);
        kfree(my_netdev);
    }
    if (proc_file)
        remove_proc_entry(modname, NULL);
    printk(KERN_INFO "-->INSIDE netdev exit");
    return;
}

static void ethx_set_rx_mode(struct net_device *netdev){
    struct ethx_priv *pp;
    long* ioaddr;
    u32 rctl;
    pp = netdev_priv(netdev);
    ioaddr = pp->mmio_addr;
    rctl = readl(pp->bar + E1000_RCTL);
    // XXX: some modification to rctl here
    writel(rctl, pp->bar + E1000_RCTL);
}

static int __devinit ethx_alloc_queues(struct ethx_priv *pp)
{
    struct pci_dev *pdev = pp->pdev;
    int size;
    printk(KERN_INFO "-->ALLOCATING ring");
	pp->tx_ring = kcalloc(1, sizeof(struct e1000_tx_ring), GFP_KERNEL);
	if (!pp->tx_ring){
        printk(KERN_INFO "tx_ring allocation failed");
		return -ENOMEM;
    }

	pp->rx_ring = kcalloc(1, sizeof(struct e1000_rx_ring), GFP_KERNEL);
	if (!pp->rx_ring) {
        printk(KERN_INFO "rx_ring allocation failed");
		return -ENOMEM;
	}

    pp->tx_ring->count = 256;
    pp->rx_ring->count = 256;

    printk(KERN_INFO "-->ring allcation succeed");

    printk(KERN_INFO "%d", pp->tx_ring->count);

    size = sizeof(struct e1000_buffer) * pp->tx_ring->count;
    printk(KERN_INFO "NEEDS %d memory", size);

    pp->tx_ring->buffer_info = vzalloc(size);
    if (!pp->tx_ring->buffer_info){
        printk(KERN_INFO "cannot allocate tx_ring buffer");
        return -ENOMEM;
    }

	pp->tx_ring->size = pp->tx_ring->count * sizeof(struct e1000_tx_desc);
	pp->tx_ring->size = ALIGN(pp->tx_ring->size, 4096);

    printk(KERN_INFO "----------before allocating desc");
	pp->tx_ring->desc = dma_alloc_coherent(&pdev->dev,
            pp->tx_ring->size, &pp->tx_ring->dma, GFP_KERNEL);
    printk(KERN_INFO "----------after allocating desc");
    memset(pp->tx_ring->desc, 0, pp->tx_ring->size);
    pp->tx_ring->next_to_use = 0;
    pp->tx_ring->next_to_clean = 0;

    return 0;
}

static void ethx_configure_tx(struct ethx_priv *pp){
    u32 tctl;
    printk(KERN_INFO "-->ethx_configure_tx");

    tctl = readl(pp->bar + E1000_TCTL);

    tctl |= E1000_TCTL_EN;
    tctl |= E1000_TCTL_PSP;
    //tctl &= E1000_TCTL_CT;

    writel(tctl, pp->bar + E1000_TCTL);
}


module_init(my_netdev_init);
module_exit(my_netdev_exit);

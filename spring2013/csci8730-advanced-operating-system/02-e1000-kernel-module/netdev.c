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

#define BAR_0       0
#define BAR_1       1
#define BAR_5       5

MODULE_LICENSE("GPL");

char modname[] = "netdev";

struct net_device *my_netdev;

static int ethx_init(struct net_device *net_dev);
static int ethx_open(struct net_device *net_dev);
static int ethx_stop(struct net_device *net_dev);
static int ethx_xmit_frame(struct sk_buff *skb, struct net_device *net_dev);
static int ethx_change_mtu(struct net_device *net_dev, int new_mtu);
static int ethx_ioctl(struct net_device *net_dev, struct ifreq *ifr, int cmd);
static void ethx_uninit(struct net_device *dev);
static struct net_device_stats* ethx_get_stats(struct net_device *dev);
static struct pci_dev* ethx_probe(struct pci_dev *pdev);

static const struct net_device_ops my_dev_ops = {
    .ndo_init = ethx_init,
    .ndo_open = ethx_open,
    .ndo_stop = ethx_stop,
    .ndo_do_ioctl = ethx_ioctl,
    .ndo_get_stats = ethx_get_stats,
    .ndo_start_xmit = ethx_xmit_frame,
    .ndo_uninit = ethx_uninit,
    .ndo_change_mtu = ethx_change_mtu
};

/* private field */
struct ethx_priv
{
    struct pci_dev *pdev;
    void *mmio_addr;
    unsigned long reg_len;
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
    printk(KERN_INFO "-->INSIDE ethx_init");
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

    printk(KERN_INFO "-->INSIDE ethx_xmit_frame");

    /* update dev states */
    net_dev->stats.tx_packets += 1;
    net_dev->stats.tx_bytes += skb->len;

    /* free skb */
    if (skb)
        dev_kfree_skb(skb);

    return 0;
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

module_init(my_netdev_init);
module_exit(my_netdev_exit);

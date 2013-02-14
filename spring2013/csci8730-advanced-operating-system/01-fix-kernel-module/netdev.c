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

static const struct net_device_ops my_dev_ops = {
	.ndo_init = ethx_init,
	.ndo_open = ethx_open,
	.ndo_stop = ethx_stop,
	.ndo_do_ioctl = ethx_ioctl,
	.ndo_start_xmit = ethx_xmit_frame,
	.ndo_change_mtu = ethx_change_mtu
};

static int ethx_init(struct net_device *dev)
{
	int ret = 0;
	return ret;
}

static void ethx_uninit(struct net_device *dev)
{
	return;
}

static int ethx_open(struct net_device * netdev)
{
	printk(KERN_INFO "netdev open");
	return 0;
}

static int ethx_stop(struct net_device * netdev)
{
	printk(KERN_INFO "netdev close");
	return 0;
}

static int ethx_xmit_frame(struct sk_buff *skb, struct net_device *net_dev)
{

        printk(KERN_INFO "xmit gets called");

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
        printk(KERN_INFO "mtu change not implemented!");
        return 0;
}

static int ethx_ioctl(struct net_device *net_dev, struct ifreq *ifr, int cmd)
{
        return 0;
}

int ethx_get_info(char *buf, char **start, off_t off, int buf_len)
{
        int len =0;
        len += sprintf(buf+len, "Get state information");
        return len;
}

int get_mod_info(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	printk(KERN_INFO "get_mod_info");
	return 0;
}	

struct proc_dir_entry *proc_file;

static void netdev_setup(struct net_device *dev)
{
	ether_setup(dev);
	dev->netdev_ops = &my_dev_ops;
	dev->destructor = free_netdev;

	dev->tx_queue_len = 0;
	dev->flags |= IFF_NOARP;
	dev->flags &= ~IFF_MULTICAST;
	random_ether_addr(dev->dev_addr);
}

static int __init my_netdev_init(void)
{
	int ret = 0;

	printk(KERN_INFO "netdev start");

	my_netdev = alloc_netdev(0, "mynetdev%d", netdev_setup);
	if (my_netdev) {
		my_netdev->netdev_ops = &my_dev_ops;
		ret=register_netdev(my_netdev);
	} else { 
		printk("alloc etherdev failed");
	}

	proc_file = create_proc_entry(modname, 0644, NULL);
	proc_file->read_proc = get_mod_info;

	return ret;

}

static void __exit my_netdev_exit(void)
{
    remove_proc_entry(modname, NULL);
	if (my_netdev) {
		unregister_netdev(my_netdev);
		free_netdev(my_netdev);
	}

	printk(KERN_INFO "netdev exit");
}

module_init(my_netdev_init);
module_exit(my_netdev_exit);

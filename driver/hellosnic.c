#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>

// 字符设备相关
#include <linux/cdev.h>
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/cdev.h>
#include <asm/uaccess.h>	/* copy_*_user */

// 网络接口相关
#include <linux/timer.h>
#include <linux/in.h>
#include <linux/netdevice.h>   /* struct device, and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>          /* struct iphdr */
#include <linux/tcp.h>         /* struct tcphdr */
#include <linux/skbuff.h>


static struct pci_device_id ids[] = {
	{ PCI_DEVICE(0x10ee, 0x7024), },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, ids);

// 在pcie设备上，设定有了128KiB的地址空间
// 其中前64KB用于设备内存的地址，实际上只有8KB的bram，前4K用于发送，后4K用于接收
// 后64K的地址，用于控制DMA（及后续可能的其他），发送控制命令，如设置DMA地址、启动DMA、获取状态等
// 目前实际使用的地址空间：
// 0x000000~0x000fff: 发送报文缓存
// 0x001000~0x001fff: 接收报文缓存
// 0x010000~0x01005b: AXI_DMA的控制器
// 0x011000~0x011003: 数据处理单元控制器
#define PCIE_TX_MEM_ADDR 0x0000
#define PCIE_RX_MEM_ADDR 0x1000
#define PCIE_CTRL_REG_OFFSET 0x010000

void *pci_bar0_addr = NULL;

static void cat_config(struct pci_dev *dev)
{
	u8 data[16];
	int i;
	pr_info("pci config: ");
	for (i=0; i<64; i++)
	{
		pci_read_config_byte(dev, i, &data[i%16]);
		if (i % 16 == 15)
		{
			pr_info("%02x: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", 
					i-15, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], 
					data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
		}
			
	}
	return;
}

static void cat_resources(struct pci_dev *dev)
{
	int start, len, flags;
	int i;
	void* vm_addr;
	unsigned int val;
	for (i=0; i<6; i++)
	{
		start = pci_resource_start(dev, i);
		len = pci_resource_len(dev, i);
		flags = pci_resource_flags(dev, i);
		pr_info("BAR%d: %16x, %10d, %6x\n", i, start, len, flags);
	}

	vm_addr = pci_iomap(dev, 0, 0);
	pr_info("BAR0 map to : %lx\n", (long)vm_addr);
}

static int probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	/* Do probing type stuff here.  
	 * Like calling request_region();
	 */
	pr_info("hello probe\n");
	if(pci_enable_device(dev)) {
		dev_err(&dev->dev, "can't enable PCI device\n");
		return -ENODEV;
	}

	pci_bar0_addr = pci_iomap(dev, 0, 0);
	
	cat_config(dev);
	cat_resources(dev);

	return 0;
}

static void remove(struct pci_dev *dev)
{
	/* clean up any allocated resources and stuff here.
	 * like call release_region();
	 */
}

static struct pci_driver pci_driver = {
	.name = "pci_hellosnic",
	.id_table = ids,
	.probe = probe,
	.remove = remove,
};
// 以上是pcie相关的

// 以下是字符设备相关的
// 这里使用两个字符设备分别对应数据区域和控制区域，实际这两个区域的地址是分开的，也可以只使用一个设备
#define DEV_MEM_SIZE 8192
// 控制寄存器的地址空间会大很多，只有少数地址是有效的
#define DEV_REG_SIZE 131072
struct scull_dev {
	int size;              /* the current quantum size */
	int index;			   /* id for this device*/
	struct mutex mutex;     /* mutual exclusion semaphore     */
	struct cdev cdev;	  /* Char device structure		*/
};

struct scull_dev scull_device_data;
struct scull_dev scull_device_ctrl;
char dev_data_buf[DEV_MEM_SIZE];
int scull_major = 0;
int scull_minor = 0;

loff_t scull_llseek(struct file *filp, loff_t off, int whence)
{
	struct scull_dev *dev = filp->private_data;
	loff_t newpos;

	switch(whence) {
	  case 0: /* SEEK_SET */
		newpos = off;
		break;

	  case 1: /* SEEK_CUR */
		newpos = filp->f_pos + off;
		break;

	  case 2: /* SEEK_END */
		newpos = dev->size + off;
		break;

	  default: /* can't happen */
		return -EINVAL;
	}
	if (newpos < 0) return -EINVAL;
	filp->f_pos = newpos;
	return newpos;
}

void read_from_pcie(char* buf, void *pci_addr, int start, int count)
{
	if (pci_addr == NULL){
		return;
	}
	int start_ali = (start + 3) / 4 * 4;
	int end = start + count;
	int end_ali = end / 4 * 4;
	int i;
	for (i=0; i < start_ali - start; i++){
		buf[i] = (char)ioread8(pci_addr + start + i);
	}
	for (i=0; i < end_ali - start_ali; i += 4){
		*(int*)(buf + i + start_ali - start) = ioread32(pci_addr + start_ali + i);
	}
	for (i=0; i < end - end_ali; i++){
		buf[end_ali - start + i] = (char)ioread8(pci_addr + end_ali + i);
	}
}

ssize_t scull_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
	struct scull_dev *dev = filp->private_data; 
	pr_info("scull%d read, f_pos=%ld, count=%ld\n", dev->index, *f_pos, count);
	ssize_t retval = 0;
	char* dev_buf = dev_data_buf;
	char dev_reg_buf[4];

	if (mutex_lock_interruptible(&dev->mutex))
		return -ERESTARTSYS;

	if (*f_pos >= dev->size)
		goto out;
	
	if (dev->index == 1){  // 1表示是控制设备，只允许读4字节
		dev_buf = dev_reg_buf;
		if (count > 4)
			count = 4;
	}
	if (*f_pos + count > dev->size)
		count = dev->size - *f_pos;
	
	read_from_pcie(dev_buf, pci_bar0_addr, *f_pos, count);

	if (copy_to_user(buf, dev_buf, count)) {
		retval = -EFAULT;
		goto out;
	}

	*f_pos += count;
	retval = count;

  out:
	mutex_unlock(&dev->mutex);
	return retval;
}

void write_to_pcie(char* buf, void *pci_addr, int start, int count)
{
	if (pci_addr == NULL){
		return;
	}
	int start_ali = (start + 3) / 4 * 4;
	int end = start + count;
	int end_ali = end / 4 * 4;
	int i;

	for (i=0; i < start_ali - start; i++){
		iowrite8(buf[i], pci_addr + start + i);
	}
	for (i=0; i < end_ali - start_ali; i += 4){
		iowrite32(*(int*)(buf + i + start_ali - start), pci_addr + start_ali + i);
	}
	for (i=0; i < end - end_ali; i++){
		iowrite8(buf[end_ali - start + i], pci_addr + end_ali + i);
	}
}

ssize_t scull_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
	
	struct scull_dev *dev = filp->private_data;
	pr_info("scull%d write, f_pos=%ld, count=%ld\n", dev->index, *f_pos, count);
	ssize_t retval = 0;
	char* dev_buf = dev_data_buf;
	char dev_reg_buf[4];

	if (mutex_lock_interruptible(&dev->mutex))
		return -ERESTARTSYS;
	
	if (*f_pos >= dev->size){
		retval = -EFAULT;
		goto out;
	}

	if (dev->index == 1){  // 1表示是控制设备，只允许写4字节
		dev_buf = dev_reg_buf;
		if (count > 4)
			count = 4;
	}
	if (count > dev->size - *f_pos)
		count = dev->size - *f_pos;
	
	if (copy_from_user(dev_buf, buf, count)) {
		retval = -EFAULT;
		goto out;
	}

	write_to_pcie(dev_buf, pci_bar0_addr, *f_pos, count);

	*f_pos += count;
	retval = count;

  out:
	mutex_unlock(&dev->mutex);
	return retval;
}

int scull_open(struct inode *inode, struct file *filp)
{
	struct scull_dev *dev; /* device information */

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev; /* for other methods */

	return 0;          /* success */
}

int scull_release(struct inode *inode, struct file *filp)
{
	return 0;
}

struct file_operations scull_fops = {
	.owner =    THIS_MODULE,
	.llseek =   scull_llseek,
	.read =     scull_read,
	.write =    scull_write,
	.open =     scull_open,
	.release =  scull_release,
};

static void scull_setup_cdev(struct scull_dev *dev, int index)
{
	int err, devno = MKDEV(scull_major, scull_minor + index);
    
	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;
	err = cdev_add (&dev->cdev, devno, 1);
	/* Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

void scull_cleanup_module(void)
{
	dev_t devno = MKDEV(scull_major, scull_minor);

	cdev_del(&scull_device_data.cdev);
	cdev_del(&scull_device_ctrl.cdev);

	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, 2);
}

int scull_init_module(void)
{
	int result;
	dev_t dev = 0;

	result = alloc_chrdev_region(&dev, 0, 2, "scull");
	scull_major = MAJOR(dev);
	scull_minor = 0;
	if (result < 0) {
		printk(KERN_WARNING "alloc_chrdev_region error %d\n", result);
		return result;
	}

	memset(&scull_device_data, 0, sizeof(struct scull_dev));
	memset(&scull_device_ctrl, 0, sizeof(struct scull_dev));

    scull_device_data.size = DEV_MEM_SIZE;
	scull_device_data.index = 0;
	scull_device_ctrl.size = DEV_REG_SIZE;
	scull_device_ctrl.index = 1;
	mutex_init(&scull_device_data.mutex);
	mutex_init(&scull_device_ctrl.mutex);
	scull_setup_cdev(&scull_device_data, 0);
	scull_setup_cdev(&scull_device_ctrl, 1);

	return 0; /* succeed */
}
// 以上是字符设备相关的

// 以下是网络接口相关的
struct net_device *snull_dev;
struct snull_priv {
	struct net_device *dev;
	struct napi_struct napi;
	struct net_device_stats stats;
	int status;
	int rx_int_enabled;
	int tx_packetlen;
	u8 *tx_packetdata;
	struct sk_buff *skb;
	spinlock_t lock;
};

#define MAX_PACKET_LEN 1522
char packet_buf[MAX_PACKET_LEN];

// 使用定时器代替网络中断
struct timer_list timer;

void jit_timer_fn(unsigned long arg)
{
	// 查看是否有报文到达
	int S2MM_DMASR = ioread32(pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x34);
	// pr_info("S2MM_DMASR: %x \n", S2MM_DMASR);
	if (S2MM_DMASR & 0x1000){
		// pr_info("receive packet： \n");
		// 取数据并生成skb
		int packet_len = ioread32(pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x58);
		if (packet_len < 60 || packet_len > MAX_PACKET_LEN){
			pr_warn("invalid packet length: %d\n", packet_len);
			goto next;
		}
		read_from_pcie(packet_buf, pci_bar0_addr, PCIE_RX_MEM_ADDR, packet_len);
		struct sk_buff *skb;
		skb = dev_alloc_skb(packet_len + 2);
		if (!skb) {
			pr_info("low on mem, packet dropped\n");
			goto next;
		}
		skb_reserve(skb, 2); /* align IP on 16B boundary */ 
		memcpy(skb_put(skb, packet_len), packet_buf, packet_len);
		/* Write metadata, and then pass to the receive level */
		skb->dev = snull_dev;
		skb->protocol = eth_type_trans(skb, snull_dev);
		skb->ip_summed = CHECKSUM_UNNECESSARY; 
		struct snull_priv *priv = netdev_priv(snull_dev);
		priv->stats.rx_packets++;
		priv->stats.rx_bytes += packet_len;
		netif_rx(skb);
	  next:
		// 清中断
		iowrite32(0x1000, pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x34);
		iowrite32(0x1000, pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x58);			// 接收4K的数据
	}
	
	timer.expires += 2;
	// timer.data = arg;
	add_timer(&timer);
}

int timer_init_module(void)
{
	timer.data = (unsigned long)100;
	timer.function = jit_timer_fn;
	timer.expires = jiffies + 250;
	add_timer(&timer);
	return 0;
}

int snull_open(struct net_device *dev)
{
	//pr_info("here is snull_open");
	/* request_region(), request_irq(), ....  (like fops->open) */

	// assign the mac addr
	memcpy(dev->dev_addr, "\x06\x05\x04\x03\x02\xda", ETH_ALEN);
	
	// 在这里初始化设备上的dma设置
	if (pci_bar0_addr != NULL){
		iowrite32(0x00007001, pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x30); 		// 启动接收dma
		iowrite32(PCIE_RX_MEM_ADDR, pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x48);	// 接收流dma地址
		iowrite32(0x00001000, pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x58);			// 接收4K的数据

		iowrite32(0x00007001, pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x00); 		// 启动发送dma
		iowrite32(PCIE_TX_MEM_ADDR, pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x18);	// 接收流dma地址
		int i;
		for(i=0; i<16; i++){
			iowrite32(0, pci_bar0_addr + PCIE_TX_MEM_ADDR);
		}
		iowrite32(0x00000040, pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x28);			// 启动一次传输的数据，仅控制用，无实质数据
	}

	// 创建定时器
	timer_init_module();

	netif_start_queue(dev);
	return 0;
}

int snull_release(struct net_device *dev)
{
	pr_info("here is snull_release");
    /* release ports, irq and such -- like fops->close */

	// 删除定时器
	del_timer_sync(&timer);

	netif_stop_queue(dev); /* can't transmit any more */
	return 0;
}

/*
 * Configuration changes (passed on by ifconfig)
 */
int snull_config(struct net_device *dev, struct ifmap *map)
{
	pr_info("here is snull_config");
	if (dev->flags & IFF_UP) /* can't act on a running interface */
		return -EBUSY;

	/* Don't allow changing the I/O address */
	if (map->base_addr != dev->base_addr) {
		printk(KERN_WARNING "snull: Can't change I/O address\n");
		return -EOPNOTSUPP;
	}

	/* Allow changing the IRQ */
	if (map->irq != dev->irq) {
		dev->irq = map->irq;
        	/* request_irq() is delayed to open-time */
	}

	/* ignore other fields */
	return 0;
}

/*
 * Transmit a packet (called by the kernel)
 */
int snull_tx(struct sk_buff *skb, struct net_device *dev)
{
	// pr_info("here is snull_tx");

	// 查看上一次发送是否已经完成
	int MM2S_DMASR = ioread32(pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x04);
	if (MM2S_DMASR & 0x1000){
		// 清中断
		iowrite32(0x1000, pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x04);
	}
	else{
		// 没传完则返回错误，等待重传
		pr_info("waiting for tx completion\n");
		return 1;
	}

	if (0) { /* enable this conditional to look at the data */
		int i;
		pr_info("data(%d) to transmit:\n", skb->len);
		for (i=0 ; i<skb->len; i++)
			printk(" %02x", skb->data[i]&0xff);
		printk("\n");
	}

	struct snull_priv *priv = netdev_priv(dev);
	
	dev->trans_start = jiffies; /* save the timestamp */

	// 拷贝到网卡后启动传输
	int len = skb->len > MAX_PACKET_LEN ? MAX_PACKET_LEN : skb->len;
	write_to_pcie(skb->data, pci_bar0_addr, PCIE_TX_MEM_ADDR, len);
	iowrite32(len, pci_bar0_addr + PCIE_CTRL_REG_OFFSET + 0x28);

	priv->stats.tx_packets++;
	priv->stats.tx_bytes += len;
	dev_kfree_skb_any(skb);

	return 0; 
}

/*
 * Ioctl commands 
 */
int snull_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	pr_info("ioctl\n");
	return 0;
}

/*
 * Return statistics to the caller
 */
struct net_device_stats *snull_stats(struct net_device *dev)
{
	pr_info("here is net_device_stats\n");
	struct snull_priv *priv = netdev_priv(dev);
	return &priv->stats;
}

/*
 * The "change_mtu" method is usually not needed.
 * If you need it, it must be like this.
 */
int snull_change_mtu(struct net_device *dev, int new_mtu)
{
	pr_info("here is snull_change_mtu");
	unsigned long flags;
	struct snull_priv *priv = netdev_priv(dev);
	spinlock_t *lock = &priv->lock;
    
	/* check ranges */
	if ((new_mtu < 68) || (new_mtu > 1500))
		return -EINVAL;
	/*
	 * Do anything you need, and the accept the value
	 */
	spin_lock_irqsave(lock, flags);
	dev->mtu = new_mtu;
	spin_unlock_irqrestore(lock, flags);

	return 0; /* success */
}

/*
 * Deal with a transmit timeout.
 */
void snull_tx_timeout (struct net_device *dev)
{
	pr_warn("snull_tx_timeout");
	return;
}


static const struct net_device_ops snull_netdev_ops = {
	.ndo_open           = snull_open,
	.ndo_stop           = snull_release,
	.ndo_set_config	    = snull_config,
	.ndo_start_xmit	    = snull_tx,
	.ndo_do_ioctl	    = snull_ioctl,
	.ndo_get_stats	    = snull_stats,
	.ndo_change_mtu	    = snull_change_mtu,
	.ndo_tx_timeout     = snull_tx_timeout,
};


void snull_init(struct net_device *dev)
{
	pr_info("%s, here is snull_init\n", dev->name);
	struct snull_priv *priv;

	/*
	 * Then, initialize the priv field. This encloses the statistics
	 * and a few private fields.
	 */
	priv = netdev_priv(dev);
	memset(priv, 0, sizeof(struct snull_priv));
	spin_lock_init(&priv->lock);
	priv->dev = dev;

    /* 
	 * Then, assign other fields in dev, using ether_setup() and some
	 * hand assignments
	 */
	ether_setup(dev); /* assign some of the fields */

	dev->watchdog_timeo = 250;
	// dev->features        |= NETIF_F_HW_CSUM;
	dev->netdev_ops = &snull_netdev_ops;
}

void snull_cleanup(void)
{
	pr_info("here is snull_cleanup");
    
	if (snull_dev) {
		unregister_netdev(snull_dev);
		free_netdev(snull_dev);
	}

	return;
}

int snull_init_module(void)
{
	pr_info("here is snull_init_module");
	int result, i, ret = -ENOMEM;

	/* Allocate the devices */
	snull_dev = alloc_netdev(sizeof(struct snull_priv), "sn%d", NET_NAME_UNKNOWN, snull_init);
	if (snull_dev == NULL)
		goto out;

	ret = -ENODEV;
	if ((result = register_netdev(snull_dev)))
		printk("snull: error %i registering device \"%s\"\n",
				result, snull_dev->name);
	else
		ret = 0;
   out:
	if (ret) 
		snull_cleanup();
	
	return ret;
}
// 以上是网络接口相关的


static int __init hellosnic_init(void)
{
	pr_info("hello snic\n");
	// TODO：这里的初始化顺序需要再审视
	scull_init_module();
	snull_init_module();
	return pci_register_driver(&pci_driver);
}

static void __exit hellosnic_exit(void)
{
	scull_cleanup_module();
	pci_unregister_driver(&pci_driver);
	snull_cleanup();
	pr_info("goodbuy snic\n");
}

MODULE_LICENSE("GPL");

module_init(hellosnic_init);
module_exit(hellosnic_exit);

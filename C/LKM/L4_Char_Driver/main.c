#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/irqdesc.h>

#define DEVICE_NAME "mist_irqmap"

static int major;
static struct class *mist_irqmap_class=NULL;
static struct device *mist_irqmap_device=NULL;

static int mist_open(struct inode *inode, struct file *file){
	return 0;
}

static int mist_release(struct inode *inode, struct file *file){
	return 0;
}

#define IOCTL_CMD_GET_VIRQ	1

struct ioctl_arg{
    int hwIRQ;
    int vIRQ;
};

static void get_virq(struct ioctl_arg *data){

	int i;
	struct irq_desc *desc;

	pr_err("%s: nr_irqs=%d\n",DEVICE_NAME,nr_irqs);

	for(i = 0; i < nr_irqs; i++){
	    desc = irq_to_desc(i);
	    if (!desc)
		    continue;

	if (desc->irq_data.domain)
	    pr_err("%s: virq=%d hwirq=%lu req_hwirq=%u\n",DEVICE_NAME,i,desc->irq_data.hwirq,data->hwIRQ);
	    if(desc->irq_data.hwirq==(long unsigned)data->hwIRQ){
		    data->vIRQ=i;
	    }
	}

}

static long mist_ioctl(struct file *file, unsigned int cmd, unsigned long arg){

	struct ioctl_arg data;

        if (copy_from_user(&data, (void __user *)arg, sizeof(struct ioctl_arg)))
                return -EFAULT;

	switch(cmd){
	    case IOCTL_CMD_GET_VIRQ:
		    get_virq(&data);
		    break;
	    default:
		return -EINVAL;
	}

        if (copy_to_user((void __user *)arg, &data, sizeof(struct ioctl_arg)))
                return -EFAULT;

	return 0;
}

static struct file_operations fops = {
    .open   = &mist_open,
    .release   = &mist_release,
    .unlocked_ioctl = &mist_ioctl,
};


static int __init mist_irqmap_init(void)
{

	pr_info("%s: Init\n",DEVICE_NAME);
	
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if(major < 0){
		pr_err("%s: failed to register a major no\n",DEVICE_NAME);
		return major;
	}

	mist_irqmap_class = class_create(THIS_MODULE, "mist_irqmap");
	if(IS_ERR(mist_irqmap_class)){
		unregister_chrdev(major, DEVICE_NAME);
		pr_err("%s: failed to register a device class\n",DEVICE_NAME);
		return PTR_ERR(mist_irqmap_class);
	}

	mist_irqmap_device = device_create(mist_irqmap_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
        if(IS_ERR(mist_irqmap_device)){
		class_destroy(mist_irqmap_class);
                unregister_chrdev(major, DEVICE_NAME);
                pr_err("%s: failed to create a device\n",DEVICE_NAME);
                return PTR_ERR(mist_irqmap_device);
        }

	pr_info("%s: Device initialized\n",DEVICE_NAME);

	return 0;
}


static void __exit mist_irqmap_exit(void)
{
	pr_info("%s: Exit\n",DEVICE_NAME);

	device_destroy(mist_irqmap_class, MKDEV(major, 0));
	class_destroy(mist_irqmap_class);
        unregister_chrdev(major, DEVICE_NAME);
}



module_init(mist_irqmap_init);
module_exit(mist_irqmap_exit);

MODULE_AUTHOR("Nishad");
MODULE_DESCRIPTION("Char Driver for Translation between hwirq and virq");
MODULE_LICENSE("GPL");

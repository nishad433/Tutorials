#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/fs.h>
#include "ncc.h"

static int major;
static struct class *ncc_char_class=NULL;
static struct device *ncc_char_device=NULL;

static int __init ncc_char_init(void)
{

	pr_info("%s: Init\n",DEVICE_NAME);
	
	major = register_chrdev(0, DEVICE_NAME, &ncc_fops);
	if(major < 0){
		pr_err("%s: failed to register a major no\n",DEVICE_NAME);
		return major;
	}

	ncc_char_class = class_create(THIS_MODULE, "ncc_char");
	if(IS_ERR(ncc_char_class)){
		unregister_chrdev(major, DEVICE_NAME);
		pr_err("%s: failed to register a device class\n",DEVICE_NAME);
		return PTR_ERR(ncc_char_class);
	}

	ncc_char_device = device_create(ncc_char_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
        if(IS_ERR(ncc_char_device)){
		class_destroy(ncc_char_class);
                unregister_chrdev(major, DEVICE_NAME);
                pr_err("%s: failed to create a device\n",DEVICE_NAME);
                return PTR_ERR(ncc_char_device);
        }

	pr_info("%s: Device initialized\n",DEVICE_NAME);

	return 0;
}


static void __exit ncc_char_exit(void)
{
	pr_info("%s: Exit\n",DEVICE_NAME);

	device_destroy(ncc_char_class, MKDEV(major, 0));
	class_destroy(ncc_char_class);
        unregister_chrdev(major, DEVICE_NAME);
}



module_init(ncc_char_init);
module_exit(ncc_char_exit);

MODULE_AUTHOR("Nishad");
MODULE_DESCRIPTION("A Sample Char Driver");
MODULE_LICENSE("GPL");

//#include <linux/kernel.h>
#include <linux/module.h>
//#include <linux/init.h>
#include    "ncc.h"

static int __init ncc_init(void)
{
        pr_info("NCC[%s:%d] LKM Module INIT\n",__func__,__LINE__);
        create_sysfs_entry();
        return 0;
}


static void __exit ncc_exit(void)
{
        pr_info("NCC[%s:%d] LKM Module EXIT\n",__func__,__LINE__);
        kobject_put(ncc_kobj);
}



module_init(ncc_init);
module_exit(ncc_exit);

MODULE_AUTHOR("Nishad");
MODULE_DESCRIPTION("Simple LKM");
MODULE_LICENSE("GPL");

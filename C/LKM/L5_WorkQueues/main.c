#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include <linux/workqueue.h>            // Required for workqueues

/* Work structure */
static struct delayed_work wq;
 
void wq_cb(struct work_struct *work); 
 
/*Workqueue Function*/
void wq_cb(struct work_struct *work)
{
        printk(KERN_INFO "Executing Workqueue Function\n");
        schedule_delayed_work(&wq, 2 * HZ);
}


static int __init ncc_init(void)
{
        pr_info("NCC[%s:%d] LKM Module INIT\n",__func__,__LINE__);
        /*Creating work by Dynamic Method */
        INIT_DELAYED_WORK(&wq, wq_cb);
	/*Allocating work to queue*/
        schedule_delayed_work(&wq, 2 * HZ);
        return 0;
}


static void __exit ncc_exit(void)
{
        pr_info("NCC[%s:%d] LKM Module EXIT\n",__func__,__LINE__);
	cancel_delayed_work(&wq);
}



module_init(ncc_init);
module_exit(ncc_exit);

MODULE_AUTHOR("Nishad");
MODULE_DESCRIPTION("Simple LKM");
MODULE_LICENSE("GPL");

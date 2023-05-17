#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include    "ncc.h"

LIST_HEAD(ncc_list_head);

static ncc_t * alloc_node(int val){
	ncc_t *p = kmalloc(sizeof(ncc_t), GFP_KERNEL);
	p->a = val;
	return p;
}

static int __init ncc_init(void)
{
	ncc_t *p1, *p2, *p3, *entry;	
	struct list_head *ptr;
		
        pr_info("NCC[%s:%d] LKM Module INIT\n",__func__,__LINE__);
	p1 = alloc_node(10);
	p2 = alloc_node(20);
	p3 = alloc_node(30);

	list_add(&p1->ncc_list, &ncc_list_head);
	list_add(&p2->ncc_list, &ncc_list_head);
	list_add(&p3->ncc_list, &ncc_list_head);

	list_for_each(ptr, &ncc_list_head){
		entry = list_entry(ptr, ncc_t, ncc_list);
		pr_info("entry=%d\n",entry->a);
	}

	list_del(&p2->ncc_list);

	pr_info("After deleting\n");
	list_for_each(ptr, &ncc_list_head){
		entry = list_entry(ptr, ncc_t, ncc_list);
		pr_info("entry=%d\n",entry->a);
	}


	kfree(p1);
	kfree(p2);
	kfree(p3);
        return 0;
}


static void __exit ncc_exit(void)
{
        pr_info("NCC[%s:%d] LKM Module EXIT\n",__func__,__LINE__);
}



module_init(ncc_init);
module_exit(ncc_exit);

MODULE_AUTHOR("Nishad");
MODULE_DESCRIPTION("Simple LKM");
MODULE_LICENSE("GPL");

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/mm_types.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/gfp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nishad");
MODULE_DESCRIPTION("A module with a single periodic timer.");

#define TIMER_INTERVAL (HZ)  // 1 second

static struct timer_list my_timer;

extern unsigned int ncc_counter;
struct page_frag_cache *getNCCpageFcache(void);

#define NUM_SKB 16
struct sk_buff *skbs[NUM_SKB]={0};
int prev_cnt=0;
int skb_alloced_counter=0;
// Timer callback function
static void timer_callback(struct timer_list *t)
{
	int cpu = smp_processor_id();
	unsigned int ncc = per_cpu(ncc_counter, cpu);	
	struct page_frag_cache *nc = getNCCpageFcache();
	gfp_t flags = GFP_ATOMIC;
	 struct sk_buff *skb=NULL;
	 struct sk_buff *tskb1=NULL;
	 struct sk_buff *tskb2=NULL;
	int i;

	if(prev_cnt != ncc){
		skb = __dev_alloc_skb(2048, flags);
		++skb_alloced_counter;
	}

	for(i=0; i<NUM_SKB; i++){
		tskb1 = __dev_alloc_skb(2048, flags);
		++skb_alloced_counter;
		if(tskb1){
			tskb2=skbs[i];
			if(tskb2){
				kfree_skb(tskb2);
				--skb_alloced_counter;
			}
			skbs[i]=tskb1;
		}
	}
	

	if(nc){
		if(skb){
    			pr_info("NCC1 cpu=%d ncc=%d nc=%px va=%px offset=%d pagecnt_bias=%d pfmemalloc=%d skb=%px skb->head=%px skb_alloced_counter=%d\n",cpu,ncc,nc,nc->va,nc->offset,nc->pagecnt_bias,nc->pfmemalloc,skb,skb->head,skb_alloced_counter);
		}else{
    			pr_info("NCC2 cpu=%d ncc=%d nc=%px va=%px offset=%d pagecnt_bias=%d pfmemalloc=%d skb=%px skb_alloced_counter=%d\n",cpu,ncc,nc,nc->va,nc->offset,nc->pagecnt_bias,nc->pfmemalloc,skb,skb_alloced_counter);
		}
	}else{
		if(skb){
    			pr_info("NCC3 cpu=%d ncc=%d nc=%px skb=%px skb->head=%px skb_alloced_counter=%d\n",cpu,ncc,nc,nc->va,nc->offset,nc->pagecnt_bias,nc->pfmemalloc,skb,skb->head,skb_alloced_counter);
		}else{
    			pr_info("NCC4 cpu=%d ncc=%d nc=%px skb=%px skb_alloced_counter=%d\n",cpu,ncc,nc,nc->va,nc->offset,nc->pagecnt_bias,nc->pfmemalloc,skb,skb_alloced_counter);
		}
	}
	prev_cnt = ncc;
    // Restart the timer
    mod_timer(&my_timer, jiffies + TIMER_INTERVAL);
}

// Module initialization function
static int __init my_module_init(void)
{
    pr_info("Module initialized.\n");

    // Initialize and set up the timer
    timer_setup(&my_timer, timer_callback, 0);
    // Set the timer to expire after TIMER_INTERVAL
    mod_timer(&my_timer, jiffies + TIMER_INTERVAL);

    return 0;
}

// Module cleanup function
static void __exit my_module_exit(void)
{
    pr_info("Module exiting.\n");

    // Remove the timer
    del_timer_sync(&my_timer);
}

module_init(my_module_init);
module_exit(my_module_exit);


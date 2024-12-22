#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/proc_fs.h>
#include <linux/refcount.h>
#include <linux/seq_file.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <net/sock.h>
#include <slab.h>

#define OO_SHIFT 16
#define OO_MASK ((1 << OO_SHIFT) - 1)
static inline int oo_objects(struct kmem_cache_order_objects x) {
  return x.x & OO_MASK;
}

/* Tracking of the number of slabs for debugging purposes */
static inline unsigned long slabs_node(struct kmem_cache *s, int node) {
  struct kmem_cache_node *n = get_node(s, node);

  return atomic_long_read(&n->nr_slabs);
}

int __init mist_slab_init(void) {
  int objectCount = 0;
  if (skbuff_head_cache) {
    printk("[%s] slab=%px %s size=%d object_size=%d flags=%x red_left_pad=%d\n", __func__,
           skbuff_head_cache, skbuff_head_cache->name, skbuff_head_cache->size,
           skbuff_head_cache->object_size, skbuff_head_cache->flags,skbuff_head_cache->red_left_pad);

    struct kmem_cache *s = skbuff_head_cache;
    struct kmem_cache_node *n;
    int node;

    for_each_kmem_cache_node(s, node, n) {
      struct page *page, *h;
      int i = 0;
      unsigned long slabs_total, slabs_partial, node_nr_objs;
      slabs_partial = n->nr_partial;
      slabs_total = slabs_node(s, node);
      node_nr_objs = atomic_long_read(&n->total_objects);
      pr_info("node=%d n=%px slabs_total=%lu slabs_partial=%lu slabs_full=%lu "
              "tot_objs=%lu\n",
              node, n, slabs_total, slabs_partial,
              (slabs_total - slabs_partial), node_nr_objs);

      // Iterate over the partial list
      list_for_each_entry_safe(page, h, &n->partial, lru) {
        // Calculate the number of objects per slab
        unsigned long num_objects = oo_objects(s->oo);

        // If all objects in this page are in use, it's fully allocated
        objectCount += page->inuse;
        pr_info(
            "%d] Fully allocated slab page: %px  inuse=%d   num_objects=%d  "
            "objectCount=%d\n",
            ++i, page, page->inuse, num_objects, objectCount);
      }

      // Check the full list as well, although it's not commonly used in SLUB
      i = 0;
      list_for_each_entry_safe(page, h, &n->full, lru) {
        objectCount += page->inuse;
        pr_info("%d]Page from slab full list: %p inuse=%d\n", ++i, page,
                page->inuse);
      }
    }
    struct kmem_cache_cpu *c;
    int cpu;

    for_each_online_cpu(cpu) {
      c = per_cpu_ptr(s->cpu_slab, cpu);
      if (c->page) {
        objectCount += c->page->inuse;
        pr_info("CPU %d has slab page: %p, inuse: %u\n", cpu, c->page,
                c->page->inuse);
      }
    }
  }
  pr_info("objectCount=%d\n", objectCount);
  return 0;
}

void __exit mist_slab_exit(void) { pr_info("MIST SKB Debug Module EXIT\n"); }

module_init(mist_slab_init);
module_exit(mist_slab_exit);

MODULE_AUTHOR("Nishad");
MODULE_DESCRIPTION("Simple LKM");
MODULE_LICENSE("GPL");

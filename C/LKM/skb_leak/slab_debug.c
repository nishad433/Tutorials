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
#define CREATE_TRACE_POINTS
#include <trace/events/ncc.h>

int s_obj_pad_offset = 0;

#if 0
#define dprintf(x, ...)                                                        \
  do {                                                                         \
    if (mist_skb_dbg_log_level >= 1)                                           \
      pr_info(x, ##__VA_ARGS__);                                               \
  } while (0)
#define ddprintf(x, ...)                                                       \
  do {                                                                         \
    if (mist_skb_dbg_log_level >= 2)                                           \
      pr_info(x, ##__VA_ARGS__);                                               \
  } while (0)
#define dddprintf(x, ...)                                                      \
  do {                                                                         \
    if (mist_skb_dbg_log_level >= 3)                                           \
      pr_info(x, ##__VA_ARGS__);                                               \
  } while (0)
#else
#define dprintf(x, ...)                                                        \
  do {                                                                         \
  } while (0)
#define ddprintf(x, ...)                                                       \
  do {                                                                         \
  } while (0)
#define dddprintf(x, ...)                                                      \
  do {                                                                         \
  } while (0)
#endif

#define mist_proc_create_single(name, dir, write_func)                         \
  {                                                                            \
    auto int name##_open(struct inode *in, struct file *file) {                \
      return single_open(file, name##_show, NULL);                             \
    }                                                                          \
    static struct file_operations name##_proc_ops = {.owner = THIS_MODULE,     \
                                                     .read = seq_read,         \
                                                     .write = (write_func),    \
                                                     .llseek = seq_lseek,      \
                                                     .release =                \
                                                         single_release};      \
    name##_proc_ops.open = name##_open;                                        \
    if (!proc_create(#name, 0644, (dir), &name##_proc_ops)) {                  \
      pr_err("proc_create(\"#name\") failed\n");                               \
    }                                                                          \
  }

struct proc_dir_entry *mist_skb_dbg_proc_dir = NULL;
extern struct kmem_cache *skbuff_head_cache;

/* Tracking of the number of slabs for debugging purposes */
static inline unsigned long slabs_node(struct kmem_cache *s, int node) {
  struct kmem_cache_node *n = get_node(s, node);

  return atomic_long_read(&n->nr_slabs);
}

#define OO_SHIFT 16
#define OO_MASK ((1 << OO_SHIFT) - 1)
static inline int oo_objects(struct kmem_cache_order_objects x) {
  return x.x & OO_MASK;
}

// taken from arch/arm64/mm/mmu.c
int k_addr_valid(unsigned long addr) {
  pgd_t *pgdp;
  pud_t *pudp, pud;
  pmd_t *pmdp, pmd;
  pte_t *ptep, pte;

  if ((((long)addr) >> VA_BITS) != -1UL)
    return 0;

  pgdp = pgd_offset_k(addr);
  if (pgd_none(READ_ONCE(*pgdp)))
    return 0;

  pudp = pud_offset(pgdp, addr);
  pud = READ_ONCE(*pudp);
  if (pud_none(pud))
    return 0;

  if (pud_sect(pud))
    return pfn_valid(pud_pfn(pud));

  pmdp = pmd_offset(pudp, addr);
  pmd = READ_ONCE(*pmdp);
  if (pmd_none(pmd))
    return 0;

  if (pmd_sect(pmd))
    return pfn_valid(pmd_pfn(pmd));

  ptep = pte_offset_kernel(pmdp, addr);
  pte = READ_ONCE(*ptep);
  if (pte_none(pte))
    return 0;

  return pfn_valid(pte_pfn(pte));
}

int dump_slab_page(struct seq_file *m, struct list_head *head, int obj_size,
                   int objsPerSlab, int *skbCount) {
  struct page *page, *h;
  int i = 0;
  list_for_each_entry_safe(page, h, head, lru) {
    int j;
    uint8_t *slab_obj = NULL;
    struct sk_buff *skb = NULL;
    int valid;
    trace_ncc_print(page, h, head, 33);
    valid = k_addr_valid(page_address(page));
    trace_ncc_print(page, h, head, valid);
    if (!valid) {
      continue;
    }
    slab_obj = ((uint8_t *)page_address(page));
    for (j = 0; j < objsPerSlab; j++) {
      skb = (struct sk_buff *)(slab_obj + s_obj_pad_offset);
      if (skb->mist_skb_dbg_magic == MIST_SKB_DBG_MAGIC &&
          refcount_inc_not_zero_checked(&skb->users)) {
        seq_printf(m, "\n%d] skb=%px pfn=%ld\n", *skbCount, skb,
                   virt_to_pfn(skb));

	++(*skbCount);
        // decrement reference count and returns true if the count is 0
        if (skb_unref(skb)) {
          kfree_skb(skb);
        }
      }
      slab_obj += obj_size;
    }
  }

  return 0;
}

void build_stats(struct seq_file *m) {
  struct kmem_cache *s = skbuff_head_cache;
  int node;
  int skbCount = 0;
  struct kmem_cache_node *n;
  int objsperslab;
  objsperslab = oo_objects(s->oo);
  for_each_kmem_cache_node(s, node, n) {
    dump_slab_page(m, &n->partial, s->size, objsperslab, &skbCount);
    dump_slab_page(m, &n->full, s->size, objsperslab, &skbCount);
  }
}

int mist_skb_debug_dump_show(struct seq_file *m, void *arg) {
  build_stats(m);
  seq_printf(m, "\n");
  return 0;
}

int __init mist_skb_dbg_init(void) {
  pr_info("MIST SKB Debug Module INIT, skb->mist_skb_dbg_magic=0x%x\n",
          offsetof(struct sk_buff, mist_skb_dbg_magic));
  if (skbuff_head_cache) {
    mist_skb_dbg_proc_dir = proc_mkdir("mist_skb_debug", NULL);
    if (mist_skb_dbg_proc_dir) {
      mist_proc_create_single(mist_skb_debug_dump, mist_skb_dbg_proc_dir, NULL);
    }

    s_obj_pad_offset = skbuff_head_cache->red_left_pad;
    printk("[%s] slab=%px %s size=%d object_size=%d flags=%x "
           "pad_offset=0x%x\n",
           __func__, skbuff_head_cache, skbuff_head_cache->name,
           skbuff_head_cache->size, skbuff_head_cache->object_size,
           skbuff_head_cache->flags, s_obj_pad_offset);
  }
  return 0;
}

void __exit mist_skb_dbg_exit(void) {
  pr_info("MIST SKB Debug Module EXIT\n");
  proc_remove(mist_skb_dbg_proc_dir);
}

module_init(mist_skb_dbg_init);
module_exit(mist_skb_dbg_exit);

MODULE_AUTHOR("Nishad");
MODULE_DESCRIPTION("Simple LKM");
MODULE_LICENSE("GPL");

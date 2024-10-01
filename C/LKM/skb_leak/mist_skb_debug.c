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

int mist_skb_dbg_log_level = 0;
int mist_skb_mod = 0;
int mist_alloc_cap_point = 0;
int mist_cap_type = 0;
int mist_skb_idx = 0;
int mist_skb_Cntr = 0;
int mist_dump_cmd = 0;
int mist_logged_pkt_cnt = 0;

#if 1
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

/*
#define MIST_MOD_OTHERS         0
#define MIST_MOD_WXLAN          1
#define MIST_MOD_IUDMA          2
#define MIST_MOD_PACE           3
#define MIST_MOD_WL             4
#define MIST_MOD_BLE            5
#define MIST_MOD_SKBUFF         6
#define MIST_MOD_DEVNIT         7
#define MIST_MOD_LAST           8
*/

char *module_itoa[MIST_MOD_LAST] = {
    "Others",  "MIST_WXLAN", "MIST_IUDMA",  "MIST_PACE",
    "MIST_WL", "MIST_BLE",   "MIST_SKBUFF", "MIST_DEVNIT",
};

#define MIST_C_TYPE_NONE 0x0
#define MIST_C_TYPE_SKBIDX 0x1
#define MIST_C_TYPE_MOD_CAP 0x2
#define MIST_C_TYPE_MOD_ONLY 0x3
#define MIST_C_TYPE_ALL 0x4
#define MIST_C_TYPE_LAST 0x5

char *ctype_itoa[MIST_C_TYPE_LAST] = {
    "None",
    "skb matching SKBidx",
    "skbs matching module & cap_point",
    "skbs matching module",
    "all skbs",
};

typedef struct {
  u64 cntr;
  u64 bridged;
  u64 host;
} mist_t;

typedef struct {
  mist_t mist_counters[MIST_MOD_LAST][MIST_CAP_LAST];
  u64 mist_cntr_err, mist_last_seen_err;
  mist_t mist_lastseen_counters[MIST_MOD_LAST];
} mist_stats_t;

mist_stats_t n_stats;

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

int config_show(struct seq_file *m, void *arg) {
  seq_printf(m, "LogLevel(L)=%d\n", mist_skb_dbg_log_level);
  seq_printf(m, "Mod(M)=%d(%s)\n", mist_skb_mod, module_itoa[mist_skb_mod]);
  seq_printf(m, "AllocCapPoint(C)=%d\n", mist_alloc_cap_point);
  seq_printf(m, "CaptureType(T)=%d(%s)\n", mist_cap_type,
             ctype_itoa[mist_cap_type]);
  seq_printf(m, "SKBidx(I)=%d\n", mist_skb_idx);
  seq_printf(m, "SKBdumpCounter(i)=%d\n", mist_skb_Cntr);
  return 0;
}

ssize_t config_wr(struct file *filp, const char __user *buf, size_t count,
                  loff_t *loff) {
  char *msg = kmalloc(1 + (count * sizeof(char)), GFP_KERNEL);
  char cmd;
  if (msg) {
    unsigned long res = 0;
    if (copy_from_user(msg, buf, count)) {
      kfree(msg);
      return -EFAULT;
    }
    msg[count] = '\0';
    cmd = msg[0];
    if (kstrtoul(&msg[1], 10, &res)) {
      kfree(msg);
      goto exit;
    }
    switch (cmd) {
    case 'L':
      mist_skb_dbg_log_level = (int)res;
      break;
    case 'M':
      mist_skb_mod = (int)res;
      break;
    case 'C':
      mist_alloc_cap_point = (int)res;
      break;
    case 'I':
      mist_skb_idx = (int)res;
      break;
    case 'i':
      mist_skb_Cntr = (int)res;
      break;
    case 'T':
      mist_cap_type = (int)res;
      break;
    }
  }
exit:
  return count;
}

void update_counters(struct sk_buff *skb) {
  __u8 module, alloc_cap_point, module_last_seen;
  if (!skb) {
    return;
  }
  module_last_seen = skb->last_seen_module;
  module = skb->alloced_mist_module;
  alloc_cap_point = skb->mist_alloced_cap_point;

  if (module >= 0 && module < MIST_MOD_LAST && alloc_cap_point >= 0 &&
      alloc_cap_point < MIST_CAP_LAST) {
    n_stats.mist_counters[module][alloc_cap_point].cntr += 1;
    n_stats.mist_counters[module][0].cntr += 1;
    if (skb->sk) {
      n_stats.mist_counters[module][alloc_cap_point].host += 1;
      n_stats.mist_counters[module][0].host += 1;
    } else {
      n_stats.mist_counters[module][alloc_cap_point].bridged += 1;
      n_stats.mist_counters[module][0].bridged += 1;
    }
  } else {
    n_stats.mist_cntr_err += 1;
    dprintf("[%s] module=%d alloc_cap_point=%d\n", __func__, module,
            alloc_cap_point);
  }

  if (module_last_seen >= 0 && module_last_seen < MIST_MOD_LAST) {
    n_stats.mist_lastseen_counters[module_last_seen].cntr += 1;
    if (skb->sk) {
      n_stats.mist_lastseen_counters[module_last_seen].host += 1;
    } else {
      n_stats.mist_lastseen_counters[module_last_seen].bridged += 1;
    }
  } else {
    n_stats.mist_last_seen_err += 1;
    dprintf("[%s] module=%d alloc_cap_point=%d\n", __func__, module,
            alloc_cap_point);
  }
}

void decode_hex_pkt(struct seq_file *m, struct sk_buff *skb) {
  int user_count = refcount_read(&skb->users);
  seq_printf(m, "skb_len=%d data=%px dev=%s users=%d sk=%x cloned=%d\n",
             skb->len, skb->data, skb->dev ? skb->dev->name : "NA", user_count,
             (skb->sk != NULL), skb->cloned);
  if (skb->sk && skb->sk->sk_socket && skb->sk->sk_socket->file &&
      skb->sk->sk_socket->file->f_owner.pid) {
    seq_printf(m, "pid =%d\n", pid_nr(skb->sk->sk_socket->file->f_owner.pid));
  } else {
    if (skb->sk) {
      seq_printf(m, "sk=%px ", skb->sk);
      if (skb->sk->sk_socket) {
        seq_printf(m, "socket=%px ", skb->sk->sk_socket);
        if (skb->sk->sk_socket->file) {
          seq_printf(m, "file=%px ", skb->sk->sk_socket->file);
          if (skb->sk->sk_socket->file->f_owner.pid) {
            seq_printf(m, "pid=%px ", skb->sk->sk_socket->file->f_owner.pid);
          }
        }
      }
    }
    seq_printf(m, "\n");
  }
  if (skb->len > 14) {
    int i;
    for (i = 0; i < 100 && i < skb->len; i++) {
      if (i > 0 && i % 16 == 0) {
        seq_printf(m, "\n");
      }
      if (i % 16 == 0) {
        seq_printf(m, "0x%4lx\t\t", (uintptr_t)(skb->data + i) & 0xffff);
      }
      seq_printf(m, "0x%02x ", *((char *)(((char *)skb->data) + i)));
    }
    seq_printf(m, "\n");
  }
}

void mist_skb_dump(struct seq_file *m, struct sk_buff *skb, int *skbCount) {
  __u8 module = 0, alloc_cap_point = 0, module_last_seen = 0, logSKB = 0;
  if (m == NULL || skb == NULL || skbCount == NULL) {
    return;
  }
  module = skb->alloced_mist_module;
  module_last_seen = skb->last_seen_module;
  alloc_cap_point = skb->mist_alloced_cap_point;

  switch (mist_cap_type) {
  case MIST_C_TYPE_SKBIDX:
    if (*skbCount == mist_skb_idx) {
      logSKB = 1;
    }
    break;
  case MIST_C_TYPE_MOD_CAP:
    if (module == mist_skb_mod && alloc_cap_point == mist_alloc_cap_point) {
      logSKB = 1;
    }
    break;
  case MIST_C_TYPE_MOD_ONLY:
    if (module == mist_skb_mod) {
      logSKB = 1;
    }
    break;
  case MIST_C_TYPE_ALL:
    logSKB = 1;
    break;
  case MIST_C_TYPE_NONE:
  default:
    logSKB = 0;
    break;
  }

  if (mist_skb_Cntr > 0 && mist_logged_pkt_cnt >= mist_skb_Cntr) {
    logSKB = 0;
  }

  if (logSKB) {
    seq_printf(m,
               "\n%d] skb=%px pfn=%ld len=%d module=%s alloc_cap_point=%d "
               "last_seen=%s\n",
               *skbCount, skb, virt_to_pfn(skb), skb->len, module_itoa[module],
               alloc_cap_point, module_itoa[module_last_seen]);
  }

  if (logSKB && mist_dump_cmd == 2 && skb) {
    decode_hex_pkt(m, skb);
  }
  if (logSKB) {
    ++mist_logged_pkt_cnt;
  }
}

int dump_slab_page(struct seq_file *m, struct list_head *head, int obj_size,
                   int objsPerSlab, int *skbCount) {
  struct page *page, *h;
  int i = 0;
  list_for_each_entry_safe(page, h, head, lru) {
    int j;
    uint8_t *slab_obj = NULL;
    struct sk_buff *skb = NULL;
    dprintf("%d]page=%px pfn=%ld va=%px inuse=%d\n", ++i, page,
            page_to_pfn(page), page_address(page), page->inuse);
    slab_obj = ((uint8_t *)page_address(page));
    for (j = 0; j < objsPerSlab; j++) {
      skb = (struct sk_buff *)(slab_obj + 0x40);
      if (skb->mist_skb_dbg_magic == MIST_SKB_DBG_MAGIC) {
        update_counters(skb);
        if (mist_skb_dbg_log_level > 1) {
          printk("pfn=%ld mist_alloced=0x%x alloced_mist_module=0x%x "
                 "mist_alloced_cap_point=0x%x last_seen_module=0x%x\n",
                 virt_to_pfn(skb), skb->mist_alloced, skb->alloced_mist_module,
                 skb->mist_alloced_cap_point, skb->last_seen_module);
        }
        if (m) {
          mist_skb_dump(m, skb, skbCount);
        }
        if (skbCount) {
          ++(*skbCount);
        }
      }
      slab_obj += obj_size;
    }
  }

  return 0;
}

void show_counters(struct seq_file *m) {
  int i, j;
  u64 total = 0;
  u64 host_tot = 0;
  u64 bridged_tot = 0;
  for (i = 0; i < MIST_MOD_LAST; i++) {
    for (j = 0; j < MIST_CAP_LAST; j++) {
      if (j == 0) {
        seq_printf(m, "\n%d] %s Total=%llu, bridged=%llu host=%llu\n", i,
                   module_itoa[i], n_stats.mist_counters[i][j].cntr,
                   n_stats.mist_counters[i][j].bridged,
                   n_stats.mist_counters[i][j].host);
      } else {
        total += n_stats.mist_counters[i][j].cntr;
        host_tot += n_stats.mist_counters[i][j].host;
        bridged_tot += n_stats.mist_counters[i][j].bridged;
        if (n_stats.mist_counters[i][j].cntr) {
          seq_printf(m, "%s Cap_Point%d_Total=%llu bridged=%llu host=%llu\n",
                     module_itoa[i], j, n_stats.mist_counters[i][j].cntr,
                     n_stats.mist_counters[i][j].bridged,
                     n_stats.mist_counters[i][j].host);
        }
      }
    }
  }
  seq_printf(m, "Total_SKB_allocated=%llu, bridged=%llu, host=%llu\n", total,
             bridged_tot, host_tot);
  seq_printf(m, "\nLast Seen on\n");
  for (i = 0; i < MIST_MOD_LAST; i++) {
    if (n_stats.mist_lastseen_counters[i].cntr) {
      seq_printf(m, "%s=%llu, bridged=%llu, host=%llu\n", module_itoa[i],
                 n_stats.mist_lastseen_counters[i].cntr,
                 n_stats.mist_lastseen_counters[i].bridged,
                 n_stats.mist_lastseen_counters[i].host);
    }
  }

  if (n_stats.mist_cntr_err) {
    seq_printf(m, "mist_cntr_err=%llu\n", n_stats.mist_cntr_err);
  }
  if (n_stats.mist_last_seen_err) {
    seq_printf(m, "mist_last_seen_err=%llu\n", n_stats.mist_last_seen_err);
  }
}

void build_stats(struct seq_file *m) {
  struct kmem_cache *s = skbuff_head_cache;
  int node;
  int skbCount = 0;
  struct kmem_cache_node *n;
  int objsperslab;
  objsperslab = oo_objects(s->oo);
  dprintf("slab_name=%s size=%d objsPerSlab=%d\n", s->name, s->size,
          objsperslab);
  for_each_kmem_cache_node(s, node, n) {
    unsigned long slabs_total, slabs_partial, node_nr_objs;
    slabs_partial = n->nr_partial;
    slabs_total = slabs_node(s, node);
    node_nr_objs = atomic_long_read(&n->total_objects);
    dprintf("node=%d n=%px slabs_total=%lu slabs_partial=%lu slabs_full=%lu "
            "tot_objs=%lu\n",
            node, n, slabs_total, slabs_partial, (slabs_total - slabs_partial),
            node_nr_objs);
    dump_slab_page(m, &n->partial, s->size, objsperslab, &skbCount);
    dump_slab_page(m, &n->full, s->size, objsperslab, &skbCount);
  }
}

int mist_skb_debug_dump_show(struct seq_file *m, void *arg) {
  memset(&n_stats, 0x0, sizeof(mist_stats_t));
  mist_logged_pkt_cnt = 0;
  build_stats(NULL);
  show_counters(m);
  seq_printf(m, "\n");
  return 0;
}

int skb_dump_show(struct seq_file *m, void *arg) {
  memset(&n_stats, 0x0, sizeof(mist_stats_t));
  mist_dump_cmd = 1;
  mist_logged_pkt_cnt = 0;
  build_stats(m);
  mist_dump_cmd = 0;
  return 0;
}

int skb_dump_single_show(struct seq_file *m, void *arg) {
  memset(&n_stats, 0x0, sizeof(mist_stats_t));
  mist_dump_cmd = 2;
  mist_logged_pkt_cnt = 0;
  build_stats(m);
  mist_dump_cmd = 0;
  return 0;
}

int __init mist_skb_dbg_init(void) {
  mist_skb_dbg_proc_dir = proc_mkdir("mist_skb_debug", NULL);
  pr_info("MIST SKB Debug Module INIT\n");
  if (mist_skb_dbg_proc_dir) {
    mist_proc_create_single(config, mist_skb_dbg_proc_dir, config_wr);
    mist_proc_create_single(mist_skb_debug_dump, mist_skb_dbg_proc_dir, NULL);
    mist_proc_create_single(skb_dump, mist_skb_dbg_proc_dir, NULL);
    mist_proc_create_single(skb_dump_single, mist_skb_dbg_proc_dir, NULL);
  }
  if (skbuff_head_cache) {
    printk("[%s] slab=%px %s size=%d object_size=%d\n", __func__,
           skbuff_head_cache, skbuff_head_cache->name, skbuff_head_cache->size,
           skbuff_head_cache->object_size);
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

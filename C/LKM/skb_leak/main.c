#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <slab.h>
#include <linux/skbuff.h>

int ncc_log_level = 0;
int ncc_mod = 0;
int ncc_cap_point = 0;
int ncc_cap_type = 0;
int ncc_skb_idx = 0;

#if 1
#define dprintf(x,...) do { if(ncc_log_level >= 1) pr_info(x, ##__VA_ARGS__); } while (0)
#define ddprintf(x,...) do { if(ncc_log_level >= 2) pr_info(x, ##__VA_ARGS__); } while (0)
#define dddprintf(x,...) do { if(ncc_log_level >= 3) pr_info(x, ##__VA_ARGS__); } while (0)
#else
#define dprintf(x,...) do { } while (0)
#define ddprintf(x,...) do { } while (0)
#define dddprintf(x,...) do { } while (0)
#endif

#define proc_create_single(name, dir, write_func) { \
        auto int name##_open(struct inode* in, struct file* file) { return single_open(file, name##_show, NULL); } \
        static struct file_operations name##_proc_ops = { .owner = THIS_MODULE, .read = seq_read, .write = (write_func), .llseek = seq_lseek, .release = single_release }; \
        name##_proc_ops.open = name##_open; \
        if (!proc_create(#name, 0644, (dir), &name##_proc_ops)) { pr_err("proc_create(\"#name\") failed\n"); } \
}

struct proc_dir_entry *ncc_proc_dir = NULL;

/* Tracking of the number of slabs for debugging purposes */
static inline unsigned long slabs_node(struct kmem_cache *s, int node) {
	struct kmem_cache_node *n = get_node(s, node);

	return atomic_long_read(&n->nr_slabs);
}

#define OO_SHIFT        16
#define OO_MASK         ((1 << OO_SHIFT) - 1)
static inline int oo_objects(struct kmem_cache_order_objects x) {
	return x.x & OO_MASK;
}

extern struct kmem_cache *skbuff_head_cache;

/*
#define MIST_MOD_OTHERS         0x0
#define MIST_MOD_WXLAN          0x1
#define MIST_MOD_BENET          0x2
#define MIST_MOD_PACE           0x3
#define MIST_MOD_WL             0x4
#define MIST_MOD_BLE            0x5
#define MIST_MOD_LAST           0x6
*/

char *module_itoa[MIST_MOD_LAST] = {
	"Others",
	"MIST_WXLAN",
	"MIST_BENET",
	"MIST_PACE",
	"MIST_WL",
	"MIST_BLE",
	"MIST_SKBUFF" "KERNEL"
};

#define NCC_C_TYPE_NONE 0x0
#define NCC_C_TYPE_SKBIDX 0x1
#define NCC_C_TYPE_MOD_CAP 0x2
#define NCC_C_TYPE_MOD_ONLY 0x3
#define NCC_C_TYPE_ALL 0x4
#define NCC_C_TYPE_LAST 0x5

char *ctype_itoa[NCC_C_TYPE_LAST] = {
	"None",
	"skb matching SKBidx",
	"skbs matching module & cap_point",
	"skbs matching module",
	"all skbs",
};

typedef struct {
	u64 mist_counters[MIST_MOD_LAST][MIST_CAP_LAST];
	u64 mist_cntr_err, mist_last_seen_err;
	u64 mist_lastseen_counters[MIST_MOD_LAST];
} ncc_stats_t;

ncc_stats_t n_stats;

static int config_show(struct seq_file *m, void *arg) {
	seq_printf(m, "LogLevel(L)=%d\n", ncc_log_level);
	seq_printf(m, "Mod(M)=%d\n", ncc_mod);
	seq_printf(m, "CapPoint(C)=%d\n", ncc_cap_point);
	seq_printf(m, "CaptureType(T)=%d(%s)\n", ncc_cap_type, ctype_itoa[ncc_cap_type]);
	seq_printf(m, "SKBidx(I)=%d\n", ncc_skb_idx);
	return 0;
}

static ssize_t config_wr(struct file *filp, const char __user * buf, size_t count, loff_t * loff) {
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
			ncc_log_level = (int)res;
			break;
		case 'M':
			ncc_mod = (int)res;
			break;
		case 'C':
			ncc_cap_point = (int)res;
			break;
		case 'I':
			ncc_skb_idx = (int)res;
			break;
		case 'T':
			ncc_cap_type = (int)res;
			break;
		}
	}
 exit:
	return count;
}

void update_counters(uint16_t flags, uint16_t last_seen) {
	int module, cap_point, module_last_seen;
	module = ((flags >> 12) & 0xf);
	cap_point = (flags & 0xfff);
	module_last_seen = (last_seen & 0xf);

	if (module >= 0 && module < MIST_MOD_LAST && cap_point >= 0 && cap_point < MIST_CAP_LAST) {
		n_stats.mist_counters[module][cap_point] += 1;
		n_stats.mist_counters[module][0] += 1;
	} else {
		n_stats.mist_cntr_err += 1;
		dprintf("NCC[%s] module=%d cap_point=%d flags=%x\n", __func__, module, cap_point, flags);
	}

	if (module_last_seen >= 0 && module_last_seen < MIST_MOD_LAST) {
		n_stats.mist_lastseen_counters[module_last_seen] += 1;
	} else {
		n_stats.mist_last_seen_err += 1;
		dprintf("NCC[%s] module=%d cap_point=%d flags=%x\n", __func__, module, cap_point, flags);
	}

}

static void ncc_skb_dump(struct seq_file *m, struct sk_buff *skb, int *skbCount) {
	int module = 0, cap_point = 0, module_last_seen = 0, logSKB = 0;
	if (m == NULL || skb == NULL || skbCount == NULL) {
		return;
	}
	module = ((skb->ncc_flags >> 12) & 0xf);
	cap_point = (skb->ncc_flags & 0xfff);
	module_last_seen = (skb->ncc_last_seen & 0xf);

	switch (ncc_cap_type) {
	case NCC_C_TYPE_SKBIDX:
		if (*skbCount == ncc_skb_idx) {
			logSKB = 1;
		}
		break;
	case NCC_C_TYPE_MOD_CAP:
		if (module == ncc_mod && cap_point == ncc_cap_point) {
			logSKB = 1;
		}
		break;
	case NCC_C_TYPE_MOD_ONLY:
		if (module == ncc_mod) {
			logSKB = 1;
		}
		break;
	case NCC_C_TYPE_ALL:
		logSKB = 1;
		break;
	case NCC_C_TYPE_NONE:
	default:
		logSKB = 0;
		break;
	}

	if (logSKB) {
		seq_printf(m, "%d] skb=%px len=%d module=%s cap_point=%d last_seen=%s\n", *skbCount, skb, skb->len, module_itoa[module], cap_point, module_itoa[module_last_seen]);
	} else {
		dddprintf("NCC[%s:%d]logSKB=%d,skbCount=%d ncc_cap_type=%d\n", __func__, __LINE__, logSKB, *skbCount, ncc_cap_type);
	}
}

static int dump_slab_page(struct seq_file *m, struct list_head *head, int obj_size, int objsPerSlab, int *skbCount) {
	struct page *page, *h;
	int i = 0;
	list_for_each_entry_safe(page, h, head, lru) {
		int j;
		struct sk_buff *skb = NULL;
		dprintf("%d]page=%px 0x%lx va=%px inuse=%d\n", ++i, page, page_to_pfn(page), page_address(page), page->inuse);
		skb = (struct sk_buff *)page_address(page);
		for (j = 0; j < objsPerSlab; j++) {
			if (skb->ncc_alloced == SKB_MIST_MAGIC) {
				update_counters(skb->ncc_flags, skb->ncc_last_seen);
				if (ncc_log_level > 1) {
					printk("pfn=0x%lx ncc_alloced=0x%x ncc_flags=0x%x\n", virt_to_pfn(skb), skb->ncc_alloced, skb->ncc_flags);
				}
				if (m) {
					ncc_skb_dump(m, skb, skbCount);
				}
				if (skbCount) {
					++(*skbCount);
				}
			}
			skb = (struct sk_buff *)(((uint8_t *) skb) + obj_size);
		}
	}

	return 0;
}

static void show_counters(struct seq_file *m) {
	int i, j;
	u64 total = 0;
	for (i = 0; i < MIST_MOD_LAST; i++) {
		for (j = 0; j < MIST_CAP_LAST; j++) {
			if (j == 0) {
				seq_printf(m, "\n%d] %s Total=%llu\n", i, module_itoa[i], n_stats.mist_counters[i][j]);
			} else {
				total += n_stats.mist_counters[i][j];
				if (n_stats.mist_counters[i][j]) {
					seq_printf(m, "%s Cap_Point%d_Total=%llu\n", module_itoa[i], j, n_stats.mist_counters[i][j]);
				}
			}
		}
	}
	seq_printf(m, "Total_SKB_allocated=%llu\n", total);
	seq_printf(m, "\nLast Seen on\n");
	for (i = 0; i < MIST_MOD_LAST; i++) {
		if (n_stats.mist_lastseen_counters[i]) {
			seq_printf(m, "%s=%llu\n", module_itoa[i], n_stats.mist_lastseen_counters[i]);
		}
	}
	if (n_stats.mist_cntr_err) {
		seq_printf(m, "mist_cntr_err=%llu\n", n_stats.mist_cntr_err);
	}
	if (n_stats.mist_last_seen_err) {
		seq_printf(m, "mist_last_seen_err=%llu\n", n_stats.mist_last_seen_err);
	}
}

static void build_stats(struct seq_file *m) {
	//struct kmem_cache *s = ncc_head_cache;
	struct kmem_cache *s = skbuff_head_cache;
	int node;
	int skbCount = 0;
	struct kmem_cache_node *n;
	int objsperslab;
	objsperslab = oo_objects(s->oo);
	dprintf("slab_name=%s size=%d objsPerSlab=%d\n", s->name, s->size, objsperslab);
	for_each_kmem_cache_node(s, node, n) {
		unsigned long slabs_total, slabs_partial, node_nr_objs;
		slabs_partial = n->nr_partial;
		slabs_total = slabs_node(s, node);
		node_nr_objs = atomic_long_read(&n->total_objects);
		dprintf("node=%d n=%px slabs_total=%lu slabs_partial=%lu slabs_full=%lu tot_objs=%lu\n", node, n, slabs_total, slabs_partial, (slabs_total - slabs_partial), node_nr_objs);
		dump_slab_page(m, &n->partial, s->size, objsperslab, &skbCount);
		dump_slab_page(m, &n->full, s->size, objsperslab, &skbCount);
	}
}

static int ncc_dump_show(struct seq_file *m, void *arg) {

	memset(&n_stats, 0x0, sizeof(ncc_stats_t));
	build_stats(NULL);

	show_counters(m);
	seq_printf(m, "\n");
	return 0;
}

static int skb_dump_show(struct seq_file *m, void *arg) {
	memset(&n_stats, 0x0, sizeof(ncc_stats_t));
	build_stats(m);
	return 0;

}

static int __init ncc_init(void) {
	ncc_proc_dir = proc_mkdir("ncc_module", NULL);
	pr_info("NCC[%s:%d] LKM Module INIT\n", __func__, __LINE__);
	if (ncc_proc_dir) {
		proc_create_single(ncc_dump, ncc_proc_dir, NULL);
		proc_create_single(config, ncc_proc_dir, config_wr);
		proc_create_single(skb_dump, ncc_proc_dir, NULL);
	}
	if (skbuff_head_cache) {
		printk("NCC[%s] slab=%px %s size=%d object_size=%d\n", __func__, skbuff_head_cache, skbuff_head_cache->name, skbuff_head_cache->size, skbuff_head_cache->object_size);
	}
	return 0;
}

static void __exit ncc_exit(void) {
	pr_info("NCC[%s:%d] LKM Module EXIT\n", __func__, __LINE__);

	proc_remove(ncc_proc_dir);
}

module_init(ncc_init);
module_exit(ncc_exit);

MODULE_AUTHOR("Nishad");
MODULE_DESCRIPTION("Simple LKM");
MODULE_LICENSE("GPL");

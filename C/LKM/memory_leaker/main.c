#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>

#define MODULE_NAME "page_alloc_proc"
#define PROC_ALLOC_ENTRY "page_alloc"
#define PROC_FREE_ENTRY "page_free"

struct page_node {
    struct list_head list;
    struct page *page;
    int order;
};

static LIST_HEAD(page_list);
static struct proc_dir_entry *proc_alloc_entry;
static struct proc_dir_entry *proc_free_entry;

static ssize_t nishad_proc_alloc_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
    int order = 0;
    char input[10] = {0};
    struct page_node *node;

    if (count > sizeof(input) - 1)
        return -EINVAL;

    if (copy_from_user(input, buffer, count))
        return -EFAULT;

    input[count] = '\0';
    kstrtoint(input, 10, &order);

    if (order < 0) {
        printk(KERN_ERR "%s: Invalid order value\n", MODULE_NAME);
        return -EINVAL;
    }

    // Allocate memory pages with the specified order
    node = kmalloc(sizeof(*node), GFP_KERNEL);
    if (!node) {
        printk(KERN_ERR "%s: Failed to allocate memory for node\n", MODULE_NAME);
        return -ENOMEM;
    }

    node->page = alloc_pages(GFP_KERNEL, order);
    if (!node->page) {
        printk(KERN_ERR "%s: Failed to allocate pages\n", MODULE_NAME);
        kfree(node);
        return -ENOMEM;
    }

    node->order = order;
    list_add_tail(&node->list, &page_list);

    printk(KERN_INFO "%s: %d pages allocated and added to the list (order: %d)\n", MODULE_NAME, 1 << order, order);

    return count;
}

static ssize_t nishad_proc_free_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
    int order = 0;
    char input[10] = {0};
    struct page_node *node, *tmp;

    if (count > sizeof(input) - 1)
        return -EINVAL;

    if (copy_from_user(input, buffer, count))
        return -EFAULT;

    input[count] = '\0';
    kstrtoint(input, 10, &order);

    if (order < 0) {
        printk(KERN_ERR "%s: Invalid order value\n", MODULE_NAME);
        return -EINVAL;
    }

    // Iterate through the list and free pages with the matching order
    list_for_each_entry_safe(node, tmp, &page_list, list) {
        if (node->order == order) {
            printk(KERN_INFO "%s: Freeing %d pages at virtual address %p (order: %d)\n",
                   MODULE_NAME, 1 << order, page_address(node->page), order);

            free_pages((unsigned long)page_address(node->page), order);

            list_del(&node->list);
            kfree(node);

            printk(KERN_INFO "%s: Pages freed\n", MODULE_NAME);
        }
    }

    return count;
}

static const struct file_operations proc_alloc_file_ops = {
    .owner = THIS_MODULE,
    .write = nishad_proc_alloc_write,
};

static const struct file_operations proc_free_file_ops = {
    .owner = THIS_MODULE,
    .write = nishad_proc_free_write,
};

static int __init page_alloc_proc_init(void)
{
    printk(KERN_INFO "%s: Initializing the module\n", MODULE_NAME);

    // Create proc entry for allocating pages
    proc_alloc_entry = proc_create(PROC_ALLOC_ENTRY, 0666, NULL, &proc_alloc_file_ops);
    if (!proc_alloc_entry) {
        printk(KERN_ERR "%s: Failed to create proc entry for allocation\n", MODULE_NAME);
        return -ENOMEM;
    }

    // Create proc entry for freeing pages
    proc_free_entry = proc_create(PROC_FREE_ENTRY, 0666, NULL, &proc_free_file_ops);
    if (!proc_free_entry) {
        printk(KERN_ERR "%s: Failed to create proc entry for freeing pages\n", MODULE_NAME);
        proc_remove(proc_alloc_entry);
        return -ENOMEM;
    }

    printk(KERN_INFO "%s: Proc entries created successfully\n", MODULE_NAME);
    return 0;
}

static void __exit page_alloc_proc_exit(void)
{
    struct page_node *node, *tmp;

    // Free all allocated pages on module exit
    list_for_each_entry_safe(node, tmp, &page_list, list) {
        printk(KERN_INFO "%s: Freeing pages of order %d on module exit\n", MODULE_NAME, node->order);
        free_pages((unsigned long)page_address(node->page), node->order);
        list_del(&node->list);
        kfree(node);
    }

    if (proc_alloc_entry)
        proc_remove(proc_alloc_entry);

    if (proc_free_entry)
        proc_remove(proc_free_entry);

    printk(KERN_INFO "%s: Module exited and all pages freed\n", MODULE_NAME);
}

module_init(page_alloc_proc_init);
module_exit(page_alloc_proc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nishad");
MODULE_DESCRIPTION("Kernel module to allocate and free pages using the page allocator via proc entries");


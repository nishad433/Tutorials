#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include"ncc.h"

int ncc_open(struct inode *inodep, struct file *filep){
	struct mmap_info *info;
	pr_info("%s\n",__func__);
	info = kmalloc(sizeof(struct mmap_info), GFP_KERNEL);
	pr_info("virt_to_phys = 0x%llx\n",(unsigned long long)virt_to_phys((void *)info));
	info->data = (char *)get_zeroed_page(GFP_KERNEL);
	memcpy(info->data,"asdf",BUFFER_SIZE);
	filep->private_data = info;
	return 0;
}

int ncc_release(struct inode *inodep, struct file *filep){
	struct mmap_info *info;
	pr_info("%s\n",__func__);
	info = filep->private_data;
	free_page((unsigned long)info->data);
	kfree(info);
	filep->private_data = NULL;
	return 0;
}

ssize_t ncc_read(struct file *filep, char __user *buf, size_t len, loff_t *off){
	struct mmap_info *info;
	ssize_t ret;
	pr_info("%s\n",__func__);
	if((ssize_t)BUFFER_SIZE <= *off){
		ret = 0;
	}else{
		info = filep->private_data;
		ret = min(len, (size_t)BUFFER_SIZE - (size_t)*off);
		if(copy_to_user(buf, info->data + *off, ret)){
			ret = -EFAULT;
		}else{
			*off += ret;
		}
	}
	return 0;
}

ssize_t ncc_write(struct file *filep, const char __user *buf, size_t len, loff_t *off){
	struct mmap_info *info;
	ssize_t ret;
	pr_info("%s\n",__func__);
	info = filep->private_data;
	if(copy_from_user(info->data, buf, min(len, (size_t)BUFFER_SIZE))){
		ret = -EFAULT;
	}else{
		ret = len;
	}
	return ret;
}

long ncc_ioctl(struct file *filep, unsigned int cmd, unsigned long arg){
	pr_info("%s\n",__func__);
	return 0;
}

struct file_operations ncc_fops = {
    	.open		= ncc_open,
    	.release	= ncc_release,
	.read		= ncc_read,
	.write		= ncc_write,
    	.unlocked_ioctl	= &ncc_ioctl,
    	.mmap		= ncc_mmap,
};

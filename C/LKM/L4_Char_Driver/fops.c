#include <linux/fs.h>
#include"ncc.h"

int ncc_open(struct inode *inode, struct file *file){
	return 0;
}

int ncc_release(struct inode *inode, struct file *file){
	return 0;
}

long ncc_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	return 0;
}

struct file_operations ncc_fops = {
    .open   = &ncc_open,
    .release   = &ncc_release,
    .unlocked_ioctl = &ncc_ioctl,
};

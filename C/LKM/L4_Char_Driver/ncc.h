#ifndef _NCC_H_

#define _NCC_H_

#define DEVICE_NAME "ncc-dev"
#define BUFFER_SIZE 4
struct mmap_info {
        char *data;
};

extern struct file_operations ncc_fops;
int ncc_mmap(struct file *filep, struct vm_area_struct *vma);

#endif

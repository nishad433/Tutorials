#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/version.h>
#include"ncc.h"

void ncc_vm_open(struct vm_area_struct * area){
	pr_info("%s\n",__func__);
}

void ncc_vm_close(struct vm_area_struct * area){
	pr_info("%s\n",__func__);
}

/* First page access. */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0))
vm_fault_t ncc_vm_fault(struct vm_fault *vmf){
#else
int ncc_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf){
#endif
	struct page *page;
	struct mmap_info *info;
	pr_info("%s\n",__func__);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0))
	info = (struct mmap_info *)vmf->vma->vm_private_data;
#else
	info = (struct mmap_info *)vma->vm_private_data;
#endif
	if(info->data){
		page  =virt_to_page(info->data);
		get_page(page);
		vmf->page=page;
	}
	return 0;
}

struct vm_operations_struct ncc_vm_ops = {
	.fault = ncc_vm_fault,
	.close = ncc_vm_close,
	.open = ncc_vm_open,
};

int ncc_mmap(struct file *filep, struct vm_area_struct *vma){
	pr_info("%s\n",__func__);
	vma->vm_ops = &ncc_vm_ops;
	vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	vma->vm_private_data = filep->private_data;
	ncc_vm_open(vma);
	return 0;
}

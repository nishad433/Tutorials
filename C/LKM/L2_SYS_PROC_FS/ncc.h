#ifndef _NCC_H_

#define _NCC_H_

#include<linux/printk.h>

void create_sysfs_entry(void);
extern struct kobject *ncc_kobj;


#endif

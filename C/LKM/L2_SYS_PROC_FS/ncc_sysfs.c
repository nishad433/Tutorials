#include"ncc.h"
#include <linux/device.h>
#include <linux/sysfs.h>


#define NCC_DEVICE_ATTR(name) \
        ssize_t store_##name(struct device *dev, struct device_attribute *attr,const char *buf, size_t count) \
        { \
            pr_info("NCC[%s:%d] buf=%s\n",__func__,__LINE__,buf);    \
            return count;\
        } \
        ssize_t show_##name(struct device *dev, struct device_attribute *attr,char *buf)         \
        { \
            return snprintf(buf,PAGE_SIZE,"sample text");\
        } \
        static DEVICE_ATTR(name, 0664, show_##name, store_##name);



struct kobject *ncc_kobj;

// single sysfs entry
NCC_DEVICE_ATTR(ncc_test);


NCC_DEVICE_ATTR(ncc_1);

struct attribute *ncc_attr[]={
        &dev_attr_ncc_1.attr,
        NULL,
};

const struct attribute_group ncc_group = {
    .name = "ncc",
    .attrs = (struct attribute **)&ncc_attr,

};


//
const struct attribute_group *ncc_groups[]={
    &ncc_group,
    NULL,
};

void create_sysfs_entry(void)
{
        // create ncc_module dir in /sys/
        ncc_kobj=kobject_create_and_add("ncc_module", NULL);
        if(!ncc_kobj)
                pr_err("NCC[%s:%d] error creating sysfs object\n",__func__,__LINE__);

        // creates single sysfs entry ncc_test
        if(sysfs_create_file(ncc_kobj,&dev_attr_ncc_test.attr))
                pr_err("NCC[%s:%d] error creating sysfs entry\n",__func__,__LINE__);


        // create sysfs group ncc
        if(sysfs_create_groups(ncc_kobj, ncc_groups))
                pr_err("NCC[%s:%d] error create sysfs group\n",__func__,__LINE__);


}



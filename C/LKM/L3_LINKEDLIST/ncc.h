#ifndef _NCC_H_

#define _NCC_H_

#include<linux/list.h>

typedef struct {
	struct list_head ncc_list;
	int a;
}ncc_t;

#endif

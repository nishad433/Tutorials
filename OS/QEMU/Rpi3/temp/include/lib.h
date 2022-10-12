#ifndef _LIB_H_
#define _LIB_H_

#include<stdint.h>

void reg_write(uint64_t addr, uint32_t value);
uint32_t reg_read(uint64_t addr);

#endif

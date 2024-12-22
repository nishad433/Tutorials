#ifndef _RNG_H_
#define _RNG_H_

#include<platform.h>
#include<stdint.h>

#ifdef RNG_SUPPORT


typedef struct{
	rw_REG32 rng_ctrl;
	rw_REG32 rng_status;
	rw_REG32 rng_data;
	ro_REG32 reserved;
	rw_REG32 rng_intr_mask;
}rng_regs_t;

#define RNG_REGS   ((volatile rng_regs_t *)RNG_BASE)

int rng_init(void);
uint32_t rand(uint32_t min, uint32_t max);

#endif //RNG_SUPPORT

#endif //_RNG_H_

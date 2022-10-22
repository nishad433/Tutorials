#ifndef _RNG_H_
#define _RNG_H_

#include<platform.h>
#include<stdint.h>

#ifdef RNG_SUPPORT


typedef struct{
	uint32_t rng_ctrl;
	uint32_t rng_status;
	uint32_t rng_data;
	uint32_t reserved;
	uint32_t rng_intr_mask;
}rng_regs_t;

#define RNG_REGS   ((volatile rng_regs_t *)RNG_BASE)

int rng_init(void);
uint32_t rand(uint32_t min, uint32_t max);

#endif //RNG_SUPPORT

#endif //_RNG_H_

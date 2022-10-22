#include <rng.h>



int rng_init(void){
	RNG_REGS->rng_status = 0x40000;
	RNG_REGS->rng_intr_mask |= 1;
	RNG_REGS->rng_ctrl |= 1;
	return 0;
}

uint32_t rand(uint32_t min, uint32_t max){
	while(!((RNG_REGS->rng_status)>>24)) {asm volatile("nop");}
	return ( RNG_REGS->rng_data % (max - min) ) + min;

}

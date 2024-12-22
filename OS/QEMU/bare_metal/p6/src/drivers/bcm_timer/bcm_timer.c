#include<printk.h>
#include<bcm_timer.h>

void bcm_timer_init(void){
	int val = BCM_TIMER_REGS->clo;
	val += 200000;
	BCM_TIMER_REGS->c1 = val;
}


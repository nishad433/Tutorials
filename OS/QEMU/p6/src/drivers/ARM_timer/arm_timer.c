#include<printk.h>
#include<irq.h>
#include<arm_timer.h>


// Enable virtual timer
void enable_cntv(void){
	uint32_t cntv_ctl = 0x1;
	asm volatile("msr cntv_ctl_el0, %0" :: "r" (cntv_ctl));
}

// Disable virtual timer
void disable_cntv(void){
	uint32_t cntv_ctl = 0x0;
	asm volatile("msr cntv_ctl_el0, %0" :: "r" (cntv_ctl));
}

uint64_t read_cntvct(void){
	uint64_t val;
	asm volatile("mrs %0, cntvct_el0" : "=r" (val));
	return val;
}
uint64_t read_cntv_tval(void){
	uint64_t val;
	asm volatile("mrs %0, cntv_tval_el0" : "=r" (val));
	return val;
}

void write_cntv_tval(uint32_t val)
{
	asm volatile ("msr cntv_tval_el0, %0" :: "r" (val));
    	return;
}

uint32_t read_cntfrq(void)
{
    	uint32_t val;
	asm volatile ("mrs %0, cntfrq_el0" : "=r" (val));
    	return val;
}

void arm_timer_irq_init(void){
	IRQ_CORE_REGS->irq_timer_core0 = 1<< CNTVIRQ_SRC_BIT;
}

static inline void io_halt(void)
{
    	asm volatile ("wfi");
}

void timer_irq_pending(void){
	uint32_t val, cntvct, cntfrq;
	cntfrq = read_cntfrq();
	if(IRQ_CORE_REGS->irq_src_core0 & (1<< CNTVIRQ_SRC_BIT)){
		val = read_cntv_tval();
		cntvct = read_cntvct();
		printk("CNTV_TVAL : %x  handler CNTVCT : %x\n", val, cntvct);	
		write_cntv_tval(cntfrq); // clear cntv interrupt and set next 1sec timer.
	}
}

void arm_timer_init(void){
	uint32_t val, cntvct, cntfrq;
	cntfrq = read_cntfrq();
	write_cntv_tval(cntfrq);
	val = read_cntv_tval();
	cntvct = read_cntvct();
	printk("CNTFRQ : %d KHz\n", cntfrq/1000);	
	printk("CNTV_TVAL : %x  handler CNTVCT : %x\n", val, cntvct);	
	arm_timer_irq_init();
	enable_cntv();  
}

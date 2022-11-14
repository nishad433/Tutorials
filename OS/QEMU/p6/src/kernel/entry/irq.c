#include<printk.h>
#include<irq.h>

void enable_interrupt_controller(void){
	IRQ_REGS->enable_irqs_1 = (1<<TIMER_IRQ);

}

void irq_init(void){
	irq_vectors_init();
	enable_interrupt_controller();
	enable_irq();
}

void do_irq(void){
	disable_irq();
	timer_irq_pending();
	enable_irq();
}

void show_msg(int type, unsigned long esr, unsigned long elr){
	printk("%d ESR: %x, Addr: %x\n",type, esr,elr);

}

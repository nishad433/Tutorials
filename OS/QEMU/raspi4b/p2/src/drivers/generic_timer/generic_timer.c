#include<printk.h>
#include<irq.h>
#include<generic_timer.h>

uint64_t tick;

void generic_timer_init(void){
    setup_CNTP_CTL();
    set_CNTP_TVAL(SYS_FREQ);
    gic_enable_irq(GIC_NS_PHYS_TIMER_IRQ, 0);
}


void handle_generic_timer()
{
    printk("Tick %d\n",tick++);
    set_CNTP_TVAL(SYS_FREQ);
}

#include<printk.h>
#include<irq.h>
#include<generic_timer.h>

uint64_t tick;
uint64_t sys_freq;
void generic_timer_init(void){
    sys_freq = get_sys_frequency();
    printk("SYS_FREQ=%d\n",sys_freq);
    setup_CNTP_CTL();
    set_CNTP_TVAL(sys_freq);
    gic_enable_irq(GIC_NS_PHYS_TIMER_IRQ, 0);
}


void handle_generic_timer()
{
    printk("Tick %d\n",tick++);
    set_CNTP_TVAL(sys_freq);
}

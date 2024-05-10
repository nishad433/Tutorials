#include<printk.h>
#include<irq.h>

void irq_controller_init(){
        gic_init();
}

void irq_init(void){
        irq_vectors_init();
        irq_controller_init();
        enable_irq();
}

void do_irq(void){
        disable_irq();
        uint32_t iar = GIC_CPU_INTF_REGS->GICC_IAR;
        uint32_t irq = iar & 0x3ff;
        uint32_t cpu_id = (iar >>10) &  7;
        switch (irq)
        {
        case GIC_NS_PHYS_TIMER_IRQ:
                handle_generic_timer();
                break;
        default:
                printk("NCC irq=%d cpu=%d UNKNOWN IRQ\n",irq,cpu_id);
                break;
        }

        GIC_CPU_INTF_REGS->GICC_EOIR = iar;
        enable_irq();
}

void show_msg(int type, unsigned long esr, unsigned long elr){
        printk("%d ESR: 0x%x, Addr: 0x%x\n",type, esr,elr);
}

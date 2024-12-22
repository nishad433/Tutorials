#include<irq.h>
#include<printk.h>


void gic_dist_enable_irq(int irq){
    int index = irq / 32;
    int bitshift = irq % 32;
    GIC_DIST_REGS->GICD_ISENABLERn[index] |= (1<<bitshift);
}

void gic_dist_disable_irq(int irq){
    int index = irq / 32;
    int bitshift = irq % 32;
    GIC_DIST_REGS->GICD_ICENABLERn[index] = (1<<bitshift);
}

void gic_dist_clear_pending_irq(int irq){
    int index = irq / 32;
    int bitshift = irq % 32;
    GIC_DIST_REGS->GICD_ICPENDRn[index] = (1<<bitshift);
}

void gic_dist_set_irq_priority(int irq, int priority){
    int index = irq / 4;
    int bitshift = irq % 4;
    GIC_DIST_REGS->GICD_IPRIORITYRn[index] |= ((priority & 0xff)<<(bitshift));
}

void gic_dist_set_irq_targetcore(int irq, int core){
    int index = irq / 4;
    int bitshift = irq % 4;
    GIC_DIST_REGS->GICD_ITARGETSRn[index] |= (1<<(core + bitshift));
}

void gic_dist_set_irq_config(int irq, int cfg){
    int index = irq / 16;
    int bitshift = irq % 16;
    GIC_DIST_REGS->GICD_ICFGRn[index] |= ((cfg & 3)<<(bitshift));
}


void gic_dist_init(void){
    /* Disable Distributor */
    GIC_DIST_REGS->GICD_CTLR = 0 ;

    /* Disable all IRQs*/
    for(int irq=0; irq<NUM_IRQs; irq++){
        gic_dist_disable_irq(irq);
    }

    /* Clear all pending IRQs*/
    for(int irq=0; irq<NUM_IRQs; irq++){
        gic_dist_clear_pending_irq(irq);
    }

    /* Set all of interrupt priorities as the lowest priority*/
    for(int irq=0; irq<NUM_IRQs; irq++){
        gic_dist_set_irq_priority(irq, 0xff);
    }

    /* Set target of all of shared peripherals to processor 0 */
    for(int irq=32; irq<NUM_IRQs; irq++){
        gic_dist_set_irq_targetcore(irq, 0);
    }

    /* Set trigger type for all peripheral interrupts level triggered */
    for(int irq=0; irq<NUM_PPIs; irq++){
        gic_dist_set_irq_config(irq, GIC_GICD_ICFGR_LEVEL);
    }

    /* Enable Distributor */
    GIC_DIST_REGS->GICD_CTLR |=  (1<<1 /*EnableGrp1*/ | 1<<0 /*EnableGrp0*/) ;
}

void gic_cpu_intf_init(){
    /* Disable CPU interface */
    GIC_CPU_INTF_REGS->GICC_CTLR = 0;

    /* Set the priority level as the lowest priority */
    /* Note: Higher priority corresponds to a lower Priority field value in the GIC_PMR.
     * In addition to this, writing 255 to the GICC_PMR always sets it to the 
     * largest supported priority field value.
     */
    GIC_CPU_INTF_REGS->GICC_PMR = 0xff;

    /* Handle all of interrupts in a single group */
    GIC_CPU_INTF_REGS->GICC_BPR = 0x0; /* handle all interrupts */

    /* Clear all of the active interrupts */
    for(int pending_irq = ( GIC_CPU_INTF_REGS->GICC_IAR & GICC_IAR_INTR_IDMASK ); 
            ( pending_irq != GICC_IAR_SPURIOUS_INTR );
            pending_irq = ( GIC_CPU_INTF_REGS->GICC_IAR & GICC_IAR_INTR_IDMASK ) ){
                GIC_CPU_INTF_REGS->GICC_EOIR = GIC_CPU_INTF_REGS->GICC_IAR;
            }

    /* Enable CPU interface */
    GIC_CPU_INTF_REGS->GICC_CTLR |= (1<<0 /*Enable*/);
}

void gic_init(void){
    gic_dist_init();
    gic_cpu_intf_init();
}

void gic_enable_irq(int irq, int core){
    if(irq > NUM_SPIs + 32){
        printk("Invalid IRQ");
        return;
    }
    gic_dist_set_irq_config(irq, GIC_GICD_ICFGR_EDGE);
    gic_dist_set_irq_priority(irq, 0);
    gic_dist_set_irq_targetcore(irq, 0);
    gic_dist_clear_pending_irq(irq);
    gic_dist_enable_irq(irq);
}

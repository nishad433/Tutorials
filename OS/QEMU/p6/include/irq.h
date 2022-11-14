#ifndef _IRQ_H_
#define _IRQ_H_

#include<platform.h>
#include<stdint.h>


#define TIMER_IRQ	0x1
#define CNTVIRQ_SRC_BIT	0x3

typedef struct {
    rw_REG32 irq_basic_pending;		// 0x00
    rw_REG32 irq_pending1;		// 0x04
    rw_REG32 irq_pending2;		// 0x08
    rw_REG32 fiq_cntrl;			// 0x0c
    rw_REG32 enable_irqs_1;		// 0x10
    rw_REG32 enable_irqs_2;		// 0x14
    rw_REG32 enable_basic_irqs;		// 0x18
    rw_REG32 disable_irqs_1;		// 0x1c
    rw_REG32 disable_irqs_2;		// 0x20
    rw_REG32 disable_basic_irqs;	// 0x24
}irq_regs_t;

typedef struct {
    rw_REG32 irq_timer_core0;		// 0x00
    rw_REG32 irq_timer_core1;		// 0x04
    rw_REG32 irq_timer_core2;		// 0x08
    rw_REG32 irq_timer_core3;		// 0x0c
    rw_REG32 irq_mbox_core0;		// 0x10
    rw_REG32 irq_mbox_core1;		// 0x14
    rw_REG32 irq_mbox_core2;		// 0x18
    rw_REG32 irq_mbox_core3;		// 0x1c
    rw_REG32 irq_src_core0;		// 0x20
    rw_REG32 irq_src_core1;		// 0x24
    rw_REG32 irq_src_core2;		// 0x28
    rw_REG32 irq_src_core3;		// 0x2c
    rw_REG32 irq_fiq_core0;		// 0x30
    rw_REG32 irq_fiq_core1;		// 0x34
    rw_REG32 irq_fiq_core2;		// 0x38
    rw_REG32 irq_fiq_core3;		// 0x3c
}irq_core_regs_t;



#define IRQ_REGS	((volatile irq_regs_t *)IRQ_BASE)
#define IRQ_CORE_REGS	((volatile irq_core_regs_t *)IRQ_CORE_BASE)

void irq_init(void);
void do_irq(void);
void irq_vectors_init(void);
void enable_irq(void);
void disable_irq(void);
void timer_irq_pending(void);

#endif

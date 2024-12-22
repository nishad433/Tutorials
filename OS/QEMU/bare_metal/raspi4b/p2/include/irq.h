#ifndef _IRQ_H_
#define _IRQ_H_

#include<gic.h>
#include<armc_irq.h>

void irq_init(void);
void irq_vectors_init(void);
void enable_irq(void);
void disable_irq(void);

#endif // _IRQ_H_

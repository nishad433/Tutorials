#include <asm/macros.h>

.section .text

ENTRY(irq_vectors_init)
        adr     x0, vectors
        msr     vbar_el1, x0
        ret
ENDPROC(irq_vectors_init)

ENTRY(enable_irq)
        msr     daifclr, #2
        ret
ENDPROC(enable_irq)

ENTRY(disable_irq)
        msr     daifset, #2
        ret
ENDPROC(disable_irq)

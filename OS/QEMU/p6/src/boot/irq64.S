.section .text

.global irq_vectors_init
.global enable_irq
.global disable_irq


irq_vectors_init:
	adr	x0, vectors
	msr	vbar_el1, x0
	ret

enable_irq:
	msr	daifclr, #2
	ret

disable_irq:
	msr	daifset, #2
	ret

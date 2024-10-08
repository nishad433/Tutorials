#include<arm/macro.h>

.section .text.boot
.global _start
_start:
	mrs	x0, mpidr_el1
	and	x0, x0,#0xFF		// Check processor id
	cbnz	x0, .		// Hang for all non-primary CPU

	switch_el	x0, 3f, 2f, 1f
3:	/* in EL3 */
	switch_from_el2_to_el1	el1_entry x1
2:	/* in EL2 */
	switch_from_el2_to_el1	el1_entry x1
1:	/* in EL1 */
	b	el1_entry

el1_entry:

 	/* Clear BSS Section */
        ldr	x0, =_bss_start
        ldr	x1, =_bss_end
1:      strb	wzr,[x0], #1
        cmp	x0, x1
        b.lo	1b

        /* Copy Data section from ROM to RAM */
        ldr	x0, =_rodata_end
        ldr	x1, =_data_start
        ldr	x2, =_data_end
1:      cmp	x1, x2
        ldrb	w3, [x0], #1
        strb	w3, [x1], #1
        b.le	1b


        /* Set Stack Memory */
0:      ldr	x0, =_stack_end
	mov	sp, x0
	
	bl	main

	bl	halt

.section .data

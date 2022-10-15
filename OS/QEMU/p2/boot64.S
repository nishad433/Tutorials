.section .vector_table
.global _start

_start:
        b Reset_Handler
        b . /* 0x4 Undefined Instruction */
        b . /* 0x8 Software Interrupt */
        b . /* 0xC Prefetch Abort */
        b . /* 0x10 Data Abort */
        b . /* 0x14 Reserved */
        b . /* 0x18 IRQ */
        b . /* 0x1C FIQ */

.section .text

clear_bss :
        ldr x0, =_bss_start
        ldr x1, =_bss_end
1:      str xzr,[x0], #8
        cmp x0, x1
        b.lo 1b
        ret

Reset_Handler:
      
	/* Clear BSS Section */
        bl clear_bss

        /* Copy Data section from ROM to RAM */
        ldr x0, =_text_end
        ldr x1, =_data_start
        ldr x2, =_data_end
1:      cmp x1, x2
        ldr x3, [x0], #8
        str x3, [x1], #8
        b.le 1b

        /* Set Stack Memory */
        ldr x0, =_stack_end
        ldr x1, [x0]
        mov sp, x1

	/* Copy str1 to x2 */
	ldr	x2, =str1
	ldr	x2, [x2]

	bl main

	/* loop */
	b	.

.section .data
str1:
	.word	0xDEADBEEF

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
        ldr r0, =_bss_start
        ldr r1, =_bss_end
        mov r3, #0x0
1:      cmp r0, r1
        strlt r3,[r0], #4
        blt 1b
        bx lr

Reset_Handler:
      
	/* Clear BSS Section */
        bl clear_bss

        /* Copy Data section from ROM to RAM */
        ldr r0, =_text_end
        ldr r1, =_data_start
        ldr r2, =_data_end
1:      cmp r1, r2
        ldrlt r3, [r0], #4
        strlt r3, [r1], #4
        blt 1b

	/* Set Stack Memory */
	ldr sp, =_stack_end

	bl main

	/* loop */
	b	.

.section .data

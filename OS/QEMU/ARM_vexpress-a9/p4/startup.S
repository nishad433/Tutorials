/*

Set up Vector table,
defines stack memory and set them for FIQ, IRQ and SVC mode.

*/

/* Some defines */
.equ MODE_FIQ, 0x11
.equ MODE_IRQ, 0x12
.equ MODE_SVC, 0x13


.section .vector_table
.global _Reset
_Reset:
	b Reset_Handler
	b . /* 0x4 Undefined Instruction */
	b . /* 0x8 Software Interrupt */
	b . /* 0xC Prefetch Abort */
	b . /* 0x10 Data Abort */
	b . /* 0x14 Reserved */
	b . /* 0x18 IRQ */
	b . /* 0x1C FIQ */

.section .text

setup_stack :
	/* Garbage data in stack memory */
	movw r0, 0xFEFE
	movt r0, 0xFEFE
1:	cmp r1, sp
	strlt r0, [r1], #4
	blt 1b
	bx lr

clear_bss :
	mov r3, #0x0
1:	cmp r0, r1
	strlt r3,[r0], #4
	blt 1b
	bx lr

Reset_Handler:
	/* Setup C Stack */

	/* FIQ Stack */
	msr cpsr_c, MODE_FIQ
	/* Set r1 = stack_start and sp = stack_end */
	/* Stack always grows downwards */
	ldr r1, =_fiq_stack_start
	ldr sp, =_fiq_stack_end
	bl setup_stack

	/* IRQ Stack */
	msr cpsr_c, MODE_IRQ
	ldr r1, =_irq_stack_start
	ldr sp, =_irq_stack_end
	bl setup_stack

	/* SVC Stack */
	msr cpsr_c, MODE_SVC
	ldr r1, =_stack_start
	ldr sp, =_stack_end
	bl setup_stack

	/* Copy Data section from ROM to RAM */
	ldr r0, =_text_end
	ldr r1, =_data_start
	ldr r2, =_data_end
1:	cmp r1, r2
	ldrlt r3, [r0], #4
	strlt r3, [r1], #4
	blt 1b

	/* clear the BSS */
	ldr r0, =_bss_start
	ldr r1, =_bss_end
	bl clear_bss

	bl main

	b .

.section .data

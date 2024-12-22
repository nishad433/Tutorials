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

Reset_Handler:
	b	.

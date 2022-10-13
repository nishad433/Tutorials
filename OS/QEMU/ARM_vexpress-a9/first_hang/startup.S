.section .text
.global _start

_start:
	ldr r2,=str1
	ldr r2,[r2]
	b .

.section .data
str1:
	.word 0xDEADBEEF

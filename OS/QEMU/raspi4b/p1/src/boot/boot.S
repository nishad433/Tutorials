.section .text.boot
.global _start

_start:
        mov x0, #0x1234
        b main


.section .text
main:
        b .

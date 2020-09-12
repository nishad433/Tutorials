/* HelloWorld.S */
// For system call ABI refer https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm-32_bit_EABI
// Text section starts with ".text" and data section starts with ".data"
// if ".text" and ".data" are not present, assumes Text section first, then data section
// "=" means address, "#" mean immediate value
// For GNU Assembler directives refer http://web.mit.edu/gnu/doc/html/as_7.html

.equ SYS_WRITE, 4			//Macro SYS_WRITE
.equ SYS_EXIT, 1			//Macro SYS_EXIT
.equ STDOUT, 1				//Macro STDOUT



	.text				// Optional

	.global _start

_start:
	MOV R7, #SYS_WRITE		// write system call No
	MOV R0, #STDOUT		
	LDR R1, =message		// Loads address of label "message"
	LDR R2, =next
	SUB R2, R2, R1			// size of message= addr(next) - addr(message)
	SWI 0
exit:
	MOV R7, #SYS_EXIT
	SWI 0


	.data				// optional
	.balign 4			// align 4 Byte boundary

message:
	.asciz  "Hello, world\n"
next:


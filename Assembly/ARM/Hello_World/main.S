/* HelloWorld Program */
// For system call ABI refer https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm-32_bit_EABI
// Text section starts with ".text" and data section starts with ".data"
// if ".text" and ".data" are not present, assumes Text section first, then data section
// "=" means address, "#" mean immediate value
// For GNU Assembler directives refer http://web.mit.edu/gnu/doc/html/as_7.html

# ARM System call
# Load System call# to r7 and call ""
# syscall(syscallNo, ...)           arg 2 onwrds is arguments for that syscall
# arg1 -> r0
# arg2 -> r1
# arg3 -> r2
# arg4 -> r3


.equ SYS_WRITE, 4			// Macro SYS_WRITE
.equ SYS_READ, 3			// Macro SYS_WRITE
.equ SYS_EXIT, 1			// Macro SYS_EXIT
.equ STDOUT, 1				// Macro STDOUT
.equ STDIN, 0				// Macro STDIN
.equ SIZE, 20				// MACRO size of User buff


	.text				// Optional

	.global _start

_start:
	ldr r1, =message		// Loads address of label "message"
	ldr r2, =end
	sub r2, r2, r1			// size of message= addr(end) - addr(message)
	bl write


	ldr r1, =user_buf		// Loads address of label "user_buf"
	mov r2, #SIZE
	bl read

	ldr r1, =user_buf	    
	mov r2, #SIZE
	bl write


	mov r0, #0
	bl exit


read:
	// read(fd, *buff, size)
	mov r7, #SYS_READ		// read system call No
	mov r0, #STDIN
	swi 0
	bx  lr

write:
	// write(fd, *buff, size)
	mov r7, #SYS_WRITE		// write system call No
	mov r0, #STDOUT	
	swi 0
	bx  lr

exit:
	// exit(retVal)
	mov r7, #SYS_EXIT
	swi 0
	bx  lr

	.data				// optional
	.balign 4			// align 4 Byte boundary

user_buf:
	.fill SIZE, 1			// user_buf[SIZE]	each of size 1 Byte
message:
	.asciz  "Enter String:"
end:


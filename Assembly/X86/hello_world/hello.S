/* HelloWorld Program */
// For system call ABI refer https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#x86_64-64_bit
// Text section starts with ".text" and data section starts with ".data"
// if ".text" and ".data" are not present, assumes Text section first, then data section
// For GNU Assembler directives refer http://web.mit.edu/gnu/doc/html/as_7.html

// X86 function call
// syscall(syscallNo, ...)	    arg 2 onwrds is arguments for that syscall
// arg1 -> %rax
// arg2 -> %rdi
// arg3 -> %rsi
// arg4 -> %rdx




.equ SYS_WRITE, 1                       # Macro SYS_WRITE
.equ SYS_READ, 0			# Macro SYS_READ
.equ SYS_EXIT, 60                       # Macro SYS_EXIT
.equ STDOUT, 1                          # Macro STDOUT
.equ STDIN, 0                           # Macro STDIN
.equ SIZE, 20				# Macro BUFFSIZE


        .text
	.global _start
_start:

	mov     $message, %rsi          # address of label "message"
	mov	$end, %rdx		# address of label "end"
	sub	%rsi, %rdx		# size of "message" = addr(end)-addr(message)
	call	write			# call procedure "write", pushes %eip into stack


	mov     $user_buf, %rsi
	mov	$SIZE, %rdx
	call	read

	mov     $user_buf, %rsi
	mov	$SIZE, %rdx
	call	write		

	xor     %rdi, %rdi              # we want return code 0
	call	exit


read:
	// syscall(sys_read, fd, *buff, size)
        mov     $SYS_READ, %rax		# system call 0 is read
        mov     $STDIN, %rdi		# file handle 0 is stdin
        syscall				# invoke operating system to do the write
	ret

write:
	// syscall(sys_write, fd, *buff, size)
        mov     $SYS_WRITE, %rax        # system call 1 is write
        mov     $STDOUT, %rdi           # file handle 1 is stdout
        syscall                         # invoke operating system to do the write
	ret

exit:        
	# syscall(sys_exit, retVal)
        mov     $SYS_EXIT, %rax         # system call 60 is exit
        syscall                         # invoke operating system to exit


	.data
	.balign 4
user_buf:
	.fill SIZE, 1			# user_buf[SIZE]	each of size 1 Byte
message:
        .asciz  "Enter String:"
end:

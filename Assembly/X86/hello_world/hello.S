.equ SYS_WRITE, 1                       #Macro SYS_WRITE
.equ SYS_EXIT, 60                       #Macro SYS_EXIT
.equ STDOUT, 1                          #Macro STDOUT
.equ size, 13                           #size of label "message"

	.global _start

        .text
_start:
        # write(1, message, 13)
        mov     $SYS_WRITE, %rax        # system call 1 is write
        mov     $STDOUT, %rdi           # file handle 1 is stdout
        mov     $message, %rsi          # address of string to output
        mov     $size, %rdx             # number of bytes
        syscall                         # invoke operating system to do the write

        # exit(0)
        mov     $SYS_EXIT, %rax         # system call 60 is exit
        xor     %rdi, %rdi              # we want return code 0
        syscall                         # invoke operating system to exit

	.data
message:
        .ascii  "Hello, world\n"

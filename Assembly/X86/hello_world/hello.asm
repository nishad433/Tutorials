;for Syscall Nos & armgument registers refer https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/

%define SYS_WRITE 1		;Macro SYS_WRITE
%define SYS_EXIT 60		;Macro SYS_EXIT
%define STDOUT 1		;Macro STDOUT

global _start			;_start must be declared as global for the linker

segment .text			;code segment
_start:				;tell linker entry point
    ;ssize_t write(int fd, const void *buf, size_t count);
    ;long syscall(long number, ...);
    mov rax, SYS_WRITE		;Arg1 syscall No should be in RAX
    mov rdi, STDOUT		;Arg2 fd should be in rdi
    lea rsi, [hello]		;Arg3 buf start address should be in rsi, lea: Load Effective Address, [label] => address of Label
    mov rdx, hello_size		;Arg4 buf size should be in rdx
    syscall			;Syscall instruction

    ; exit 
    ;void _exit(int status);
    mov rax, SYS_EXIT		;Syscall Exit No
    mov rdi, 0			;Exit Status
    syscall
    ret				;Return

segment .data			;Data Segment
;Bytes are allocated by define bytes db
;Words are allocated by define words dw
;EQU directive does not allocate any memory: it creates a constant value to be used by Assembler
;$ => address of current label
hello: db "Hello World",10		;Label "hello" hold data, 10 => "\n"
hello_size: equ $-hello		;$-hello is difference between start address of label "hello_size" & label "hello"

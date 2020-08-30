/* HelloWorld.S */
// For system call ABI refer https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm-32_bit_EABI
// Text section starts with ".text" and data section starts with ".data"
// if ".text" and ".data" are not present, assumes Text section first, then data section
// "=" means address, "#" mean immediate value


.equ SYS_WRITE, 4             	//Macro SYS_WRITE
.equ SYS_EXIT, 1             	//Macro SYS_EXIT
.equ STDOUT, 1               	//Macro STDOUT
.equ size, 12               	//size of label "str"


.text				// Optional

.global _start
_start:
  MOV R7, #SYS_WRITE		// write system call No
  MOV R0, #STDOUT		//
  MOV R2, #size
  LDR R1, =str			// Loads address of lable string
  SWI 0
  MOV R7, #SYS_EXIT
  SWI 0

.data				// optional

str: .ascii "Hello World\n"


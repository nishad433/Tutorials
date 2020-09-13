.equ SIZE, 20

	.text
	.global main


main:
        push {fp, lr}

        ldr r0, =message
        bl printf

	ldr r0, =fmt_str
	ldr r1, =user_buf
	bl scanf

	ldr r0, =fmt_str_l
	ldr r1, =user_buf
	bl printf

	mov r0, #0	    // return value 0
        pop {fp, pc}



	.data

	.balign 4
message:
	.asciz "Enter String:"

	.balign 4
fmt_str:
	.asciz "%s"

	.balign 4
fmt_str_l:
	.asciz "%s\n"

	.balign 4
user_buf:
	.fill SIZE,1

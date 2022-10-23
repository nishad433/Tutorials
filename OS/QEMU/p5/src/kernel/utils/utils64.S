.section .text
.global get_current_EL

get_current_EL:
	mrs x0, CurrentEL
	lsr x0, x0, #2
	and x0, x0, #2
	ret
.section .data

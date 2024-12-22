.section .text
.global get_current_EL
.global halt

get_current_EL:
	mrs	x0, CurrentEL
	lsr	x0, x0, #2
	and	x0, x0, #3
	ret

halt:
	wfi
	b halt

.section .data

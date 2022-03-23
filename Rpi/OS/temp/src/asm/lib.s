.section .text


.global reg_write
.global reg_read



reg_write:
	    str w1, [x0]
	    ret


reg_read:
	    ldr w0, [x0]
	    ret

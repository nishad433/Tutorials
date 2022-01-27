.section .text
.global ncc_printf


ncc_printf:
    
	push {fp, lr}

	bl printf

	pop {fp, pc}


#include<arm/macro.h>
#include<arm/exception.h>


.align 11   // 2^11 = 2K
.global vectors

vectors:
        kernel_ventry	sync_invalid_el1t                 // Synchronous EL1t
        kernel_ventry	irq_invalid_el1t                  // IRQ EL1t
        kernel_ventry	fiq_invalid_el1t                  // FIQ EL1t
        kernel_ventry	error_invalid_el1t                // Error EL1t

        kernel_ventry	sync_invalid_el1h                 // Synchronous EL1h
        kernel_ventry	irq_el1h                          // IRQ EL1h
        kernel_ventry	fiq_invalid_el1h                  // FIQ EL1h
        kernel_ventry	error_invalid_el1h                // Error EL1h

        kernel_ventry	sync_invalid_el0_64               // Synchronous 64-bit EL0
        kernel_ventry	irq_invalid_el0_64                // IRQ 64-bit EL0
        kernel_ventry	fiq_invalid_el0_64                // FIQ 64-bit EL0
        kernel_ventry	error_invalid_el0_64              // Error 64-bit EL0
       
	kernel_ventry	sync_invalid_el0_32	          // Synchronous 32-bit EL0
        kernel_ventry	irq_invalid_el0_32		  // IRQ 32-bit EL0
        kernel_ventry	fiq_invalid_el0_32	          // FIQ 32-bit EL0
        kernel_ventry	error_invalid_el0_32              // Error 32-bit EL0


sync_invalid_el1t:
	handle_invalid_entry	SYNC_INVALID_EL1t

sync_invalid_el1h:
	handle_invalid_entry	SYNC_INVALID_EL1h

sync_invalid_el0_64:
	handle_invalid_entry	SYNC_INVALID_EL0_64

sync_invalid_el0_32:
	handle_invalid_entry	SYNC_INVALID_EL0_32

irq_invalid_el1t:
	handle_invalid_entry	IRQ_INVALID_EL1t

irq_el1h:
        exception_entry
	bl do_irq
        exception_exit
	eret

irq_invalid_el0_64:
	handle_invalid_entry	IRQ_INVALID_EL0_64

irq_invalid_el0_32:
	handle_invalid_entry	IRQ_INVALID_EL0_32

fiq_invalid_el1t:
	handle_invalid_entry	FIQ_INVALID_EL1t

fiq_invalid_el1h:
	handle_invalid_entry	FIQ_INVALID_EL1h

fiq_invalid_el0_64:
	handle_invalid_entry	FIQ_INVALID_EL0_64

fiq_invalid_el0_32:
	handle_invalid_entry	FIQ_INVALID_EL0_32

error_invalid_el1t:
	handle_invalid_entry	ERROR_INVALID_EL1t

error_invalid_el1h:
	handle_invalid_entry	ERROR_INVALID_EL1h

error_invalid_el0_64:
	handle_invalid_entry	ERROR_INVALID_EL0_64

error_invalid_el0_32:
	handle_invalid_entry	ERROR_INVALID_EL0_32

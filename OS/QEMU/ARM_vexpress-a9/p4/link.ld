OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_Reset)

MEMORY
{
/*
	RAM(LOCAL DDR2) is mapped from 0x60000000 onwards (Refer DUI0448I_v2p_ca9_trm.pdf).
	Since we copy our code to start of RAM, Let us assume first 10M of RAM as read and execute only (or as ROM).
	We can map Data and Stack section from 0x70000000, So define RAM as starting from 0x70000000.
*/
	ROM (rx) : ORIGIN = 0x60010000, LENGTH = 10M
	RAM (rwx) : ORIGIN = 0x70000000, LENGTH = 200M
}

SECTIONS
{
/*
	QEMU is primarily used for running Linux or other Unix-like kernels, which is reflected in how it’s
	normally started. When we start QEMU with -kernel first-hang.bin , QEMU acts as if booting such
	a kernel. It copies our code to the memory location 0x10000 , that is, a 64 kilobyte offset from the
	beginning of RAM. Then it starts executing from the address 0x0.

	
	So our code is mapped from 0x60010000
*/
	.text : 
	{
        	KEEP(*(.vector_table))
        	*(.text)
		*(.rodata)
	} > ROM

	_text_end = .;

	.data : AT(ADDR(.text) + SIZEOF(.text)) 
	{
		. = ALIGN(8);
		_data_start = ADDR(.data);
		*(.data)
		_data_end = ADDR(.data) + SIZEOF(.data);
	} > RAM

	.bss (NOLOAD) : 
	{
		. = ALIGN(8);
		_bss_start = ADDR(.bss);
		*(.bss)
		_bss_end = ADDR(.bss) + SIZEOF(.bss);
	} > RAM

	_fiq_stack_start = ADDR(.bss) + SIZEOF(.bss);
	_fiq_stack_end = _fiq_stack_start + 0x1000; /* 64 KB */
	_fiq_stack_size = _fiq_stack_end - _fiq_stack_start;

	_irq_stack_start = _fiq_stack_end;
	_irq_stack_end = _irq_stack_start + 0x1000; /* 64 KB */
	_irq_stack_size = _irq_stack_end - _irq_stack_start;

	_stack_start = _irq_stack_end;
	_stack_end = _stack_start + 0x1000; /* 64 KB */
}

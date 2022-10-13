ENTRY(_start)
SECTIONS
{
/*
	QEMU is primarily used for running Linux or other Unix-like kernels, which is reflected in how it’s
	normally started. When we start QEMU with -kernel first-hang.bin , QEMU acts as if booting such
	a kernel. It copies our code to the memory location 0x10000 , that is, a 64 kilobyte offset from the
	beginning of RAM. Then it starts executing from the address 0x0.

	RAM(LOCAL DDR2) is mapped from 0x60000000 onwards (Refer DUI0448I_v2p_ca9_trm.pdf)
	
	So our code is mapped from 0x60010000
*/
	. = 0x60010000;   /* Entry Address for QEMU -M vexpress-a9 */
	.text : {
		*(.text)
	}
	.data : {
		*(.data)
	}
}

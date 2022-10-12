ENTRY(_start)
SECTIONS
{
	. = 0x60010000;   /* Entry Address for QEMU -M vexpress-a9 */
	.text : {
		*(.text)
	}
	.data : {
		*(.data)
	}
}

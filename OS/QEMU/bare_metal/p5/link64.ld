OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

MEMORY
{
        ROM (rx) : ORIGIN = 0x80000, LENGTH = 10M
        RAM (rwx) : ORIGIN = 0xA80000, LENGTH = 1024M
}

SECTIONS
{
        .text :
        {
                *(.text)
		_text_end = .;
        } > ROM

	.eh_frame :
	{
		*(.eh_frame)
	} > ROM

        .rodata : {
                *(.rodata)
                *(.rodata.str1.4)
                _rodata = . ;  /* symbol for initialization data */
        } > ROM

        .bss (NOLOAD) :
        {
                . = ALIGN(8);
                _bss_start = ADDR(.bss);
                *(.bss)
                _bss_end = ADDR(.bss) + SIZEOF(.bss);
        } > RAM

        .data : AT(_rodata)
        {
                . = ALIGN(8);
                _data_start = ADDR(.data);
                *(.data)
                _data_end = ADDR(.data) + SIZEOF(.data);
        } > RAM

        _stack_start = ADDR(.data) + SIZEOF(.data);
        _stack_end = _stack_start + 0x10000;

/*  /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }  */

}

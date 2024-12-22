OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

MEMORY
{
        ROM (rx) : ORIGIN = 0x80000, LENGTH = 1M
        RAM (rwx) : ORIGIN = 0x0, LENGTH = 2048M
}



SECTIONS
{
        .text :
        {
                *(.text.boot)
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
                _rodata_end = . ;  /* symbol for initialization data */
        } > ROM

        .bss (NOLOAD) :
        {
                _bss_start = ADDR(.bss);
                *(.bss)
                _bss_end = ADDR(.bss) + SIZEOF(.bss);
        } > RAM

        .data : AT(_rodata_end)
        {
                *(.data)
                . = ALIGN(16);
        } > RAM

        _data_start = ADDR(.data);
        _data_end = ADDR(.data) + SIZEOF(.data);

        /* stack to start from start of kernel code text and go downwards */
        _stack_start = ORIGIN(ROM);
}


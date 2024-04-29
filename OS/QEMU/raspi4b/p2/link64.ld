OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

MEMORY
{
        ROM (rx) : ORIGIN = 0x80000, LENGTH = 10M
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

        .bss (NOLOAD) :
        {
                _bss_start = ADDR(.bss);
                *(.bss)
                _bss_end = ADDR(.bss) + SIZEOF(.bss);
        } > RAM

        .data : AT(_text_end)
        {
                *(.data)
                . = ALIGN(16);
        } > RAM

        _data_start = ADDR(.data);
        _data_end = ADDR(.data) + SIZEOF(.data);

        /* stack to start from end of RAM upto size 0x80000 bytes */
        _stack_start = ORIGIN(RAM) + LENGTH(RAM);
        _stack_end = _stack_start - 0x10000;
}


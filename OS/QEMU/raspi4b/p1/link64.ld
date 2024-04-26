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
                *(.text.boot)
                *(.text)
                _text_end = .;
        } > ROM

}


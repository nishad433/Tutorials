OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

MEMORY
{
    /* Define memory regions */
    FLASH (rx) : ORIGIN = 0x80000, LENGTH = 1M      /* Flash memory for code and read-only data */
    DRAM  (rwx) : ORIGIN = 0x0, LENGTH = 2048M     /* RAM for dynamic data and stack */
}

SECTIONS
{
    /* .text section for executable code */
    .text :
    {
        *(.text.boot)         /* Bootloader code */
        *(.text)              /* General code */
        . = ALIGN(16);        /* Align the section to 16-byte boundary */
        _text_end = .;        /* End of code section */
    } > FLASH

    /* .eh_frame section for exception handling */
    .eh_frame :
    {
        *(.eh_frame)          /* Exception handling frames */
    } > FLASH

    /* .rodata section for read-only data (string constants, etc.) */
    .rodata :
    {
        *(.rodata)            /* Read-only data */
        *(.rodata.str1.4)     /* Specific read-only string data */
        . = ALIGN(16);        /* Align to 16-byte boundary */
        _rodata_end = .;      /* End of read-only data */
    } > FLASH

    /* .bss section for zero-initialized variables (not loaded from file) */
    .bss (NOLOAD) :
    {
        _bss_start = ADDR(.bss);  /* Address where .bss starts */
        *(.bss)                  /* BSS data */
        _bss_end = .;            /* End of BSS section */
    } > DRAM

    /* .data section for initialized data (loaded from file) */
    .data : AT(_rodata_end)
    {
        *(.data)                /* Initialized data */
        . = ALIGN(16);          /* Align the section to 16-byte boundary */
    } > DRAM

    _data_start = ADDR(.data);  /* Start of .data section */
    _data_end = ADDR(.data) + SIZEOF(.data);  /* End of .data section */

    /* Stack start address - placed at the start of the code in FLASH */
    _stack_start = ORIGIN(FLASH);
}


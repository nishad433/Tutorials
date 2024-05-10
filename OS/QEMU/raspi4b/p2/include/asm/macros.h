#ifndef _MACROS_H_
#define _MACROS_H_

#include <asm/sysregs.h>

/*
 * Branch according to exception level
 */
.macro  switch_el, xreg, el3_label, el2_label, el1_label
        mrs     \xreg, CurrentEL
        cmp     \xreg, 0xc
        b.eq    \el3_label
        cmp     \xreg, 0x8
        b.eq    \el2_label
        cmp     \xreg, 0x4
        b.eq    \el1_label
.endm


/*
 * Switch from EL3 to EL1 for ARMv8
 * @ep:     kernel entry point
 * @flag:   The execution state flag for lower exception
 *          level, ES_TO_AARCH64 or ES_TO_AARCH32
 * @tmp:    temporary register
 *
 * For loading 32-bit OS, x1 is machine nr and x2 is ftaddr.
 * For loading 64-bit OS, x0 is physical address to the FDT blob.
 * They will be passed to the guest.
 */
.macro switch_from_el3_to_el1, ep, tmp
        ldr     \tmp, =(SCTLR_VALUE_MMU_DISABLED)
        msr     sctlr_el1, \tmp

        ldr     \tmp, =HCR_VALUE
        msr     hcr_el2, \tmp

        ldr     \tmp, =SCR_VALUE
        msr     scr_el3, \tmp

        ldr     \tmp, =SPSR_VALUE
        msr     spsr_el3, \tmp

        adr     \tmp, \ep
        msr     elr_el3, \tmp
        eret
.endm

/*
 * Switch from EL2 to EL1 for ARMv8
 * @ep:     kernel entry point
 * @flag:   The execution state flag for lower exception
 *          level, ES_TO_AARCH64 or ES_TO_AARCH32
 * @tmp:    temporary register
 *
 * For loading 32-bit OS, x1 is machine nr and x2 is ftaddr.
 * For loading 64-bit OS, x0 is physical address to the FDT blob.
 * They will be passed to the guest.
 */
.macro switch_from_el2_to_el1, ep, tmp
        ldr     \tmp, =(SCTLR_VALUE_MMU_DISABLED)
        msr     sctlr_el1, \tmp

        ldr     \tmp, =(HCR_RW)
        msr     hcr_el2, \tmp

        ldr     \tmp, =(CPACR_FPEN_EN)
        msr     cpacr_el1, \tmp

        ldr     \tmp, =(SPSR_MASK_ALL | SPSR_EL1h)
        msr     spsr_el2, \tmp

        adr     \tmp, \ep
        msr     elr_el2, \tmp
        eret
.endm


/*
 * switch core 0 and other cores to different labels
 */
.macro switch_primary, xreg, primary_label, others_label
        mrs     \xreg, mpidr_el1
        and     \xreg, \xreg, #0xFF     // Check processor id
        cbz     \xreg, \primary_label   // branch core 0 to primary_label
        b       \others_label           // branch non core 0 to others_label
.endm

.macro kernel_ventry, label
        .align 7        // align 2^7 = 128
        b \label
.endm

.macro handle_invalid_entry,type
        exception_entry
        mov     x0, #\type
        mrs     x1, esr_el1
        mrs     x2, elr_el1
        b show_msg
        exception_exit
        eret
.endm

.macro exception_entry
        str     x30, [sp, #-8]!
        stp     x28, x29, [sp, #-16]!
        stp     x28, x29, [sp, #-16]!
        stp     x26, x27, [sp, #-16]!
        stp     x24, x25, [sp, #-16]!
        stp     x22, x23, [sp, #-16]!
        stp     x20, x21, [sp, #-16]!
        stp     x18, x19, [sp, #-16]!
        stp     x16, x17, [sp, #-16]!
        stp     x14, x15, [sp, #-16]!
        stp     x12, x13, [sp, #-16]!
        stp     x10, x11, [sp, #-16]!
        stp     x8, x9, [sp, #-16]!
        stp     x6, x7, [sp, #-16]!
        stp     x4, x5, [sp, #-16]!
        stp     x2, x3, [sp, #-16]!
        stp     x0, x1, [sp, #-16]!
.endm

.macro exception_exit
        ldp     x0, x1, [sp], #16
        ldp     x2, x3, [sp], #16
        ldp     x4, x5, [sp], #16
        ldp     x6, x7, [sp], #16
        ldp     x8, x9, [sp], #16
        ldp     x10, x11, [sp], #16
        ldp     x12, x13, [sp], #16
        ldp     x14, x15, [sp], #16
        ldp     x16, x17, [sp], #16
        ldp     x18, x19, [sp], #16
        ldp     x20, x21, [sp], #16
        ldp     x22, x23, [sp], #16
        ldp     x24, x25, [sp], #16
        ldp     x26, x27, [sp], #16
        ldp     x28, x29, [sp], #16
        ldr     x30, [sp], #8
.endm

#endif

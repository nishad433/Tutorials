#ifndef _MACROS_H_
#define _MACROS_H_

#include <asm/sysregs.h>
#include <asm/asmdefs.h>

/*
 * Switch to a specific exception level based on the current exception level.
 * 
 * @xreg:        Register to hold the current exception level.
 * @el3_label:   Label to jump to if the current exception level is EL3.
 * @el2_label:   Label to jump to if the current exception level is EL2.
 * @el1_label:   Label to jump to if the current exception level is EL1.
 */
.macro switch_el, xreg, el3_label, el2_label, el1_label
    mrs     \xreg, CurrentEL      // Read current exception level into xreg
    cmp     \xreg, #0x8           // Compare with EL3 value
    b.gt    \el3_label            // Branch to EL3 label if current level > EL3
    b.eq    \el2_label            // Branch to EL2 label if current level == EL2
    b.lt    \el1_label            // Branch to EL1 label if current level < EL2
.endm

/*
 * Switch from EL3 to EL1 for ARMv8.
 * 
 * @ep:     Entry point of the kernel.
 * @tmp:    Temporary register for intermediate values.
 * 
 * Sets up EL1, EL2, and EL3 control registers before performing an `eret` to EL1.
 */
.macro switch_from_el3_to_el1, ep, tmp
    // Disable MMU and set SCTLR_EL1
    ldr     \tmp, =(SCTLR_VALUE_MMU_DISABLED)
    msr     sctlr_el1, \tmp

    // Set HCR_EL2 to appropriate value for EL2
    ldr     \tmp, =HCR_VALUE
    msr     hcr_el2, \tmp

    // Set SCR_EL3 to appropriate value for EL3
    ldr     \tmp, =SCR_VALUE
    msr     scr_el3, \tmp

    // Set SPSR_EL3 for exception return
    ldr     \tmp, =SPSR_VALUE
    msr     spsr_el3, \tmp

    // Set ELR_EL3 to the entry point
    adr     \tmp, \ep
    msr     elr_el3, \tmp

    // Perform exception return (eret)
    eret
.endm

/*
 * Switch from EL2 to EL1 for ARMv8.
 * 
 * @ep:     Entry point of the kernel.
 * @tmp:    Temporary register for intermediate values.
 * 
 * Sets up EL1, EL2, and EL3 control registers before performing an `eret` to EL1.
 */
.macro switch_from_el2_to_el1, ep, tmp
    // Disable MMU and set SCTLR_EL1
    ldr     \tmp, =(SCTLR_VALUE_MMU_DISABLED)
    msr     sctlr_el1, \tmp

    // Set HCR_EL2 to enable correct access to EL2
    ldr     \tmp, =(HCR_RW)
    msr     hcr_el2, \tmp

    // Enable floating point and set CPACR_EL1
    ldr     \tmp, =(CPACR_FPEN_EN)
    msr     cpacr_el1, \tmp

    // Set SPSR_EL2 with appropriate flags
    ldr     \tmp, =(SPSR_MASK_ALL | SPSR_EL1h)
    msr     spsr_el2, \tmp

    // Set ELR_EL2 to the entry point
    adr     \tmp, \ep
    msr     elr_el2, \tmp

    // Perform exception return (eret)
    eret
.endm

/*
 * Branch based on whether this is the primary core (core 0) or not.
 * 
 * @xreg:            Register to store the core ID (MPIDR_EL1).
 * @primary_label:   Label to jump to for core 0 (primary core).
 * @others_label:    Label to jump to for non-core 0.
 */
.macro switch_primary, xreg, primary_label, others_label
    mrs     \xreg, mpidr_el1          // Read MPIDR_EL1 to determine core ID
    and     \xreg, \xreg, #0xFF       // Mask out all but the core ID
    cbz     \xreg, \primary_label     // If core 0, branch to primary_label
    b       \others_label             // Otherwise, branch to others_label
.endm

/*
 * Jump to the specified label, with 128-byte alignment.
 * 
 * @label:   The label to jump to.
 */
.macro kernel_ventry, label
    .align 7                      // Align to 128-byte boundary
    b \label                      // Branch to the specified label
.endm

/*
 * Handle an invalid entry (for exception handling).
 * 
 * @type:   Exception type (e.g., interrupt, fault).
 */
.macro handle_invalid_entry, type
    exception_entry                // Save current state (registers)
    mov     x0, #\type             // Set exception type in x0
    mrs     x1, esr_el1            // Read ESR_EL1 (Exception Syndrome Register)
    mrs     x2, elr_el1            // Read ELR_EL1 (Exception Link Register)
    b       show_msg               // Branch to the message handler
    exception_exit                 // Restore state (registers)
    eret                           // Return from exception
.endm

/*
 * Save the current register state onto the stack.
 */
.macro exception_entry
    // Save registers to stack (from x30 to x0)
    str     x30, [sp, #-8]!         // Save x30 (LR)
    stp     x28, x29, [sp, #-16]!   // Save x28 and x29 (FP)
    stp     x28, x29, [sp, #-16]!   // Save x28 and x29 again
    stp     x26, x27, [sp, #-16]!   // Save x26 and x27
    stp     x24, x25, [sp, #-16]!   // Save x24 and x25
    stp     x22, x23, [sp, #-16]!   // Save x22 and x23
    stp     x20, x21, [sp, #-16]!   // Save x20 and x21
    stp     x18, x19, [sp, #-16]!   // Save x18 and x19
    stp     x16, x17, [sp, #-16]!   // Save x16 and x17
    stp     x14, x15, [sp, #-16]!   // Save x14 and x15
    stp     x12, x13, [sp, #-16]!   // Save x12 and x13
    stp     x10, x11, [sp, #-16]!   // Save x10 and x11
    stp     x8, x9, [sp, #-16]!     // Save x8 and x9
    stp     x6, x7, [sp, #-16]!     // Save x6 and x7
    stp     x4, x5, [sp, #-16]!     // Save x4 and x5
    stp     x2, x3, [sp, #-16]!     // Save x2 and x3
    stp     x0, x1, [sp, #-16]!     // Save x0 and x1
.endm

/*
 * Restore the register state from the stack.
 */
.macro exception_exit
    // Restore registers from stack (from x0 to x30)
    ldp     x0, x1, [sp], #16       // Restore x0 and x1
    ldp     x2, x3, [sp], #16       // Restore x2 and x3
    ldp     x4, x5, [sp], #16       // Restore x4 and x5
    ldp     x6, x7, [sp], #16       // Restore x6 and x7
    ldp     x8, x9, [sp], #16       // Restore x8 and x9
    ldp     x10, x11, [sp], #16     // Restore x10 and x11
    ldp     x12, x13, [sp], #16     // Restore x12 and x13
    ldp     x14, x15, [sp], #16     // Restore x14 and x15
    ldp     x16, x17, [sp], #16     // Restore x16 and x17
    ldp     x18, x19, [sp], #16     // Restore x18 and x19
    ldp     x20, x21, [sp], #16
    ldp     x22, x23, [sp], #16
    ldp     x24, x25, [sp], #16
    ldp     x26, x27, [sp], #16
    ldp     x28, x29, [sp], #16
    ldr     x30, [sp], #8
.endm

#endif

#ifndef _SYSREGS_H_
#define _SYSREGS_H_
// ARMv8a_arm.pdf

// ***************************************
// SCTLR_EL1, System Control Register (EL1), Page 2654 of AArch64-Reference-Manual.
// https://developer.arm.com/documentation/ddi0500/j/System-Control/AArch64-register-descriptions/System-Control-Register--EL1
// ***************************************

#define SCTLR_RESERVED                  (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define SCTLR_EE_LITTLE_ENDIAN          (0 << 25)   // Exception endianness
#define SCTLR_EOE_LITTLE_ENDIAN         (0 << 24)   // Endianness of explicit data access at EL0.
#define SCTLR_I_CACHE_DISABLED          (0 << 12)   // Instruction cache enable.
#define SCTLR_D_CACHE_DISABLED          (0 << 2)    // Data and unified caches enable
#define SCTLR_MMU_DISABLED              (0 << 0)    // Bit 0 : MMU enable/disable.
#define SCTLR_MMU_ENABLED               (1 << 0)

// Exception Endianness: Lower Endian, Instruction/Data Cache Disabled, MMU Disabled.
#define SCTLR_VALUE_MMU_DISABLED        (SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)

// ***************************************
// HCR_EL2, Hypervisor Configuration Register (EL2), Page 2487 of AArch64-Reference-Manual.
// https://developer.arm.com/documentation/ddi0595/2021-06/AArch64-Registers/HCR-EL2--Hypervisor-Configuration-Register
// ***************************************

#define HCR_RW                          (1 << 31)   // 0:aarch32, 1:aarch64, Execution state control for lower Exception levels:

// Execution state for lower ELs is aarch64
#define HCR_VALUE                       HCR_RW

// ***************************************
// SCR_EL3, Secure Configuration Register (EL3), Page 2648 of AArch64-Reference-Manual.
// https://developer.arm.com/documentation/ddi0595/2021-06/AArch64-Registers/SCR-EL3--Secure-Configuration-Register
// ***************************************

#define SCR_RESERVED                    (3 << 4)
#define SCR_RW                          (1 << 10)   // 0:aarch32, 1:next lower EL is aarch64, Execution state control for lower Exception levels.
#define SCR_NS                          (1 << 0)    // 0: EL0,EL1 are in secure state, 1: all EL lower than EL3 are in non secure state, Non-secure bit.

// Execution state for all lower ELs to be aarch64, all ELs lower than EL3 are in non secure state
#define SCR_VALUE                       (SCR_RESERVED | SCR_RW | SCR_NS)

// ***************************************
// SPSR_EL3, Saved Program Status Register (EL3) Page 389 of AArch64-Reference-Manual.
// https://developer.arm.com/documentation/ddi0595/2021-03/AArch64-Registers/SPSR-EL3--Saved-Program-Status-Register--EL3-
// ***************************************

// D, bit [9] Debug exception mask.
// A, bit [8] SError interrupt mask.
// I, bit [7] IRQ interrupt mask.
// F, bit [6] FIQ interrupt mask.
#define SPSR_MASK_ALL                   (0xf << 6)
#define SPSR_EL1h                       (5 << 0)    // AArch64 Exception level and selected Stack Pointer.

// Mask Debug Exception, SError Interrupt, IRQ interrupt, FIQ interrupt
#define SPSR_VALUE                      (SPSR_MASK_ALL | SPSR_EL1h)

// ***************************************
// ESR_EL1, Exception Syndrome Register (EL1). Page 1899 of AArch64-Reference-Manual.
// https://developer.arm.com/documentation/100616/0301/register-descriptions/aarch64-system-registers/esr-el1--exception-syndrome-register--el1
// ***************************************

// EC, [31:26] Exception Class. Indicates the reason for the exception that this register holds information about.
// ISS, [24:0] Syndrome information.
#define ESR_ELx_EC_SHIFT                26
#define ESR_ELx_EC_SVC64                0x15
#define ESR_ELx_EC_DABT_LOW             0x24

// ***************************************
// CPACR_EL1, Architectural Feature Access Control Register (EL1), Page 2411 of AArch64-Reference-Manual.
// https://developer.arm.com/documentation/100616/0301/register-descriptions/aarch64-system-registers/cpacr-el1--architectural-feature-access-control-register--el1
// ***************************************

#define CPACR_FPEN_EN                   (0x3 << 20) // Floating Point enable

#endif // _SYSREGS_H_

#ifndef _ASM_UTILS_H

#define _ASM_UTILS_H

#ifdef ARM64
#include<arm/sysregs.h>

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
	ldr	\tmp, =(SCTLR_VALUE_MMU_DISABLED)
	msr	sctlr_el1, \tmp

	ldr	\tmp, =HCR_VALUE
	msr	hcr_el2, \tmp

	ldr	\tmp, =SCR_VALUE
	msr	scr_el3, \tmp

	ldr	\tmp, =SPSR_VALUE
	msr	spsr_el3, \tmp
	
	adr	\tmp, ep
	msr	elr_el3, \tmp 
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
	ldr	\tmp, =(SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)
	msr	sctlr_el1, \tmp

	ldr	\tmp, =(HCR_RW)
	msr	hcr_el2, \tmp
	
	ldr	\tmp, =(CPACR_FPEN_EN)
	msr	cpacr_el1, \tmp

	ldr	\tmp, =(SPSR_MASK_ALL | SPSR_EL1h)
	msr	spsr_el2, \tmp
	
	adr	\tmp, \ep
	msr	elr_el2, \tmp
	eret 
.endm

#endif  //ARM64

#endif //_ASM_UTILS_H

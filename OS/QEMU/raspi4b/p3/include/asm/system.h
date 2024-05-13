#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <raspi4_types.h>
#include <asm/utils.h>

static inline void set_ttbr_tcr_mair(u64 table, u64 tcr, u64 attr)
{
        int el = current_el();
        asm volatile("dsb sy");
        if (el == 1) {
                asm volatile("msr ttbr0_el1, %0" : : "r" (table) : "memory");
                asm volatile("msr tcr_el1, %0" : : "r" (tcr) : "memory");
                asm volatile("msr mair_el1, %0" : : "r" (attr) : "memory");
        } else if (el == 2) {
                asm volatile("msr ttbr0_el2, %0" : : "r" (table) : "memory");
                asm volatile("msr tcr_el2, %0" : : "r" (tcr) : "memory");
                asm volatile("msr mair_el2, %0" : : "r" (attr) : "memory");
        } else if (el == 3) {
                asm volatile("msr ttbr0_el3, %0" : : "r" (table) : "memory");
                asm volatile("msr tcr_el3, %0" : : "r" (tcr) : "memory");
                asm volatile("msr mair_el3, %0" : : "r" (attr) : "memory");
        } else {
                hang();
        }
        asm volatile("isb");
}

static inline void set_sctlr(unsigned long val)
{
        unsigned int el;

        el = current_el();
        if (el == 1)
                asm volatile("msr sctlr_el1, %0" : : "r" (val) : "cc");
        else if (el == 2)
                asm volatile("msr sctlr_el2, %0" : : "r" (val) : "cc");
        else
                asm volatile("msr sctlr_el3, %0" : : "r" (val) : "cc");

        asm volatile("isb");
}

static inline unsigned int get_sctlr(void)
{
        unsigned int el;
        unsigned long val;

        el = current_el();
        if (el == 1)
                asm volatile("mrs %0, sctlr_el1" : "=r" (val) : : "cc");
        else if (el == 2)
                asm volatile("mrs %0, sctlr_el2" : "=r" (val) : : "cc");
        else
                asm volatile("mrs %0, sctlr_el3" : "=r" (val) : : "cc");

        return val;
}



/**
 * __asm_invalidate_dcache_range() - Invalidate a range of virtual addresses
 *
 * This performance an invalidate from @start to @end - 1. Both addresses
 * should be cache-aligned, otherwise this function will align the start
 * address and may continue past the end address.
 *
 * Data in the address range is evicted from the cache and is not written back
 * to memory.
 *
 * @start: Start address to invalidate
 * @end: End address to invalidate up to (exclusive)
 */
void __asm_invalidate_dcache_range(u64 start, u64 end);
void __asm_invalidate_tlb_all(void);
void __asm_invalidate_icache_all(void);
int __asm_invalidate_l3_dcache(void);
int __asm_flush_l3_dcache(void);
int __asm_invalidate_l3_icache(void);
void __asm_switch_ttbr(u64 new_ttbr);
#endif

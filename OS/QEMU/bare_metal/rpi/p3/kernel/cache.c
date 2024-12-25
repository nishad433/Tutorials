#include<mm.h>
#include<printk.h>
#include <asm/system.h>

#define debug printk

#if 0
void invalidate_icache_all(void)
{
        __asm_invalidate_icache_all();
        __asm_invalidate_l3_icache();
}

void icache_enable(void)
{
        invalidate_icache_all();
        set_sctlr(get_sctlr() | CR_I);
}

void icache_disable(void)
{
        set_sctlr(get_sctlr() & ~CR_I);
}

void dcache_enable(void)
{
        /* The data cache is not active unless the mmu is enabled */
        if (!(get_sctlr() & CR_M)) {
                invalidate_dcache_all();
                __asm_invalidate_tlb_all();
                mmu_setup();
        }

        /* Set up page tables only once (it is done also by mmu_setup()) */
        if (!gd->arch.tlb_fillptr)
                setup_all_pgtables();

        set_sctlr(get_sctlr() | CR_C);
}

/*
 * Performs a clean & invalidation of the entire data cache at all levels.
 * This function needs to be inline to avoid using stack.
 * __asm_flush_l3_dcache return status of timeout
 */
inline void flush_dcache_all(void)
{
        int ret;

        __asm_flush_dcache_all();
        ret = __asm_flush_l3_dcache();
        if (ret)
                debug("flushing dcache returns 0x%x\n", ret);
        else
                debug("flushing dcache successfully.\n");
}


void dcache_disable(void)
{
        uint32_t sctlr;

        sctlr = get_sctlr();

        /* if cache isn't enabled no need to disable */
        if (!(sctlr & CR_C))
                return;

#if 0
        if (IS_ENABLED(CONFIG_CMO_BY_VA_ONLY)) {
                /*
                 * When invalidating by VA, do it *before* turning the MMU
                 * off, so that at least our stack is coherent.
                 */
                flush_dcache_all();
        }
#endif

        set_sctlr(sctlr & ~(CR_C|CR_M));

#if 0
        if (!IS_ENABLED(CONFIG_CMO_BY_VA_ONLY))
                flush_dcache_all();
#endif
        __asm_invalidate_tlb_all();
}

void enable_caches(void)
{
        icache_enable();
        dcache_enable();
}

#endif

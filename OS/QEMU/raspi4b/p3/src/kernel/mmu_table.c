#include<stdio.h>
#include<stdint.h>
#include <stdlib.h>
#include <base.h>
#include<mm.h>

static struct mm_region raspi4b_mem_map[] = {
        {
                .virt = DDR_START_VA,
                .phys = DDR_START_PA,
                .size = DDR_SIZE,
                .attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
                        PTE_BLOCK_INNER_SHARE
        },
        {
                .virt = MAIN_PERIPH_BASE_VA,
                .phys = MAIN_PERIPH_BASE_PA,
                .size = MAIN_PERIPH_SIZE,
                .attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
                         PTE_BLOCK_NON_SHARE |
                         PTE_BLOCK_PXN | PTE_BLOCK_UXN
        },
        {
                .virt = ARM_LOCAL_PERIPH_BASE_VA,
                .phys = ARM_LOCAL_PERIPH_BASE_PA,
                .size = ARM_LOCAL_PERIPH_SIZE,
                .attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
                         PTE_BLOCK_NON_SHARE |
                         PTE_BLOCK_PXN | PTE_BLOCK_UXN
        },
        {
                /* List terminator */
                0,
        }
};

struct mm_region *mem_map = raspi4b_mem_map;

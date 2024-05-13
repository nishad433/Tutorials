#include<stdio.h>
#include<stdint.h>
#include <stdlib.h>
#include<mm.h>



static struct mm_region raspi4b_mem_map[] = {
        {
                .virt = 0x0UL,
                .phys = 0x0UL,
                .size = 100UL * SZ_1M,
                .attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
                        PTE_BLOCK_INNER_SHARE
        },
        {
                .virt = 0x0FC000000UL,
                .phys = 0x0FC000000UL,
                .size = 64UL * SZ_1M,
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

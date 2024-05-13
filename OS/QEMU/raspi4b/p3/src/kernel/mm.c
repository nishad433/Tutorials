#include <mm.h>
#include <printk.h>
#include <asm/system.h>
#include <asm/sysregs.h>

extern uint64_t id_pg_dir;

struct pgtbl{
    u64 tlb_addr;
    u64 tlb_size;
    u64 tlb_fillptr;
};

struct pgtbl ptbl={
        .tlb_addr = 0,
        .tlb_size = 0,
        .tlb_fillptr = 0
};

static int pte_type(u64 *pte)
{
        return *pte & PTE_TYPE_MASK;
}

/* Returns the LSB number for a PTE on level <level> */
static int level2shift(int level)
{
        /* Page is 12 bits wide, every level translates 9 bits */
        return (12 + 9 * (3 - level));
}

/* Returns and creates a new full table (512 entries) */
static u64 *create_table(void)
{
        u64 *new_table = (u64*)ptbl.tlb_fillptr;
        u64 pt_len = MAX_PTE_ENTRIES * sizeof(u64);

        /* Allocate MAX_PTE_ENTRIES pte entries */
        ptbl.tlb_fillptr += pt_len;

        if (ptbl.tlb_fillptr - ptbl.tlb_addr > ptbl.tlb_size)
                printk("Insufficient RAM for page table: 0x%lx > 0x%lx. "
                      "Please increase the size in get_page_table_size()",
                        ptbl.tlb_fillptr - ptbl.tlb_addr,
                        ptbl.tlb_size);

        /* Mark all entries as invalid */
        memset(new_table, 0, pt_len);

        return new_table;
}

u64 get_tcr(u64 *pips, u64 *pva_bits)
{
        int el = current_el();
        u64 max_addr = 0;
        u64 ips, va_bits;
        u64 tcr;
        int i;

        /* Find the largest address we need to support */
        for (i = 0; mem_map[i].size || mem_map[i].attrs; i++)
                max_addr = max(max_addr, mem_map[i].virt + mem_map[i].size);

        /* Calculate the maximum physical (and thus virtual) address */
        if (max_addr > (1ULL << 44)) {
                ips = 5;
                va_bits = 48;
        } else  if (max_addr > (1ULL << 42)) {
                ips = 4;
                va_bits = 44;
        } else  if (max_addr > (1ULL << 40)) {
                ips = 3;
                va_bits = 42;
        } else  if (max_addr > (1ULL << 36)) {
                ips = 2;
                va_bits = 40;
        } else  if (max_addr > (1ULL << 32)) {
                ips = 1;
                va_bits = 36;
        } else {
                ips = 0;
                va_bits = 32;
        }

        if (el == 1) {
                tcr = TCR_EL1_RSVD | (ips << 32) | TCR_EPD1_DISABLE;
        } else if (el == 2) {
                tcr = TCR_EL2_RSVD | (ips << 16);
        } else {
                tcr = TCR_EL3_RSVD | (ips << 16);
        }

        /* PTWs cacheable, inner/outer WBWA and inner shareable */
        tcr |= TCR_TG0_4K | TCR_SHARED_INNER | TCR_ORGN_WBWA | TCR_IRGN_WBWA;
        tcr |= TCR_T0SZ(va_bits);

        if (pips)
                *pips = ips;
        if (pva_bits)
                *pva_bits = va_bits;

        return tcr;
}

static void set_pte_table(u64 *pte, u64 *table)
{
        /* Point *pte to the new table */
        //printk("Setting %p to addr=%p\n", pte, table);
        *pte = PTE_TYPE_TABLE | (u64)table;
}


static void map_range(u64 virt, u64 phys, u64 size, int level,
                      u64 *table, u64 attrs)
{
        u64 map_size = BIT_ULL(level2shift(level));
        int i, idx;

        idx = (virt >> level2shift(level)) & (MAX_PTE_ENTRIES - 1);
        for (i = idx; size; i++) {
                u64 next_size, *next_table;

                if (level >= 1 &&
                    size >= map_size && !(virt & (map_size - 1))) {
                        if (level == 3)
                                table[i] = phys | attrs | PTE_TYPE_PAGE;
                        else
                                table[i] = phys | attrs;

                        virt += map_size;
                        phys += map_size;
                        size -= map_size;

                        continue;
                }

                /* Going one level down */
                if (pte_type(&table[i]) == PTE_TYPE_FAULT)
                        set_pte_table(&table[i], create_table());

                next_table = (u64 *)(table[i] & GENMASK_ULL(47, PAGE_SHIFT));
                next_size = min(map_size - (virt & (map_size - 1)), size);

                map_range(virt, phys, next_size, level + 1, next_table, attrs);

                virt += next_size;
                phys += next_size;
                size -= next_size;
        }
}

static void add_map(struct mm_region *map)
{
        u64 attrs = map->attrs | PTE_TYPE_BLOCK | PTE_BLOCK_AF;
        u64 va_bits;
        int level = 0;

        get_tcr(NULL, &va_bits);
        if (va_bits < 39)
                level = 1;

        map_range(map->virt, map->phys, map->size, level,
                  (u64 *)ptbl.tlb_addr, attrs);
}

void setup_pgtables(void)
{
        int i;

        if (!ptbl.tlb_fillptr || !ptbl.tlb_addr)
                printk("Page table pointer not setup.");

        /*
         * Allocate the first level we're on with invalidate entries.
         * If the starting level is 0 (va_bits >= 39), then this is our
         * Lv0 page table, otherwise it's the entry Lv1 page table.
         */
        create_table();

        /* Now add all MMU table entries one after another to the table */
        for (i = 0; mem_map[i].size || mem_map[i].attrs; i++){
                add_map(&mem_map[i]);
        }
}

void mm_init(void){

    ptbl.tlb_addr = (u64)&id_pg_dir;
    ptbl.tlb_size = 20 * SZ_4K;
    ptbl.tlb_fillptr = ptbl.tlb_addr;

    setup_pgtables();
    set_ttbr_tcr_mair(ptbl.tlb_addr, get_tcr(NULL, NULL),
                          MEMORY_ATTRIBUTES);

    /* enable the mmu */
    set_sctlr(get_sctlr() | CR_M);
    printk("MMU Enabled\n");
}

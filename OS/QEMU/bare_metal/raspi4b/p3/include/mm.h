#ifndef _MM_H_
#define _MM_H_

#include <raspi4_types.h>
#include <sizes.h>
#include <utils.h>


/*

                           Virtual address                                                                 Physical Memory
+-----------------------------------------------------------------------+                                +------------------+
|         | PGD Index | PUD Index | PMD Index | PTE Index | Page offset |                                |                  |
+-----------------------------------------------------------------------+                                |                  |
63        47     |    38      |   29     |    20    |     11      |     0                                |     Page N       |
                 |            |          |          |             +--------------------+           +---->+------------------+
                 |            |          |          +---------------------+            |           |     |                  |
          +------+            |          |                                |            |           |     |                  |
          |                   |          +----------+                     |            |           |     |------------------|
+------+  |        PGD        |                     |                     |            +---------------->| Physical address |
| ttbr |---->+-------------+  |           PUD       |                     |                        |     |------------------|
+------+  |  |             |  | +->+-------------+  |          PMD        |                        |     |                  |
          |  +-------------+  | |  |             |  | +->+-------------+  |          PTE           |     +------------------+
          +->| PUD address |----+  +-------------+  | |  |             |  | +->+--------------+    |     |                  |
             +-------------+  +--->| PMD address |----+  +-------------+  | |  |              |    |     |                  |
             |             |       +-------------+  +--->| PTE address |----+  +-------------_+    |     |                  |
             +-------------+       |             |       +-------------+  +--->| Page address |----+     |                  |
                                   +-------------+       |             |       +--------------+          |                  |
                                                         +-------------+       |              |          |                  |
                                                                             +--------------+          +------------------+


Section Mapping

                           Virtual address                                               Physical Memory
+-----------------------------------------------------------------------+              +------------------+
|         | PGD Index | PUD Index | PMD Index |      Section offset     |              |                  |
+-----------------------------------------------------------------------+              |                  |
63        47     |    38      |   29     |    20            |           0              |    Section N     |
                 |            |          |                  |                    +---->+------------------+
                 |            |          |                  |                    |     |                  |
          +------+            |          |                  |                    |     |                  |
          |                   |          +----------+       |                    |     |------------------|
+------+  |        PGD        |                     |       +------------------------->| Physical address |
| ttbr |---->+-------------+  |           PUD       |                            |     |------------------|
+------+  |  |             |  | +->+-------------+  |            PMD             |     |                  |
          |  +-------------+  | |  |             |  | +->+-----------------+     |     +------------------+
          +->| PUD address |----+  +-------------+  | |  |                 |     |     |                  |
             +-------------+  +--->| PMD address |----+  +-----------------+     |     |                  |
             |             |       +-------------+  +--->| Section address |-----+     |                  |
             +-------------+       |             |       +-----------------+           |                  |
                                   +-------------+       |                 |           |                  |
                                                         +-----------------+           |                  |
                                                                                       +------------------+


Page descriptor format

                           Descriptor format
`+------------------------------------------------------------------------------------------+
 | Upper attributes | Address (bits 47:12) | Lower attributes | Block/table bit | Valid bit |
 +------------------------------------------------------------------------------------------+
 63                 47                     11                 2                 1           0

Bit 0 This bit must be set to 1 for all valid descriptors. If MMU encounter non-valid descriptor during translation process a synchronous exception is generated. The kernel then should handle this exception, allocate a new page and prepare a correct descriptor (We will look in details on how this works a little bit later)

Bit 1 This bit indicates whether the current descriptor points to a next page table in the hierarchy (we call such descriptor a "table descriptor") or it points instead to a physical page or a section (such descriptors are called "block descriptors").

Bits [11:2] Those bits are ignored for table descriptors. For block descriptors they contain some attributes that control, for example, whether the mapped page is cachable, executable, etc.

Bits [47:12]. This is the place where the address that a descriptor points to is stored. As I mentioned previously, only bits [47:12] of the address need to be stored, because all other bits are always 0.

Bits [63:48] Another set of attributes.

Block/Page Descriptors:

Lower Attributes [11:2]
+------------------------------------------------------------------------------+
|           | AF |          |          |          | Perms | NS | AttrIndx[2:0] |
+------------------------------------------------------------------------------+
12          11   10         9          8          7       6    5               2
Bits [4:2]  AttrIndx[2:0]
Bits [6]  0x1
Bits [10] Access Flag 0x1

Upper Attributes [63:47]

*/

/*
 * Memory region attributes
 * 0b0000dd00                                           Device memory. See encoding of 'dd' for the type of Device memory.
 * 0booooiiii, (oooo != 0000 and iiii != 0000)          Normal memory. See encoding of 'oooo' and 'iiii' for the type of Normal Memory.
 *
 * 'dd' is encoded as follows:
 *  dd          Meaning
 * 0b00         Device-nGnRnE memory
 * 0b01         Device-nGnRE memory
 * 0b10         Device-nGRE memory
 * 0b11         Device-GRE memory
 *
 * 'oooo' is encoded as follows:
 * 'oooo'                       Meaning
 * 0b0000                       See encoding of Attr
 * 0b00RW, RW not 0b00          Normal memory, Outer Write-Through Transient
 * 0b0100                       Normal memory, Outer Non-cacheable
 * 0b01RW, RW not 0b00          Normal memory, Outer Write-Back Transient
 * 0b10RW                       Normal memory, Outer Write-Through Non-transient
 * 0b11RW                       Normal memory, Outer Write-Back Non-transient
 *
 * R = Outer Read-Allocate policy, W = Outer Write-Allocate policy.
 *
 * 'iiii' is encoded as follows:
 * 'iiii'        Meaning
 * 0b0000                       See encoding of Attr
 * 0b00RW, RW not 0b00          Normal memory, Inner Write-Through Transient
 * 0b0100                       Normal memory, Inner Non-cacheable
 * 0b01RW, RW not 0b00          Normal memory, Inner Write-Back Transient
 * 0b10RW                       Normal memory, Inner Write-Through Non-transient
 * 0b11RW                       Normal memory, Inner Write-Back Non-transient
 *
 * R = Inner Read-Allocate policy, W = Inner Write-Allocate policy.
 *
 * The R and W bits in 'oooo' and 'iiii' fields have the following meanings:
 * R or W               Meaning
 *  0b0                 No Allocate
 *  0b1                 Allocate
 *
 *
 *
 *
 * n = AttrIndx[2:0]
 *                        n       MAIR
 * DEVICE_nGnRnE        0x000   0b00000000   0x0        Device-nGnRnE memory
 * NORMAL_NC            0x001   0b01000100   0x44       Normal memory, Outer Non-cacheable  Inner Non-cacheable
 *
 *
*/


/*
 *  With 4k page granule, a virtual address is split into 4 lookup parts
 *  spanning 9 bits each:
 *
 *    _______________________________________________
 *   |       |       |       |       |       |       |
 *   |   0   |  Lv0  |  Lv1  |  Lv2  |  Lv3  |  off  |
 *   |_______|_______|_______|_______|_______|_______|
 *     63-48   47-39   38-30   29-21   20-12   11-00
 *
 *             mask        page size
 *
 *    Lv0: FF8000000000       --
 *    Lv1:   7FC0000000       1G
 *    Lv2:     3FE00000       2M
 *    Lv3:       1FF000       4K
 *    off:          FFF
 */

/* Doing identity Mapping
 * # |    PA Start  |   PA End     | Flags
 * 0 |      0x0     |  0x200000    |
 * 1 |  0x0FC000000 | 0x100000000  |
 *
 *
 * # 0
 * PGD : 0    1 entry
 * PUD : 0    1 entry
 * PMD : 0    1 entry
 *
 * # 1
 * PGD : 0              1 entry
 * PUD : 3               1 entry
 * PMD : 0 - 511        512 entries
 *
 *
 *
 */


/*
 * block/section address mask and size definitions.
 */

/* PAGE_SHIFT determines the page size */
#undef  PAGE_SIZE
#define PAGE_SHIFT              12
#define PAGE_SIZE               (1 << PAGE_SHIFT)       /* 4096 */
#define PAGE_MASK               (~(PAGE_SIZE - 1))

#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)        /* 21 */
#define SECTION_SIZE (1 << SECTION_SHIFT)               /* 2MB */

#define MAX_PTE_ENTRIES 512
#define PGD_SHIFT              (PAGE_SHIFT + 3 * TABLE_SHIFT)   /* 39 */
#define PUD_SHIFT              (PAGE_SHIFT + 2 * TABLE_SHIFT)   /* 30 */
#define PMD_SHIFT              (PAGE_SHIFT + TABLE_SHIFT)       /* 21 */

/***************************************************************/

/*
 * Memory types
 */
#define MT_DEVICE_NGNRNE        0
#define MT_DEVICE_NGNRE         1
#define MT_DEVICE_GRE           2
#define MT_NORMAL_NC            3
#define MT_NORMAL               4

#define MEMORY_ATTRIBUTES       ((0x00 << (MT_DEVICE_NGNRNE * 8)) |     \
                                (0x04 << (MT_DEVICE_NGNRE * 8))   |     \
                                (0x0c << (MT_DEVICE_GRE * 8))     |     \
                                (0x44 << (MT_NORMAL_NC * 8))      |     \
                                (UL(0xff) << (MT_NORMAL * 8)))

/***************************************************************/

/*
 * Hardware page table definitions.
 *
 */

#define PTE_TYPE_MASK           (3 << 0)
#define PTE_TYPE_FAULT          (0 << 0)
#define PTE_TYPE_TABLE          (3 << 0)
#define PTE_TYPE_PAGE           (3 << 0)
#define PTE_TYPE_BLOCK          (1 << 0)
#define PTE_TYPE_VALID          (1 << 0)

#define PTE_TABLE_PXN           (1UL << 59)
#define PTE_TABLE_XN            (1UL << 60)
#define PTE_TABLE_AP            (1UL << 61)
#define PTE_TABLE_NS            (1UL << 63)


/***************************************************************/

/*
 * Block
 */
#define PTE_BLOCK_MEMTYPE(x)    ((x) << 2)
#define PTE_BLOCK_NS            (1 << 5)
#define PTE_BLOCK_NON_SHARE     (0 << 8)
#define PTE_BLOCK_OUTER_SHARE   (2 << 8)
#define PTE_BLOCK_INNER_SHARE   (3 << 8)
#define PTE_BLOCK_AF            (1 << 10)
#define PTE_BLOCK_NG            (1 << 11)
#define PTE_BLOCK_PXN           (UL(1) << 53)
#define PTE_BLOCK_UXN           (UL(1) << 54)


/***************************************************************/

/*
 * AttrIndx[2:0]
 */
#define PMD_ATTRINDX(t)         ((t) << 2)
#define PMD_ATTRINDX_MASK       (7 << 2)
#define PMD_ATTRMASK            (PTE_BLOCK_PXN          | \
                                 PTE_BLOCK_UXN          | \
                                 PMD_ATTRINDX_MASK      | \
                                 PTE_TYPE_VALID)

/***************************************************************/

/*
 * TCR flags.
 */
#define TCR_T0SZ(x)             ((64 - (x)) << 0)
#define TCR_IRGN_NC             (0 << 8)
#define TCR_IRGN_WBWA           (1 << 8)
#define TCR_IRGN_WT             (2 << 8)
#define TCR_IRGN_WBNWA          (3 << 8)
#define TCR_IRGN_MASK           (3 << 8)
#define TCR_ORGN_NC             (0 << 10)
#define TCR_ORGN_WBWA           (1 << 10)
#define TCR_ORGN_WT             (2 << 10)
#define TCR_ORGN_WBNWA          (3 << 10)
#define TCR_ORGN_MASK           (3 << 10)
#define TCR_SHARED_NON          (0 << 12)
#define TCR_SHARED_OUTER        (2 << 12)
#define TCR_SHARED_INNER        (3 << 12)
#define TCR_TG0_4K              (0 << 14)
#define TCR_TG0_64K             (1 << 14)
#define TCR_TG0_16K             (2 << 14)
#define TCR_EPD1_DISABLE        (1 << 23)

#define TCR_EL1_RSVD            (1U << 31)
#define TCR_EL2_RSVD            (1U << 31 | 1 << 23)
#define TCR_EL3_RSVD            (1U << 31 | 1 << 23)

#define HCR_EL2_E2H_BIT         34

/***************************************************************/

struct mm_region {
        u64 virt;
        u64 phys;
        u64 size;
        u64 attrs;
};

extern struct mm_region *mem_map;

/***************************************************************/

void mm_init(void);

#endif //_MM_H_

#include <asm/macros.h>

.section .text

/*
 * void __asm_invalidate_tlb_all(void)
 *
 * invalidate all tlb entries.
*/
ENTRY(__asm_invalidate_tlb_all)
        switch_el x9, 3f, 2f, 1f
3:      tlbi    alle3
        dsb     sy
        isb
        b       0f
2:      tlbi    alle2
        dsb     sy
        isb
        b       0f
1:      tlbi    vmalle1
        dsb     sy
        isb
0:
        ret
ENDPROC(__asm_invalidate_tlb_all)


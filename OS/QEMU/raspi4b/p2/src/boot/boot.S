#include<asm/macros.h>

.section .text.boot
.global _start

_start:
        switch_primary x0, primary, secondary

.section .text
secondary:
        b .
primary:
        switch_el       x0, 3f, 2f, 1f
3:      /* in EL3 */
        switch_from_el3_to_el1  el1_entry x1
2:      /* in EL2 */
        switch_from_el2_to_el1  el1_entry x1
1:      /* in EL1 */
        b       el1_entry

el1_entry:
        b .

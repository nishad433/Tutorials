#include<asm/macros.h>

.section .text.boot
.global _start

_start:
        switch_primary x0, primary, secondary

.section .text
secondary:
        b .
primary:
        switch_el       x0, 3f, 2f, el1_entry
3:      /* in EL3 */
        switch_from_el3_to_el1  el1_entry x1
2:      /* in EL2 */
        switch_from_el2_to_el1  el1_entry x1

        /* in EL1 */
el1_entry:
        b .

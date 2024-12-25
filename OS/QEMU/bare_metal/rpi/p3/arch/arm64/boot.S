#include<asm/macros.h>

.section .text.boot

ENTRY(_start)
        switch_primary x0, primary, secondary
secondary:
        b hang
primary:
        switch_el       x0, 3f, 2f, el1_entry
3:      /* in EL3 */
        switch_from_el3_to_el1  el1_entry x1
2:      /* in EL2 */
        switch_from_el2_to_el1  el1_entry x1

        /* in EL1 */
el1_entry:
        // setup stack
        //ldr x0, =_stack_start
        adrp x0, _stack_start
        mov sp, x0
        // setup bss and move data sections.
        bl setup_c_environment
        // Call main routine
        bl main
ENDPROC(_start)

ENTRY(hang)
    /* wait for event */
    wfe
    b hang
ENDPROC(hang)

ENTRY(current_el)
    mrs x0, CurrentEL;
    lsr x0, x0, #2
    and x0, x0, #3
    ret;
ENDPROC(current_el)

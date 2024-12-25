#include <asm/macros.h>

.section .text

ENTRY(get_sys_count)
    mrs x0, CNTPCT_EL0
    ret
ENDPROC(get_sys_count)

ENTRY(set_CNTP_TVAL)
    msr CNTP_TVAL_EL0, x0
    ret
ENDPROC(set_CNTP_TVAL)

ENTRY(setup_CNTP_CTL)
    mov x9, 1
    msr CNTP_CTL_EL0, x9
    ret
ENDPROC(setup_CNTP_CTL)

ENTRY(get_sys_frequency)
    mrs x0, CNTFRQ_EL0
    ret
ENDPROC(get_sys_frequency)

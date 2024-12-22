#ifndef _ARM_TIMER_H_
#define _ARM_TIMER_H_


#include<platform.h>
#include<stdint.h>

typedef struct {
    rw_REG32 ctrl;			// 0x00
    ro_REG32 reserverd0;		// 0x04
    rw_REG32 core_timer_prescaler;	// 0x08
    ro_REG32 reserverd1[4];		// 0x0c
    rw_REG32 core_timer_access_ls32;	// 0x1c
    rw_REG32 core_timer_access_ms32;	// 0x20
    ro_REG32 reserverd2[4];		// 0x24
    rw_REG32 local_timer_ctrl_status;	// 0x34
    rw_REG32 local_timer_write_flags;	// 0x38
    ro_REG32 reserverd3;		// 0x3c
    rw_REG32 timer_intr_ctrl_0;		// 0x40
    rw_REG32 timer_intr_ctrl_1;		// 0x44
    rw_REG32 timer_intr_ctrl_2;		// 0x48
    rw_REG32 timer_intr_ctrl_3;		// 0x4c
}armtimer_regs_t;

//#define ARM_TIMER_REGS   ((volatile armtimer_regs_t *)ARM_TIMER_BASE)

void arm_timer_init(void);

#endif

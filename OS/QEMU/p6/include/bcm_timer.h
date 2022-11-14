#ifndef _BCM_TIMER_H_
#define _BCM_TIMER_H_


#include<platform.h>
#include<stdint.h>

typedef struct {
    rw_REG32 cs;	// 0x00
    rw_REG32 clo;	// 0x04
    rw_REG32 chi;	// 0x08
    rw_REG32 c0;	// 0x0c
    rw_REG32 c1;	// 0x10
    rw_REG32 c2;	// 0x14
    rw_REG32 c3;	// 0x18
}bcmtimer_regs_t;

#define BCM_TIMER_REGS   ((volatile bcmtimer_regs_t *)BCM_TIMER_BASE)

void bcm_timer_init(void);

#endif

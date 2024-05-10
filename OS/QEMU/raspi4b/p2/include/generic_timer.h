#ifndef GENERIC_TIMER_H
#define GENERIC_TIMER_H

#include<raspi4_types.h>

#define SYS_FREQ 54000000

uint64_t get_sys_count(void);
void set_CNTP_TVAL(uint32_t);
void setup_CNTP_CTL(void);
void generic_timer_init(void);
void handle_generic_timer(void);

#endif /* GENERIC_TIMER_H */

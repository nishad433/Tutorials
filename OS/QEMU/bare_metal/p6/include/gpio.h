#ifndef _GPIO_H_
#define _GPIO_H_

#include<platform.h>
#include<stdint.h>

#ifdef GPIO_SUPPORT

#define GPIO_NUM_MAX (54)

#define GPIO_TO_FSEL_ARRAY_IDX(X) ((X<GPIO_NUM_MAX)?(X/10):0)
#define GPIO_TO_FSEL_ARRAY_SHIFT(X) ((X<GPIO_NUM_MAX)?((X%10)*3):0)

#define GPIO_TO_IDX(X) ((X<GPIO_NUM_MAX)?(X/32):0)
#define GPIO_TO_SHIFT(X) ((X<GPIO_NUM_MAX)?(X%32):0)

#define GPIO_14	14
#define GPIO_15	14

typedef enum {
	gpio_fn_input  = 0x0,
	gpio_fn_output = 0x1,
	gpio_fn_altfn0 = 0x4,
	gpio_fn_altfn1 = 0x5,
	gpio_fn_altfn2 = 0x6,
	gpio_fn_altfn3 = 0x7,
	gpio_fn_altfn4 = 0x3,
	gpio_fn_altfn5 = 0x2
}gpio_fn_t;

typedef struct {
    rw_REG32 gpfsel[6];
    ro_REG32 reserved1;
    rw_REG32 gpset[2];
    ro_REG32 reserved2;
    rw_REG32 gpclr[2];
    ro_REG32 reserved3;
    rw_REG32 gplev[2];
    ro_REG32 reserved4;
    rw_REG32 gpeds[2];
    ro_REG32 reserved5;
    rw_REG32 gpren[2];
    ro_REG32 reserved6;
    rw_REG32 gpfen[2];
    ro_REG32 reserved7;
    rw_REG32 gphen[2];
    ro_REG32 reserved8;
    rw_REG32 gplen[2];
    ro_REG32 reserved9;
    rw_REG32 gparen[2];
    ro_REG32 reserved10;
    rw_REG32 gpafen[2];
    ro_REG32 reserved11;
    rw_REG32 gppud;
    rw_REG32 gppudclk[2];
}gpio_regs_t;


#define GPIO_REGS   ((volatile gpio_regs_t *)GPIO_BASE)

int gpio_init(void);
void gpio_set_fn(int gpio, gpio_fn_t fn);
void gpio_direction_output(int gpio);
void gpio_direction_input(int gpio);
int gpio_get_value(int gpio);
void gpio_set_value(int gpio, int val);

#endif //GPIO_SUPPORT

#endif //_GPIO_H_

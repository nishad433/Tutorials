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
    uint32_t gpfsel[6];
    uint32_t reserved1;
    uint32_t gpset[2];
    uint32_t reserved2;
    uint32_t gpclr[2];
    uint32_t reserved3;
    uint32_t gplev[2];
    uint32_t reserved4;
    uint32_t gpeds[2];
    uint32_t reserved5;
    uint32_t gpren[2];
    uint32_t reserved6;
    uint32_t gpfen[2];
    uint32_t reserved7;
    uint32_t gphen[2];
    uint32_t reserved8;
    uint32_t gplen[2];
    uint32_t reserved9;
    uint32_t gparen[2];
    uint32_t reserved10;
    uint32_t gpafen[2];
    uint32_t reserved11;
    uint32_t gppud;
    uint32_t gppudclk[2];
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

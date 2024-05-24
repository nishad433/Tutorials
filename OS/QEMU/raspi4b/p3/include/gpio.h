#ifndef _GPIO_H_
#define _GPIO_H_

#include <base.h>
#include <raspi4_types.h>

#define GPIO_NUM_MAX 58

typedef enum {
  gpio_input = 0x0,
  gpio_output = 0x1,
  gpio_alt_func0 = 0x4,
  gpio_alt_func1 = 0x5,
  gpio_alt_func2 = 0x6,
  gpio_alt_func3 = 0x7,
  gpio_alt_func4 = 0x3,
  gpio_alt_func5 = 0x2,
} gpio_func_t;

typedef enum {
  gpio_no_resistor = 0, // No resistor selected
  gpio_pull_high = 1,   // Pull up resistor selected
  gpio_pull_low = 2,    // Pull down resistor selected
} gpio_pud_pdn_resistor_t;

typedef struct {
  rw_REG32 GPFSEL[6]; // [0x00] GPIO Function Select 0-5
  ro_REG32 reserved0;
  rw_REG32 GPSET[2]; // [0x1c] GPIO Pin Output Set 0-1
  ro_REG32 reserved1;
  rw_REG32 GPCLR[2]; // [0x28] GPIO Pin Output Clear 0-1
  ro_REG32 reserved2;
  rw_REG32 GPLEV[2]; // [0x34] GPIO Pin Level 0-1
  ro_REG32 reserved3;
  rw_REG32 GPEDS[2]; // [0x40] GPIO Pin Event Detect Status 0-1
  ro_REG32 reserved4;
  rw_REG32 GPREN[2]; // [0x4c] GPIO Pin Rising Edge Detect Enable 0-1
  ro_REG32 reserved5;
  rw_REG32 GPFEN[2]; // [0x58] GPIO Pin Falling Edge Detect Enable 0-1
  ro_REG32 reserved6;
  rw_REG32 GPHEN[2]; // [0x64] GPIO Pin High Detect Enable 0-1
  ro_REG32 reserved7;
  rw_REG32 GPLEN[2]; // [0x70] GPIO Pin Low Detect Enable 0-1
  ro_REG32 reserved8;
  rw_REG32 GPAREN[2]; // [0x7c] GPIO Pin Async. Rising Edge Detect 0-1
  ro_REG32 reserved9;
  rw_REG32 GPAFEN[2]; // [0x88] GPIO Pin Async. Falling Edge Detect 0-1
  ro_REG32 reserved10[0x15];
  rw_REG32
      GPIO_PUP_PDN_CNTRL_REG[4]; // [0xe4] GPIO Pull-up / Pull-down Register 0-3
} gpio_regs_t;

#define GPIO_BASE (MAIN_PERIPH_BASE_VA + 0x2200000)
// GPIO Register
#define GPIO_REGS ((volatile gpio_regs_t *)GPIO_BASE)

void gpio_set_function(int, gpio_func_t);
void gpio_output_set(int);
void gpio_output_clear(int);
int gpio_get_value(int);
void gpio_set_pup_pdn(int, gpio_pud_pdn_resistor_t);
int gpio_init(void);

#endif // _GPIO_H_

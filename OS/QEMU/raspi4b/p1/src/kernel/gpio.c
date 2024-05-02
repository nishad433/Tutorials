#include <gpio.h>

#define GPIO_TO_IDX(X) (X < GPIO_NUM_MAX ? X / 32 : -1)
#define GPIO_TO_SHIFT(X) (X < GPIO_NUM_MAX ? X % 32 : -1)

typedef struct {
  int gpio;
  char *name;
  gpio_func_t func;
  gpio_pud_pdn_resistor_t gpio_pud_pdn;
} GPIO_t;

const GPIO_t gpio_set[] = {
    {0, "GPIO_0", gpio_output, gpio_pull_high},
    {1, "GPIO_1", gpio_output, gpio_pull_high},
    {2, "GPIO_2", gpio_output, gpio_pull_high},
    {3, "GPIO_3", gpio_output, gpio_pull_high},
    {4, "GPIO_4", gpio_output, gpio_pull_high},
    {5, "GPIO_5", gpio_output, gpio_pull_high},
    {6, "GPIO_6", gpio_output, gpio_pull_high},
    {7, "GPIO_7", gpio_output, gpio_pull_high},
    {8, "GPIO_8", gpio_output, gpio_pull_high},
    {9, "GPIO_9", gpio_output, gpio_pull_low},
    {10, "GPIO_10", gpio_output, gpio_pull_low},
    {11, "GPIO_11", gpio_output, gpio_pull_low},
    {12, "GPIO_12", gpio_output, gpio_pull_low},
    {13, "GPIO_13", gpio_output, gpio_pull_low},
    {14, "GPIO_TXD0", gpio_alt_func0, gpio_pull_low},
    {15, "GPIO_RXD0", gpio_alt_func0, gpio_pull_low},
    {16, "GPIO_16", gpio_output, gpio_pull_low},
    {17, "GPIO_17", gpio_output, gpio_pull_low},
    {18, "GPIO_18", gpio_output, gpio_pull_low},
    {19, "GPIO_19", gpio_output, gpio_pull_low},
    {20, "GPIO_20", gpio_output, gpio_pull_low},
    {21, "GPIO_21", gpio_output, gpio_pull_low},
    {22, "GPIO_22", gpio_output, gpio_pull_low},
    {23, "GPIO_23", gpio_output, gpio_pull_low},
    {24, "GPIO_24", gpio_output, gpio_pull_low},
    {25, "GPIO_25", gpio_output, gpio_pull_low},
    {26, "GPIO_26", gpio_output, gpio_pull_low},
    {27, "GPIO_27", gpio_output, gpio_pull_low},
    {28, "GPIO_28", gpio_output, gpio_no_resistor},
    {29, "GPIO_29", gpio_output, gpio_no_resistor},
    {30, "GPIO_30", gpio_output, gpio_pull_low},
    {31, "GPIO_31", gpio_output, gpio_pull_low},
    {32, "GPIO_32", gpio_output, gpio_pull_low},
    {33, "GPIO_33", gpio_output, gpio_pull_low},
    {34, "GPIO_34", gpio_output, gpio_pull_high},
    {35, "GPIO_35", gpio_output, gpio_pull_high},
    {36, "GPIO_36", gpio_output, gpio_pull_high},
    {37, "GPIO_37", gpio_output, gpio_pull_low},
    {38, "GPIO_38", gpio_output, gpio_pull_low},
    {39, "GPIO_39", gpio_output, gpio_pull_low},
    {40, "GPIO_40", gpio_output, gpio_pull_low},
    {41, "GPIO_41", gpio_output, gpio_pull_low},
    {42, "GPIO_42", gpio_output, gpio_pull_low},
    {43, "GPIO_43", gpio_output, gpio_pull_low},
    {44, "GPIO_44", gpio_output, gpio_no_resistor},
    {45, "GPIO_45", gpio_output, gpio_no_resistor},
    {-1, NULL, -1, -1},
};

void gpio_set_function(int gpio, gpio_func_t func) {
  int array_idx = gpio / 10;
  int bit_shift = (gpio % 10) * 3;
  GPIO_REGS->GPFSEL[array_idx] &= ~((0x7) << bit_shift);
  GPIO_REGS->GPFSEL[array_idx] |= ((func & 0x7) << bit_shift);
}

void gpio_output_set(int gpio) {
  GPIO_REGS->GPSET[GPIO_TO_IDX(gpio)] &= ~(0x1 << GPIO_TO_SHIFT(gpio));
  GPIO_REGS->GPSET[GPIO_TO_IDX(gpio)] |= 0x1 << GPIO_TO_SHIFT(gpio);
}

void gpio_output_clear(int gpio) {
  GPIO_REGS->GPCLR[GPIO_TO_IDX(gpio)] &= ~(0x1 << GPIO_TO_SHIFT(gpio));
  GPIO_REGS->GPCLR[GPIO_TO_IDX(gpio)] |= 0x1 << GPIO_TO_SHIFT(gpio);
}

int gpio_get_value(int gpio) {
  return ((GPIO_REGS->GPLEV[GPIO_TO_IDX(gpio)] >> GPIO_TO_SHIFT(gpio)) & 0x1);
}

void gpio_set_pup_pdn(int gpio, gpio_pud_pdn_resistor_t val) {
  int array_idx = gpio / 16;
  int bit_shift = (gpio % 16) * 2;
  GPIO_REGS->GPIO_PUP_PDN_CNTRL_REG[array_idx] &= ~(0x3 << bit_shift);
  GPIO_REGS->GPIO_PUP_PDN_CNTRL_REG[array_idx] |= (0x3 << bit_shift);
}

int gpio_init(void) {
  const GPIO_t *g = gpio_set;
  while (g->name) {
    gpio_set_function(g->gpio, g->func);
    gpio_set_pup_pdn(g->gpio, g->gpio_pud_pdn);
    g++;
  }
  return 0;
}

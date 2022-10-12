#include "gpio.h"



static gpio_regs_t *gpio_reg;
static uint8_t initted = 0;

static void set_gpio_fn(int gpio, int fn){

    gpio_reg->gpfsel[GPIO_TO_FSEL_ARRAY_IDX(gpio)] |= ((fn & 0x3)<<GPIO_TO_FSEL_ARRAY_SHIFT(gpio));

}

static void gpio_output_set(int gpio){
	gpio_reg->gpset[GPIO_TO_IDX(gpio)]|= 0x1<<GPIO_TO_SHIFT(gpio);
}
static void gpio_output_clear(int gpio){
	gpio_reg->gpclr[GPIO_TO_IDX(gpio)]|= 0x1<<GPIO_TO_SHIFT(gpio);
}
static int gpio_input_get(int gpio){
	return ((gpio_reg->gplev[GPIO_TO_IDX(gpio)]>>GPIO_TO_SHIFT(gpio))&0x1);
}

static void gpio_set_pud(int gpio, int val){

    int cycle_count = 150;
    gpio_reg->gppud = val;
    while(cycle_count--){
	asm volatile("nop");
    }
    gpio_reg->gppudclk[GPIO_TO_IDX(gpio)] = 0x1 <<GPIO_TO_SHIFT(gpio);
    cycle_count = 150;
    while(cycle_count--){
	asm volatile("nop");
    }
    gpio_reg->gppud = 0;
    gpio_reg->gppudclk[GPIO_TO_IDX(gpio)] = 0x0;
}

void gpio_set_value(int gpio, int val){
    if(val){
	gpio_output_set(gpio);
    }else{
	gpio_output_clear(gpio);
    }
}

int gpio_get_value(int gpio){
    return gpio_input_get(gpio);
}



void gpio_direction_input(int gpio){
    set_gpio_fn(gpio, GPIO_INPUT);
}

void gpio_direction_output(int gpio){

    set_gpio_fn(gpio, GPIO_OUTPUT);
}

void gpio_set_fn(int gpio, int fn){
    switch(fn){
	case 0:
		set_gpio_fn(gpio, GPIO_ALT_FN0);
		break;
	case 1:
		set_gpio_fn(gpio, GPIO_ALT_FN1);
		break;
	case 2:
		set_gpio_fn(gpio, GPIO_ALT_FN2);
		break;
	case 3:
		set_gpio_fn(gpio, GPIO_ALT_FN3);
		break;
	case 4:
		set_gpio_fn(gpio, GPIO_ALT_FN4);
		break;
	case 5:
		set_gpio_fn(gpio, GPIO_ALT_FN5);
		break;
    }

    gpio_set_pud(gpio, 0);
}

void gpio_init(void){

    gpio_reg = (gpio_regs_t *)GPIO_BASE_ADDR;
    initted = 1;

}

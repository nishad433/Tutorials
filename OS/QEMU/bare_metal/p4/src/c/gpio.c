#include <gpio.h>




static void set_gpio_fn(int gpio, gpio_fn_t fn){

    GPIO_REGS->gpfsel[GPIO_TO_FSEL_ARRAY_IDX(gpio)] &= ~((0x7)<<GPIO_TO_FSEL_ARRAY_SHIFT(gpio));
    GPIO_REGS->gpfsel[GPIO_TO_FSEL_ARRAY_IDX(gpio)] |= ((fn & 0x7)<<GPIO_TO_FSEL_ARRAY_SHIFT(gpio));

}

static void gpio_output_set(int gpio){
	GPIO_REGS->gpset[GPIO_TO_IDX(gpio)] &= ~(0x1<<GPIO_TO_SHIFT(gpio));
	GPIO_REGS->gpset[GPIO_TO_IDX(gpio)] |= 0x1<<GPIO_TO_SHIFT(gpio);
}
static void gpio_output_clear(int gpio){
	GPIO_REGS->gpclr[GPIO_TO_IDX(gpio)] &= ~(0x1<<GPIO_TO_SHIFT(gpio));
	GPIO_REGS->gpclr[GPIO_TO_IDX(gpio)] |= 0x1<<GPIO_TO_SHIFT(gpio);
}
static int gpio_input_get(int gpio){
	return ((GPIO_REGS->gplev[GPIO_TO_IDX(gpio)]>>GPIO_TO_SHIFT(gpio))&0x1);
}

static void gpio_set_pud(int gpio, int val){

    	int cycle_count = 150;
    	GPIO_REGS->gppud = val;
    	while(cycle_count--){
		asm volatile("nop");
    	}
    	GPIO_REGS->gppudclk[GPIO_TO_IDX(gpio)] = 0x1 <<GPIO_TO_SHIFT(gpio);
    	cycle_count = 150;
    	while(cycle_count--){
		asm volatile("nop");
    	}
    	GPIO_REGS->gppud = 0;
    	GPIO_REGS->gppudclk[GPIO_TO_IDX(gpio)] = 0x0;
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
    	set_gpio_fn(gpio, gpio_fn_input);
}

void gpio_direction_output(int gpio){

    	set_gpio_fn(gpio, gpio_fn_output);
}

void gpio_set_fn(int gpio, gpio_fn_t fn){

	set_gpio_fn(gpio, fn);
    	gpio_set_pud(gpio, 0);
}

int gpio_init(void){
	return 0;
}

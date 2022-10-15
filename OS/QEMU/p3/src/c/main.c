#include<platform.h>
#include<gpio.h>
#include<uart.h>
#include<printk.h>

int main(void){
        uart_cfg_t uart_cfg={
                .data_bits = 8,
                .stop_bits = 1,
                .parity = 0,
                .baudrate = 9600
        };
	
	gpio_init();	
	uart1_init();
	uart_configure(uart_cfg);

	printk("Hello World Nishad\n");

	return 0;
}

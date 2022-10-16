#include<platform.h>
#include<gpio.h>
#include<uart.h>
#include<printk.h>

int main(void){
#if defined(UART0_SUPPORT) || defined(UART1_SUPPORT)
        uart_cfg_t uart_cfg={
                .data_bits = 8,
                .stop_bits = 1,
                .parity = 0,
                .baudrate = 115200
        };
#endif
	
	gpio_init();
#if defined(UART0_SUPPORT) || defined(UART1_SUPPORT)
	uart_configure(uart_cfg);
#endif
	printk("Hello World Nishad\n");

	return 0;
}

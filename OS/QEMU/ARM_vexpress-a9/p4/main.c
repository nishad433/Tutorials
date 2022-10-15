#include "printk.h"

void main(void){
	uart_cfg_t uart_cfg={
		.data_bits = 8,
		.stop_bits = 1,
		.parity = 0,
		.baudrate = 9600
	};
	uart_configure(uart_cfg);
}

#include"gpio.h"
#include"uart.h"
void main(void)
{
    gpio_init();
    uart1_init();
    uart_puts("Hello nishad\n");
    while(1){
        ;
    }   
}

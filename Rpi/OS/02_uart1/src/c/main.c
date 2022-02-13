#include"gpio.h"
#include"uart.h"
#include"printk.h"
void main(void)
{
    gpio_init();
    uart1_init();
    printk("Hello nishad\n");
    while(1){
        ;
    }   
}

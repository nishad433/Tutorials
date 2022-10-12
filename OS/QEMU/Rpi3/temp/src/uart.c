#include "uart.h"
#include "lib.h"


void uart_init(){
    reg_write(UART_CR,0);
    reg_write(UART_LCRH,(1<<4|1<<5|1<<6));
    reg_write(UART_CR,(1<<0|1<<8|1<<9));

}

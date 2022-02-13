#include"uart.h"
#include"gpio.h"



static aux_regs_t *aux_reg;

/*
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */

void uart1_init(void){

	aux_reg = (aux_regs_t *)AUX_BASE_ADDR;
	aux_reg->aux_mu_cntl = 0x0;
	aux_reg->aux_enable = 0x1;
	aux_reg->aux_mu_ier = 0x0;
	aux_reg->aux_mu_iir = 0x6;
	aux_reg->aux_mu_lcr = 0x3;
	aux_reg->aux_mu_baud = 270;
	gpio_set_fn(14, 5);
	gpio_set_fn(15, 5);
	aux_reg->aux_mu_cntl = 0x3;



}

void uart_send(unsigned int c){

    do{
	    asm volatile("nop");
    }while(!(aux_reg->aux_mu_lsr &0x20));

    aux_reg->aux_mu_io = c;
}

void uart_puts(char *s){
    while(*s){
	if(*s=='\0')
		break;
	if(*s=='\n'){
		uart_send('\r');
	}
	uart_send(*s++);
    }

}

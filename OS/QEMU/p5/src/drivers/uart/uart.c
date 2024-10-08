#include<uart.h>
#include<gpio.h>


//#define disp(name) { printf("NCC %s %lx\n",#name, offsetof(uart_regs_t, name)); }


void uart_putchar(char c){
#if defined(CONSOLE_UART0)
	while( UART0_REGS->fr & FR_TXFF );
	UART0_REGS->dr = c;
#elif defined(CONSOLE_UART1)
	while(!(AUX_REGS->aux_mu_lsr &0x20));
	AUX_REGS->aux_mu_io = c;
#endif
}

void write(const char *str){
	while(*str){
		uart_putchar(*str++);
	}
}

static inline void write_set(volatile uint32_t *offset, uint32_t val){
	uint32_t v = *offset;
	v |= val;
	*offset = v;
}
static inline void write_clr(volatile uint32_t *offset, uint32_t val){
	uint32_t v = *offset;
	v &= ~val;
	*offset = v;
}

int uart_configure(uart_cfg_t cfg){

	if(cfg.data_bits < 5u || cfg.data_bits > 8u ){
		return UART_WORDSIZE_ERR;
	}
	if(cfg.stop_bits < 1u || cfg.stop_bits > 2u ){
		return UART_STOP_BITS_ERR;
	}
	if(cfg.baudrate < 110u || cfg.baudrate > 460800u ){
		return UART_BAUDRATE_ERR;
	}
#if defined(UART0_SUPPORT)
#if defined(GPIO_SUPPORT) && defined(RASPI3) && defined(CONSOLE_UART0)
	gpio_set_fn(UART_TXD, gpio_fn_altfn0);
	gpio_set_fn(UART_RXD, gpio_fn_altfn0);
#endif
	int ibrd, frac, val;
	write_clr( &UART0_REGS->cr, CR_UARTEN );
	while( UART0_REGS->fr & FR_BUSY );
	write_clr( &UART0_REGS->lcr_h, LCRH_FEN );

	ibrd = UARTCLK/(16*cfg.baudrate);
	frac = ((UARTCLK%(16*cfg.baudrate))*64/(16*cfg.baudrate)) + 0.5;
	UART0_REGS->ibrd = ibrd;
	UART0_REGS->fbrd = frac;

	val = UART0_REGS->lcr_h;
	val &= ~(7 << 5u | 0x7 << 1u);
	val |= ((cfg.data_bits - 5) & 3) << 5 /* WLEN */;
	val |= (cfg.stop_bits & 2) << 3 /* STP2 */;
	val |= 1 << 4 /* FEN */;
	if(cfg.parity){
		val |= (1 << 7 | 1 << 3 | 1 << 1);
	}
	UART0_REGS->lcr_h = val;

	write_set( &UART0_REGS->cr, CR_UARTEN | CR_TXE | CR_RXE );
#endif
#if defined(UART1_SUPPORT)
	AUX_REGS->aux_mu_cntl = 0x0;
	AUX_REGS->aux_enable = 0x1;
	AUX_REGS->aux_mu_ier = 0x0;
	AUX_REGS->aux_mu_iir = 0x6;
	AUX_REGS->aux_mu_lcr = 0x3;
	AUX_REGS->aux_mu_baud = 270;
#if defined(GPIO_SUPPORT) && defined(RASPI3) && defined(CONSOLE_UART1)
	gpio_set_fn(UART_TXD, gpio_fn_altfn5);
	gpio_set_fn(UART_RXD, gpio_fn_altfn5);
#endif
	AUX_REGS->aux_mu_cntl = 0x3;
#endif
	return UART_OK;
}



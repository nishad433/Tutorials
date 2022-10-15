#include<uart.h>
#include<gpio.h>


//#define disp(name) { printf("NCC %s %lx\n",#name, offsetof(uart_regs_t, name)); }


void uart_putchar(char c){
#if defined(VEXP_A9)
	while( UART_REGS->fr & FR_TXFF );
	UART_REGS->dr = c;
#elif defined(RASPI3)
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


#if defined(RASPI3)
int uart1_init(void){
	AUX_REGS->aux_mu_cntl = 0x0;
	AUX_REGS->aux_enable = 0x1;
	AUX_REGS->aux_mu_ier = 0x0;
	AUX_REGS->aux_mu_iir = 0x6;
	AUX_REGS->aux_mu_lcr = 0x3;
	AUX_REGS->aux_mu_baud = 270;
	gpio_set_fn(14, 5);
	gpio_set_fn(15, 5);
	AUX_REGS->aux_mu_cntl = 0x3;
	return 0;
}
#elif defined(VEXP_A9)
int uart1_init(void){
	return 0;
}
#endif //RASPI3

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
#if defined(VEXP_A9)
	int val;
	write_clr( &UART_REGS->cr, CR_UARTEN );
	while( UART_REGS->fr & FR_BUSY );
	write_clr( &UART_REGS->lcr_h, LCRH_FEN );

	UART_REGS->ibrd = 156;
	UART_REGS->fbrd = 63;

	val = UART_REGS->lcr_h;
	val &= ~(7 << 5u | 0x7 << 1u);
	val |= ((cfg.data_bits - 5) & 3) << 5 /* WLEN */;
	val |= (cfg.stop_bits & 2) << 3 /* STP2 */;
	val |= 1 << 4 /* FEN */;
	if(cfg.parity){
		val |= (1 << 7 | 1 << 3 | 1 << 1);
	}
	UART_REGS->lcr_h = val;

	write_set( &UART_REGS->cr, CR_UARTEN | CR_TXE | CR_RXE );
#elif defined(RASPI3)
#endif
	return UART_OK;
}



#ifndef _UART_H_
#define _UART_H_

#include<stdint.h>

typedef enum {
	UART_OK = 0,
	UART_BAUDRATE_ERR,
	UART_WORDSIZE_ERR,
	UART_STOP_BITS_ERR,
	UART_RECV_ERR,
	UART_NO_DATA
}uart_error;

typedef struct uart_regs{
        uint32_t dr;
        uint32_t rsr;
        uint32_t reserved[4];
#define FR_TXFF (1 << 5u)
#define FR_BUSY (1 << 3u)
        uint32_t fr;
        uint32_t reserved1;
        uint32_t ilpr;
        uint32_t ibrd;
        uint32_t fbrd;
#define LCRH_FEN (1 << 4u)
        uint32_t lcr_h;
#define CR_UARTEN (1 << 0u)
#define CR_TXE 	(1 << 8u)
#define CR_RXE 	(1 << 9u)
        uint32_t cr;
        uint32_t ifls;
        uint32_t imsc;
        uint32_t ris;
        uint32_t mis;
        uint32_t icr;
        uint32_t dmacr;
}uart_regs_t;

#define UART_BASE       0x10009000 

#define UART_REGS       ((volatile uart_regs_t *)UART_BASE)

typedef struct uart_cfg{
	uint8_t data_bits;
	uint8_t stop_bits;
	uint8_t parity;
	uint32_t baudrate;
}uart_cfg_t;

int uart_configure(uart_cfg_t cfg);
void write(const char *str);

#endif

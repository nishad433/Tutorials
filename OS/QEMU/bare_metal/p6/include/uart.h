#ifndef _UART_H_
#define _UART_H_

#include<platform.h>
#include<stdint.h>
#if defined(GPIO_SUPPORT)
#include<gpio.h>
#endif

#if defined(RASPI3)
#define UARTCLK 3000000u   // 3MHz
#if defined(GPIO_SUPPORT)
#define UART_TXD GPIO_14
#define UART_RXD GPIO_15
#endif
#elif defined(VEXP_A9)
#define UARTCLK 24000000u    // 24MHz 
#endif

#if defined(UART0_SUPPORT) || defined(UART1_SUPPORT)

typedef struct { 
        rw_REG32 dr;			// 0x00
        rw_REG32 rsr;			// 0x04
        ro_REG32 reserved[4];
#define FR_TXFF (1 << 5u)
#define FR_BUSY (1 << 3u)
        rw_REG32 fr;                	// 0x18
        ro_REG32 reserved1;
        rw_REG32 ilpr;			// 0x20
        rw_REG32 ibrd;              	// 0x24
        rw_REG32 fbrd;			// 0x28
#define LCRH_FEN (1 << 4u)
        rw_REG32 lcr_h;		// 0x2c
#define CR_UARTEN (1 << 0u)
#define CR_TXE  (1 << 8u)
#define CR_RXE  (1 << 9u)
        rw_REG32 cr; 			// 0x30
        rw_REG32 ifls; 		// 0x34
        rw_REG32 imsc; 		// 0x38
        rw_REG32 ris;			// 0x3c
        rw_REG32 mis;			// 0x40
        rw_REG32 icr;			// 0x44
        rw_REG32 dmacr;		// 0x48
        ro_REG32 reserved3[0xd];    	// 0x4c
        rw_REG32 itcr;              	// 0x80
        rw_REG32 itip;              	// 0x84
        rw_REG32 itop;              	// 0x88
        rw_REG32 tdr;               	// 0x8c        
}uart_regs_t;

typedef enum {
        UART_OK = 0,
        UART_BAUDRATE_ERR,
        UART_WORDSIZE_ERR,
        UART_STOP_BITS_ERR,
        UART_RECV_ERR,
        UART_NO_DATA
}uart_error;


typedef struct uart_cfg{
        uint8_t data_bits;
        uint8_t stop_bits;
        uint8_t parity;
        rw_REG32 baudrate;
}uart_cfg_t;

#if defined(UART0_SUPPORT)

#define UART0_REGS       ((volatile uart_regs_t *)UART0_BASE)

#endif //UART0_SUPPORT
#if defined(UART1_SUPPORT)

/* Auxilary mini UART registers */
typedef struct {
    rw_REG32 aux_irq;                   // 0x00
    rw_REG32 aux_enable;                // 0x04
    ro_REG32 reserved[0xe];             // 0x08
    rw_REG32 aux_mu_io;                 // 0x40
    rw_REG32 aux_mu_ier;                // 0x44
    rw_REG32 aux_mu_iir;                // 0x48
    rw_REG32 aux_mu_lcr;                // 0x4c
    rw_REG32 aux_mu_mcr;                // 0x50
    rw_REG32 aux_mu_lsr;                // 0x54
    rw_REG32 aux_mu_msr;                // 0x58
    rw_REG32 aux_mu_scratch;            // 0x5c
    rw_REG32 aux_mu_cntl;               // 0x60
    rw_REG32 aux_mu_stat;               // 0x64
    rw_REG32 aux_mu_baud;               // 0x68
}aux_regs_t;

#define AUX_REGS       ((volatile aux_regs_t *)AUX_BASE)

#define UART1_REGS       ((volatile uart_regs_t *)UART1_BASE)

#endif //UART1_SUPPORT

int uart_configure(uart_cfg_t cfg);
void write(const char *str);

#else
static inline void write(const char *str){};
#endif // (UART0_SUPPORT) || UART1_SUPPORT

#endif //_UART_H_

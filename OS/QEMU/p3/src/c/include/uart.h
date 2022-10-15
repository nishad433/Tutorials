#ifndef _UART_H_
#define _UART_H_

#include<platform.h>
#include<stdint.h>


#ifdef UART_SUPPORT
#if defined(VEXP_A9)

typedef struct {
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
#define CR_TXE  (1 << 8u)
#define CR_RXE  (1 << 9u)
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

#elif defined(RASPI3)

/* Auxilary mini UART registers */
typedef struct {
    uint32_t aux_irq;                   // 0x00
    uint32_t aux_enable;                // 0x04
    uint32_t reserved[0xe];             // 0x08
    uint32_t aux_mu_io;                 // 0x40
    uint32_t aux_mu_ier;                // 0x44
    uint32_t aux_mu_iir;                // 0x48
    uint32_t aux_mu_lcr;                // 0x4c
    uint32_t aux_mu_mcr;                // 0x50
    uint32_t aux_mu_lsr;                // 0x54
    uint32_t aux_mu_msr;                // 0x58
    uint32_t aux_mu_scratch;            // 0x5c
    uint32_t aux_mu_cntl;               // 0x60
    uint32_t aux_mu_stat;               // 0x64
    uint32_t aux_mu_baud;               // 0x68
}aux_regs_t;

#define AUX_BASE   (IO_BASE_ADDR + 0x00215000)
#define AUX_REGS       ((volatile aux_regs_t *)AUX_BASE)

typedef struct {
    uint32_t dr;                // 0x00
    uint32_t reserved1[5];      // 0x04
    uint32_t fr;                // 0x18
    uint32_t reserved2[2];      // 0x1c
    uint32_t ibrd;              // 0x24
    uint32_t fbrd;              // 0x28
    uint32_t lcrh;              // 0x2c
    uint32_t cr;                // 0x30
    uint32_t ifls;              // 0x34
    uint32_t imsc;              // 0x38
    uint32_t ris;               // 0x3c
    uint32_t mis;               // 0x40
    uint32_t icr;               // 0x44
    uint32_t dmacr;             // 0x48
    uint32_t reserved3[0xd];    // 0x4c
    uint32_t itcr;              // 0x80
    uint32_t itip;              // 0x84
    uint32_t itop;              // 0x88
    uint32_t tdr;               // 0x8c
}uart_regs_t;

#define UART0_BASE  IO_BASE_ADDR + 0x1000
#define UART_REGS       ((volatile uart_regs_t *)UART0_BASE)

#endif
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
        uint32_t baudrate;
}uart_cfg_t;


int uart1_init(void);
int uart_configure(uart_cfg_t cfg);
void write(const char *str);

#else
static inline int uart1_init(void){ return 0; }
static inline int uart_configure(uart_cfg_t cfg){ return 0;}
static inline void write(const char *str){};
#endif // UART_SUPPORT

#endif //_UART_H_

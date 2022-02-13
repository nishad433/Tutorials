#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include "mmio.h"

#define UART0_BASE_ADDR  IO_BASE_ADDR + 0x1000

/* Auxilary mini UART registers */
#define AUX_BASE_ADDR	(IO_BASE_ADDR + 0x00215000)
#define AUX_IRQ         (AUX_BASE_ADDR + 0x00)
#define AUX_ENABLE      (AUX_BASE_ADDR + 0x04)
#define AUX_MU_IO       (AUX_BASE_ADDR + 0x40)
#define AUX_MU_IER      (AUX_BASE_ADDR + 0x44)
#define AUX_MU_IIR      (AUX_BASE_ADDR + 0x48)
#define AUX_MU_LCR      (AUX_BASE_ADDR + 0x4C)
#define AUX_MU_MCR      (AUX_BASE_ADDR + 0x50)
#define AUX_MU_LSR      (AUX_BASE_ADDR + 0x54)
#define AUX_MU_MSR      (AUX_BASE_ADDR + 0x58)
#define AUX_MU_SCRATCH  (AUX_BASE_ADDR + 0x5C)
#define AUX_MU_CNTL     (AUX_BASE_ADDR + 0x60)
#define AUX_MU_STAT     (AUX_BASE_ADDR + 0x64)
#define AUX_MU_BAUD     (AUX_BASE_ADDR + 0x68)


typedef struct {
    uint32_t aux_irq;			// 0x00
    uint32_t aux_enable;		// 0x04
    uint32_t reserved[0xe];		// 0x08
    uint32_t aux_mu_io;			// 0x40
    uint32_t aux_mu_ier;		// 0x44
    uint32_t aux_mu_iir;                // 0x48
    uint32_t aux_mu_lcr;                // 0x4c
    uint32_t aux_mu_mcr;                // 0x50
    uint32_t aux_mu_lsr;                // 0x54
    uint32_t aux_mu_msr;                // 0x58
    uint32_t aux_mu_scratch;		// 0x5c
    uint32_t aux_mu_cntl;		// 0x60
    uint32_t aux_mu_stat;		// 0x64
    uint32_t aux_mu_baud;		// 0x68
}aux_regs_t;



#define UART0_D0     (UART_BASE_ADDR + 0x00)
#define UART0_FR     (UART_BASE_ADDR + 0x18)
#define UART0_IBRD   (UART_BASE_ADDR + 0x24)
#define UART0_FBRD   (UART_BASE_ADDR + 0x28)
#define UART0_LCRH   (UART_BASE_ADDR + 0x2c)
#define UART0_CR     (UART_BASE_ADDR + 0x30)
#define UART0_IFLS   (UART_BASE_ADDR + 0x34)
#define UART0_IMSC   (UART_BASE_ADDR + 0x38)
#define UART0_RIS    (UART_BASE_ADDR + 0x3c)
#define UART0_MIS    (UART_BASE_ADDR + 0x40)
#define UART0_ICR    (UART_BASE_ADDR + 0x44)
#define UART0_DMACR  (UART_BASE_ADDR + 0x48)
#define UART0_ITCR   (UART_BASE_ADDR + 0x80)
#define UART0_ITIP   (UART_BASE_ADDR + 0x84)
#define UART0_ITOP   (UART_BASE_ADDR + 0x88)
#define UART0_TDR    (UART_BASE_ADDR + 0x8c)


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
}uart0_regs;


void uart1_init(void);
void uart_puts(char *s);

#endif

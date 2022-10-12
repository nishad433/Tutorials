#ifndef _UART_H_
#define _UART_H_

#include "mmio.h"

#define UART_BASE_ADDR	IO_BASE_ADDR + 0x1000

#define UART_D0	    UART_BASE_ADDR + 0x0
#define UART_FR	    UART_BASE_ADDR + 0x18
#define UART_IBRD   UART_BASE_ADDR + 0x24
#define UART_FBRD   UART_BASE_ADDR + 0x28
#define UART_LCRH   UART_BASE_ADDR + 0x2c
#define UART_CR	    UART_BASE_ADDR + 0x30
#define UART_IFLS   UART_BASE_ADDR + 0x34
#define UART_IMSC   UART_BASE_ADDR + 0x38
#define UART_RIS    UART_BASE_ADDR + 0x3c
#define UART_MIS    UART_BASE_ADDR + 0x40
#define UART_ICR    UART_BASE_ADDR + 0x44
#define UART_DMACR  UART_BASE_ADDR + 0x48
#define UART_ITCR   UART_BASE_ADDR + 0x80
#define UART_ITIP   UART_BASE_ADDR + 0x84
#define UART_ITOP   UART_BASE_ADDR + 0x88
#define UART_TDR    UART_BASE_ADDR + 0x8c

typedef struct {
    uint32_t dr;		0x0
    uint32_t reserved1[5];	0x4
    uint32_t fr;		0x18
    uint32_t reserved2[2];	0x1c
    uint32_t ibrd;		0x24
    uint32_t fbrd;		0x28
    uint32_t lcrh;		0x2c
    uint32_t cr;		0x30
    uint32_t ifls;		0x34
    uint32_t imsc;		0x38
    uint32_t ris;		0x3c
    uint32_t mis;		0x40
    uint32_t icr;		0x44
    uint32_t dmacr;		0x48
    uint32_t reserved3[13];      0x4c
    uint32_t itcr;		0x80
    uint32_t itip;		0x84
    uint32_t itop;		0x88
    uint32_t tdr;		0x8c
}uart0_regs;

void uart_init();

#endif

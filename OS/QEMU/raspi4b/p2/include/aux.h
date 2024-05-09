#ifndef _AUX_H_
#define _AUX_H_

#include <base.h>
#include <raspi4_types.h>

typedef struct {
  rw_REG32 AUX_IRQ;     // [0x00] Auxiliary Interrupt status
  rw_REG32 AUX_ENABLES; // [0x04] Auxiliary enables
  uint32_t reserved0[0xe];
  rw_REG32 AUX_MU_IO_REG;   // [0x40] Mini UART I/O Data
  rw_REG32 AUX_MU_IER_REG;  // [0x44] Mini UART Interrupt Enable
  rw_REG32 AUX_MU_IIR_REG;  // [0x48] Mini UART Interrupt Identify
  rw_REG32 AUX_MU_LCR_REG;  // [0x4c] Mini UART Line Control
  rw_REG32 AUX_MU_MCR_REG;  // [0x50] Mini UART Modem Control
  rw_REG32 AUX_MU_LSR_REG;  // [0x54] Mini UART Line Status
  rw_REG32 AUX_MU_MSR_REG;  // [0x58] Mini UART Modem Status
  rw_REG32 AUX_MU_SCRATCH;  // [0x5c] Mini UART Scratch
  rw_REG32 AUX_MU_CNTL_REG; // [0x60] Mini UART Extra Control
  rw_REG32 AUX_MU_STAT_REG; // [0x64] Mini UART Extra Status
  rw_REG32 AUX_MU_BAUD_REG; // [0x68] Mini UART Baudrate
  uint32_t reserved1[0x5];
  rw_REG32 AUX_SPI1_CNTL0_REG; // [0x80] SPI 1 Control register 0
  rw_REG32 AUX_SPI1_CNTL1_REG; // [0x84] SPI 1 Control register 1
  rw_REG32 AUX_SPI1_STAT_REG;  // [0x88] SPI 1 Status
  rw_REG32 AUX_SPI1_PEEK_REG;  // [0x8c] SPI 1 Peek
  uint32_t reserved2[0x4];
  rw_REG32 AUX_SPI1_IO_REGa;     // [0xa0] SPI 1 Data
  rw_REG32 AUX_SPI1_IO_REGb;     // [0xa4] SPI 1 Data
  rw_REG32 AUX_SPI1_IO_REGc;     // [0xa8] SPI 1 Data
  rw_REG32 AUX_SPI1_IO_REGd;     // [0xac] SPI 1 Data
  rw_REG32 AUX_SPI1_TXHOLD_REGa; // [0xb0] SPI 1 Extended Data
  rw_REG32 AUX_SPI1_TXHOLD_REGb; // [0xb4] SPI 1 Extended Data
  rw_REG32 AUX_SPI1_TXHOLD_REGc; // [0xb8] SPI 1 Extended Data
  rw_REG32 AUX_SPI1_TXHOLD_REGd; // [0xbc] SPI 1 Extended Data
  rw_REG32 AUX_SPI2_CNTL0_REG;   // [0xc0] SPI 2 Control register 0
  rw_REG32 AUX_SPI2_CNTL1_REG;   // [0xc4] SPI 2 Control register 1
  rw_REG32 AUX_SPI2_STAT_REG;    // [0xc8] SPI 2 Status
  rw_REG32 AUX_SPI2_PEEK_REG;    // [0xcc] SPI 2 Peek
  uint32_t reserved3[0x5];
  rw_REG32 AUX_SPI2_IO_REGa;     // [0xe0] SPI 2 Data
  rw_REG32 AUX_SPI2_IO_REGb;     // [0xe4] SPI 2 Data
  rw_REG32 AUX_SPI2_IO_REGc;     // [0xe8] SPI 2 Data
  rw_REG32 AUX_SPI2_IO_REGd;     // [0xec] SPI 2 Data
  rw_REG32 AUX_SPI2_TXHOLD_REGa; // [0xf0] SPI 2 Extended Data
  rw_REG32 AUX_SPI2_TXHOLD_REGb; // [0xf4] SPI 2 Extended Data
  rw_REG32 AUX_SPI2_TXHOLD_REGc; // [0xf8] SPI 2 Extended Data
  rw_REG32 AUX_SPI2_TXHOLD_REGd; // [0xfc] SPI 2 Extended Data
} aux_regs_t;

#define AUX_BASE (PERIPH_BASE + 0x215000)

#define AUX_REGS ((volatile aux_regs_t *)AUX_BASE)

#define AUX_PERIPH_MODULE_MINI_UART 0x0
#define AUX_PERIPH_MODULE_SPI1 0x1
#define AUX_PERIPH_MODULE_SPI2 0x2

void enable_aux_periph_module(int, int);

#endif // _AUX_H_

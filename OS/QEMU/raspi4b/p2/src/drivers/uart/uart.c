#include <aux.h>
#include <gpio.h>
#include <uart.h>

void uart_putchar(char c) {
  while (!(AUX_REGS->AUX_MU_LSR_REG & 0x20))
    ;
  AUX_REGS->AUX_MU_IO_REG = c;
}

void write(const char *str) {
  while (*str) {
    uart_putchar(*str++);
  }
}

int uart_init() {

  AUX_REGS->AUX_MU_CNTL_REG = 0 << 1 /*TX disable*/ | 0 << 0 /*RX disable*/;
  AUX_REGS->AUX_MU_IER_REG = 0 << 1 /*RX IRQ disable*/ | 0 << 0 /*TX IRQ disable*/;
  AUX_REGS->AUX_MU_IIR_REG = 1 << 2 /* clear TX FIFO*/ | 1 << 1 /* clear RX FIFO*/;
  AUX_REGS->AUX_MU_LCR_REG = 1 << 1 /*8 bit mode*/;
  AUX_REGS->AUX_MU_BAUD_REG = 270 /* mini UART baud counter*/;
  // set GPIO pins
  gpio_set_function(14, gpio_alt_func5);
  gpio_set_function(15, gpio_alt_func5);
  AUX_REGS->AUX_MU_CNTL_REG = 1 << 1 /*TX enable*/ | 1 << 0 /*RX enable*/;

  enable_aux_periph_module(AUX_PERIPH_MODULE_MINI_UART, 1);

  return 0;
}

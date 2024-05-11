#include <aux.h>

void enable_aux_periph_module(int module, int enable) {
  if (module >= AUX_PERIPH_MODULE_MINI_UART &&
      module <= AUX_PERIPH_MODULE_SPI2) {
    if (enable) {
      AUX_REGS->AUX_ENABLES |= 1 << module;
    } else {
      AUX_REGS->AUX_ENABLES &= ~(1 << module);
    }
  }
}
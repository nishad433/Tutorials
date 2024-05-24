#include <platform.h>
#include <gpio.h>
#include <printk.h>
#include <uart.h>
#include <irq.h>
#include <generic_timer.h>
#include <mm.h>
#include <debug.h>

// First routine after C environment is setup
int main(void) {
  gpio_init();
  uart_init();
  printk("Starting\nEL=%d\n",current_el());
  irq_init();
  generic_timer_init();
  mm_init();
  debug_init();
  hang();
  return 0;
}
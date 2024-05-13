#include <platform.h>
#include <gpio.h>
#include <printk.h>
#include <uart.h>
#include <irq.h>
#include <generic_timer.h>
#include <mm.h>

// First routine after C environment is setup
int main(void) {
  gpio_init();
  uart_init();
  printk("Starting\nEL=%d\n",current_el());
  mm_init();
  irq_init();
  generic_timer_init();
  hang();
  return 0;
}

#include <platform.h>
#include <gpio.h>
#include <printk.h>
#include <uart.h>
#include <irq.h>
#include <generic_timer.h>

// First routine after C environment is setup
int main(void) {
  gpio_init();
  uart_init();
  printk("Starting\nEL=%d\n",getCurrEL());
  irq_init();
  generic_timer_init();
  while (1) {
  }
  return 0;
}

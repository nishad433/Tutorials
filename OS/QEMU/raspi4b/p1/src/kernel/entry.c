#include <gpio.h>
#include <printk.h>
#include <uart.h>

// First routine after C environment is setup
int main(void) {
  gpio_init();
  uart_init();
  printk("Hello World\n");
  while (1) {
  }
  return 0;
}

#include <gpio.h>
#include <printk.h>
#include <uart.h>

int getCurrEL();
// First routine after C environment is setup
int main(void) {
  gpio_init();
  uart_init();
  printk("Starting\nEL=%d\n",getCurrEL());
  while (1) {
  }
  return 0;
}

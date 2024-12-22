#include <stdint.h>
#include <stdio.h>

typedef uint32_t rw_REG32;

typedef struct {
  rw_REG32 GPFSEL0; // [0x00] GPIO Function Select 0
  rw_REG32 GPFSEL1; // [0x04] GPIO Function Select 1
  rw_REG32 GPFSEL2; // [0x08] GPIO Function Select 2
  rw_REG32 GPFSEL3; // [0x0c] GPIO Function Select 3
  rw_REG32 GPFSEL4; // [0x10] GPIO Function Select 4
  rw_REG32 GPFSEL5; // [0x14] GPIO Function Select 5
  uint32_t reserved0;
  rw_REG32 GPSET0; // [0x1c] GPIO Pin Output Set 0
  rw_REG32 GPSET1; // [0x20] GPIO Pin Output Set 1
  uint32_t reserved1;
  rw_REG32 GPCLR0; // [0x28] GPIO Pin Output Clear 0
  rw_REG32 GPCLR1; // [0x2c] GPIO Pin Output Clear 1
  uint32_t reserved2;
  rw_REG32 GPLEV0; // [0x34] GPIO Pin Level 0
  rw_REG32 GPLEV1; // [0x38] GPIO Pin Level 1
  uint32_t reserved3;
  rw_REG32 GPEDS0; // [0x40] GPIO Pin Event Detect Status 0
  rw_REG32 GPEDS1; // [0x44] GPIO Pin Event Detect Status 1
  uint32_t reserved4;
  rw_REG32 GPREN0; // [0x4c] GPIO Pin Rising Edge Detect Enable 0
  rw_REG32 GPREN1; // [0x50] GPIO Pin Rising Edge Detect Enable 1
  uint32_t reserved5;
  rw_REG32 GPFEN0; // [0x58] GPIO Pin Falling Edge Detect Enable 0
  rw_REG32 GPFEN1; // [0x5c] GPIO Pin Falling Edge Detect Enable 1
  uint32_t reserved6;
  rw_REG32 GPHEN0; // [0x64] GPIO Pin High Detect Enable 0
  rw_REG32 GPHEN1; // [0x68] GPIO Pin High Detect Enable 1
  uint32_t reserved7;
  rw_REG32 GPLEN0; // [0x70] GPIO Pin Low Detect Enable 0
  rw_REG32 GPLEN1; // [0x74] GPIO Pin Low Detect Enable 1
  uint32_t reserved8;
  rw_REG32 GPAREN0; // [0x7c] GPIO Pin Async. Rising Edge Detect 0
  rw_REG32 GPAREN1; // [0x80] GPIO Pin Async. Rising Edge Detect 1
  uint32_t reserved9;
  rw_REG32 GPAFEN0; // [0x88] GPIO Pin Async. Falling Edge Detect 0
  rw_REG32 GPAFEN1; // [0x8c] GPIO Pin Async. Falling Edge Detect 1
  uint32_t reserved10[0x15];
  rw_REG32
      GPIO_PUP_PDN_CNTRL_REG0; // [0xe4] GPIO Pull-up / Pull-down Register 0
  rw_REG32
      GPIO_PUP_PDN_CNTRL_REG1; // [0xe8] GPIO Pull-up / Pull-down Register 1
  rw_REG32
      GPIO_PUP_PDN_CNTRL_REG2; // [0xec] GPIO Pull-up / Pull-down Register 2
  rw_REG32
      GPIO_PUP_PDN_CNTRL_REG3; // [0xf0] GPIO Pull-up / Pull-down Register 3
} regs_t;

//#define disp(x)  printf("%-30s 0x%02lx\n",#x,(uint64_t)(&((regs_t *)0)->x));
#define disp(x) printf("0x%02lx\t%s\n", (uint64_t)(&((regs_t *)0)->x), #x);

int main() {

  disp(GPFSEL0);
  disp(GPFSEL1);
  disp(GPFSEL2);
  disp(GPFSEL3);
  disp(GPFSEL4);
  disp(GPFSEL5);
  disp(GPSET0);
  disp(GPSET1);
  disp(GPCLR0);
  disp(GPCLR1);
  disp(GPLEV0);
  disp(GPLEV1);
  disp(GPEDS0);
  disp(GPEDS1);
  disp(GPREN0);
  disp(GPREN1);
  disp(GPFEN0);
  disp(GPFEN1);
  disp(GPHEN0);
  disp(GPHEN1);
  disp(GPLEN0);
  disp(GPLEN1);
  disp(GPAREN0);
  disp(GPAREN1);
  disp(GPAFEN0);
  disp(GPAFEN1);
  disp(GPIO_PUP_PDN_CNTRL_REG0);
  disp(GPIO_PUP_PDN_CNTRL_REG1);
  disp(GPIO_PUP_PDN_CNTRL_REG2);
  disp(GPIO_PUP_PDN_CNTRL_REG3);

  return 0;
}
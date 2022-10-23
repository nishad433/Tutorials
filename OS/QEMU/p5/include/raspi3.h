#ifndef _RASPI3_H_
#define _RASPI3_H_

#include<asm_utils.h>

#define GPIO_SUPPORT
#define UART0_SUPPORT
#define UART1_SUPPORT
#if !defined(CONSOLE_UART0) &&  !defined(CONSOLE_UART1)
#define CONSOLE_UART0
#endif
#define MAILBOX_SUPPORT
#define RNG_SUPPORT

#define IO_BASE_ADDR 0x3F000000

#define UART0_BASE	( IO_BASE_ADDR + 0x00201000 )
#define UART1_BASE	( IO_BASE_ADDR + 0x1000 )
#define AUX_BASE	( IO_BASE_ADDR + 0x00215000 )
#define MAILBOX_BASE	( IO_BASE_ADDR + 0xB880 )
#define GPIO_BASE	( IO_BASE_ADDR + 0x00200000 )
#define RNG_BASE	( IO_BASE_ADDR + 0x00104000 )

#endif //_RASPI3_H_

#ifndef _BASE_H_
#define _BASE_H_

#include <sizes.h>

// low peripheral mode is the default boot environment. If you want to set high
// peripheral mode, you can do it explicitly in config.txt using arm_peri_high=1

// Low Peripheral Mode
#define MAIN_PERIPH_BASE_PA		0x0FC000000     // upto 0x0FF800000 // size 0x03800000
#define MAIN_PERIPH_SIZE                56UL * SZ_1M    // 0x003800000
#define ARM_LOCAL_PERIPH_BASE_PA	0x0FF800000     // upto 0x100000000 // size 0x00800000
#define ARM_LOCAL_PERIPH_SIZE		8UL * SZ_1M     // 0x000800000

// High Peripheral Mode - Full 35 Bit
//#define MAIN_PERIPH_BASE_PA           0x47C000000     // upto 0x480000000 // size 0x04000000
//#define MAIN_PERIPH_SIZE              0x004000000
//#define ARM_LOCAL_PERIPH_BASE_PA      0x4C0000000     // upto 0x500000000 // size 0x40000000
//#define ARM_LOCAL_PERIPH_SIZE         0x040000000

#define DDR_START_PA            	0x0
#define DDR_SIZE                	100UL * SZ_1M


/* Virutal Address Macros*/

#define MAIN_PERIPH_BASE_VA             MAIN_PERIPH_BASE_PA
#define ARM_LOCAL_PERIPH_BASE_VA        ARM_LOCAL_PERIPH_BASE_PA
#define DDR_START_VA                    DDR_START_PA

#endif // _BASE_H_

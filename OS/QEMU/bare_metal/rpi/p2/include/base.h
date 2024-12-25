#ifndef _BASE_H_
#define _BASE_H_

// low peripheral mode is the default boot environment. If you want to set high
// peripheral mode, you can do it explicitly in config.txt using arm_peri_high=1
#define PERIPH_BASE 0x0FE000000 // Low Peripheral Mode
//#define PERIPH_BASE 0x47E000000       // High Peripheral Mode - Full 35 Bit
//Mode

#endif // _BASE_H_

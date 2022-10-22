#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#if defined(VEXP_A9)
#include<vexp_a9.h>
#elif defined(RASPI3)
#include<raspi3.h>
#else
#error "Machine Type Not defined"
#endif


#endif //_PLATFORM_H_

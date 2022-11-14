#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include<stdint.h>

typedef const uint32_t	ro_REG32;
typedef uint32_t	rw_REG32;
typedef const uint64_t	ro_REG64;
typedef uint64_t	rw_REG64;

#if defined(VEXP_A9)
#include<vexp_a9.h>
#elif defined(RASPI3)
#include<raspi3.h>
#else
#error "Machine Type Not defined"
#endif


#endif //_PLATFORM_H_

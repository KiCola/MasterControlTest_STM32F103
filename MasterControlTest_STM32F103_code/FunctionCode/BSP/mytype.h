#ifndef __MYTYPE__
#define __MYTYPE__

#if defined(STM32F407xx)
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
    #include "stm32f1xx_hal.h"
#elif defined(STM32F030x6)
    #include "stm32f0xx_hal.h"
#endif

#include "main.h"


#define ABS(x) ((x)>0? (x):(-(x)))

typedef uint8_t 	u8;
typedef uint16_t 	u16;
typedef uint32_t 	u32;

typedef int8_t 		s8;
typedef int16_t 	s16;
typedef int32_t		s32;

typedef volatile uint8_t 	vu8;
typedef volatile uint16_t 	vu16;
typedef volatile uint32_t 	vu32;

typedef volatile int8_t 	vs8;
typedef volatile int16_t 	vs16;
typedef volatile int32_t	vs32;

typedef void(*VoidFuncVoid)(void);

#endif

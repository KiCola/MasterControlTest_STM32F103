#ifndef __BSP_HAL_UART3DMA232AndTTL_H__
#define __BSP_HAL_UART3DMA232AndTTL_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "mytype.h"


/* 类型定义 ------------------------------------------------------------------*/

/* 宏定义 --------------------------------------------------------------------*/
#define UART3_TTL 1
#define UART3_232 2

/* 扩展变量 ------------------------------------------------------------------*/
extern uint16_t USART3_RX_STA;    
/* 函数声明 ------------------------------------------------------------------*/
void Bsp_USART3_MODE_Init(uint8_t mode);
void Bsp_UART3_SetHandler(VoidFuncVoid func,uint8_t *USART_RX_BUF,uint16_t len);
void Bsp_UART3_Transmit(uint8_t *pData, uint16_t Size);


#endif  // __BSP_HAL_UART3DMA232AndTTL_H__


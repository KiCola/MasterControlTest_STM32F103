#ifndef __BSP_HAL_UARTDMA_H__
#define __BSP_HAL_UARTDMA_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "mytype.h"


/* 类型定义 ------------------------------------------------------------------*/

/* 宏定义 --------------------------------------------------------------------*/


/* 扩展变量 ------------------------------------------------------------------*/
extern uint16_t USART_RX_STA;    
/* 函数声明 ------------------------------------------------------------------*/
void Bsp_UART_SetHandler(VoidFuncVoid func,uint8_t *USART_RX_BUF,uint16_t len);
void Bsp_UART_Transmit(uint8_t *pData, uint16_t Size);


#endif  // __BSP_HAL_UARTDMA_H__


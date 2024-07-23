#ifndef __BSP_UART_H__
#define __BSP_UART_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "mytype.h"


/* 类型定义 ------------------------------------------------------------------*/

/* 宏定义 --------------------------------------------------------------------*/
#define UART_EN_TX HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET)
#define UART_EN_RX HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET)


#define USART_RX_LEN  	250 
#define USART_TX_LEN  	400
/* 扩展变量 ------------------------------------------------------------------*/
extern uint8_t USART_RX_BUF[USART_RX_LEN];   
extern uint16_t USART_RX_STA; 

extern uint8_t USART_TX_BUF[USART_TX_LEN];   
extern uint16_t USART_TX_STA; 
/* 函数声明 ------------------------------------------------------------------*/
void Bsp_UART_Transmit(uint8_t *pData, uint16_t Size);
void Bsp_UART_SetHandler(VoidFuncVoid func);


#endif  // __BSP_UART_H__


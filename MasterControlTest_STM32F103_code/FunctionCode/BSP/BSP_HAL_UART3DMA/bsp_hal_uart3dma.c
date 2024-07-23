/* 包含头文件 ----------------------------------------------------------------*/
#include "BSP_HAL_UART3DMA/bsp_hal_uart3dma.h"
#include "usart.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define  UARTDMAHEAD  huart3
/* 私有变量 ------------------------------------------------------------------*/
static VoidFuncVoid UARTCallBackFunc = NULL;
static uint16_t USART_RX_LEN;
static uint8_t *USART_RX_HEAD;
/* 扩展变量 ------------------------------------------------------------------*/
uint16_t USART3_RX_STA = 0; 
/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/ 
void Bsp_UART3_SetHandler(VoidFuncVoid func,uint8_t *USART_RX_BUF,uint16_t len)
{
	UARTCallBackFunc = func;
	USART_RX_LEN = len;
	USART_RX_HEAD = USART_RX_BUF;
    HAL_UART_Receive_DMA(&UARTDMAHEAD, USART_RX_HEAD, USART_RX_LEN);
    __HAL_UART_ENABLE_IT(&UARTDMAHEAD, UART_IT_IDLE); 
}


void Bsp_UART3_Transmit(uint8_t *pData, uint16_t Size)
{
    HAL_UART_Transmit_DMA(&UARTDMAHEAD,pData, Size);  
}

/*
把此函数放到stm32f1xx_it.c文件中对应函数中，在stm32f1xx_it.c文件中须加 extern void UART3_IDLECallBack(UART_HandleTypeDef *huart);
在本历程中是放在 void USART3_IRQHandler(void)函数中
*/
void UART3_IDLECallBack(UART_HandleTypeDef *huart)   
{  
    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
    {
        if(huart->Instance==UARTDMAHEAD.Instance)
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart); 
            HAL_UART_DMAStop(huart);  
            USART3_RX_STA = USART_RX_LEN - __HAL_DMA_GET_COUNTER(huart->hdmarx);
			if(USART3_RX_STA == 0)
			{
				USART3_RX_STA = USART_RX_LEN;
			}
			if(NULL != UARTCallBackFunc)
            {
                UARTCallBackFunc();
            }
            HAL_UART_Receive_DMA(huart, USART_RX_HEAD, USART_RX_LEN);
        }
    }
}      
    




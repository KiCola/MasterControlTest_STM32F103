/* 包含头文件 ----------------------------------------------------------------*/
#include "BSP_HAL_UART3DMA232AndTTL/bsp_hal_uart3dma232AndTTL.h"
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
void Bsp_USART3_MODE_Init(uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**USART3 GPIO Configuration    
    PC10     ------> USART3_TX
    PC11     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	if(UART3_TTL == mode)
	{
		__HAL_AFIO_REMAP_USART3_PARTIAL();
	}
	else if(UART3_232 == mode)
	{
		__HAL_AFIO_REMAP_USART3_DISABLE();
	}
}


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
    




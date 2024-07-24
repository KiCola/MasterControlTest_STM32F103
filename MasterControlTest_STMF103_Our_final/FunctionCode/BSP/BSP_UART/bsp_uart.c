/* 包含头文件 ----------------------------------------------------------------*/
#include "BSP_UART/bsp_uart.h"
#include "usart.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
static VoidFuncVoid UARTCallBackFunc = NULL;
/* 扩展变量 ------------------------------------------------------------------*/
uint8_t USART_RX_BUF[USART_RX_LEN];   
uint16_t USART_RX_STA = 0; 

uint8_t USART_TX_BUF[USART_TX_LEN];   
uint16_t USART_TX_STA = 0; 
/* 私有函数原形 --------------------------------------------------------------*/
#ifdef PRINTF_ENABLE
    #pragma import(__use_no_semihosting)
    struct __FILE 
    { 
        int handle; 
        /* Whatever you require here. If the only file you are using is */ 
        /* standard output using printf() for debugging, no file handling */ 
        /* is required. */ 
    };
    FILE __stdout;       
    void _sys_exit(int x) 
    { 
        x = x; 
    }
    int fputc(int ch, FILE *f)
    {
        UART_EN_TX;
        while((USART1->SR&0X40)==0);
        USART1->DR = (u8) ch; 
		while((USART1->SR&0X40)==0);
        UART_EN_RX;       
        return ch;
    }
#endif
/* 函数体 --------------------------------------------------------------------*/ 
void Bsp_UART_Transmit(uint8_t *pData, uint16_t Size)
{
    UART_EN_TX;
    HAL_UART_Transmit_DMA(&huart1,pData, Size);  
}
    
void Bsp_UART_SetHandler(VoidFuncVoid func)
{
	UARTCallBackFunc = func;
    UART_EN_RX;
    HAL_UART_Receive_DMA(&huart1, USART_RX_BUF, USART_RX_LEN);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); 
}
void UART_IDLECallBack(UART_HandleTypeDef *huart)  
{  
    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
    {
        if(huart->Instance==USART1)
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart); 
            HAL_UART_DMAStop(huart);  
            USART_RX_STA = USART_RX_LEN - __HAL_DMA_GET_COUNTER(huart->hdmarx);
            HAL_UART_Receive_DMA(huart, USART_RX_BUF, USART_RX_LEN);
            
            if(NULL != UARTCallBackFunc)
            {
                UARTCallBackFunc();
            }
        }
    }
}    
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) 
{
    UART_EN_RX;
}    
    




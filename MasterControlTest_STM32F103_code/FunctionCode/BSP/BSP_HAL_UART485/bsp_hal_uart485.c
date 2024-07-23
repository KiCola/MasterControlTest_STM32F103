/* 包含头文件 ----------------------------------------------------------------*/
#include "BSP_HAL_UART485/bsp_hal_uart485.h"
#include "usart.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define  UARTHEAD  huart1
/* 私有变量 ------------------------------------------------------------------*/
static VoidFuncVoid UART485CallBackFunc = NULL;
/* 扩展变量 ------------------------------------------------------------------*/
uint8_t RxBuffer;
/* 私有函数原形 --------------------------------------------------------------*/
#if (PRINTF_ENABLE)

#pragma import(__use_no_semihosting)
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
};
typedef struct __FILE FILE;
FILE __stdout;    
//定义_sys_exit()以避免使用半主机模式 	
void _sys_exit(int x) 
{ 
	x = x; 
}
//重定义fputc函数 
int fputc(int ch, FILE *f)
{
	UART_EN_TX;
	
	while((UARTHEAD.Instance  ->SR&0X40) == 0){}
	UARTHEAD.Instance  ->DR = (u8) ch; 
	while((UARTHEAD.Instance  ->SR&0X40) == 0){}
		
	UART_EN_RX;       
	return ch;
}
	
#endif
/* 函数体 --------------------------------------------------------------------*/ 
void Bsp_UART485_SetHandler(VoidFuncVoid func)
{
	UART485CallBackFunc = func;
    HAL_UART_Receive_IT(&UARTHEAD, &RxBuffer, 1);
}
void Bsp_UART485_Transmit(uint8_t *pData, uint16_t Size)
{
	UART_EN_TX;
	HAL_UART_Transmit(&UARTHEAD, pData, Size, Size);
	UART_EN_RX; 
}


/**
  * 函数功能: 串口接收完成回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == (UARTHEAD.Instance))
	{
		if(UART485CallBackFunc != NULL)
		{
			UART485CallBackFunc();
		}
		HAL_UART_Receive_IT(&UARTHEAD, &RxBuffer, 1);
	}
}




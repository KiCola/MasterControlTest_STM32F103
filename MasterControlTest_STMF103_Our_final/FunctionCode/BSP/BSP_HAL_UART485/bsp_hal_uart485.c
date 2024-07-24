/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "BSP_HAL_UART485/bsp_hal_uart485.h"
#include "usart.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define  UARTHEAD  huart1
/* ˽�б��� ------------------------------------------------------------------*/
static VoidFuncVoid UART485CallBackFunc = NULL;
/* ��չ���� ------------------------------------------------------------------*/
uint8_t RxBuffer;
/* ˽�к���ԭ�� --------------------------------------------------------------*/
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
//����_sys_exit()�Ա���ʹ�ð�����ģʽ 	
void _sys_exit(int x) 
{ 
	x = x; 
}
//�ض���fputc���� 
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
/* ������ --------------------------------------------------------------------*/ 
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
  * ��������: ���ڽ�����ɻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
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




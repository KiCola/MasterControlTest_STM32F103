#ifndef __bsp_hal_uart485_H__
#define __bsp_hal_uart485_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "mytype.h"


/* ���Ͷ��� ------------------------------------------------------------------*/

/* �궨�� --------------------------------------------------------------------*/



#define  UART_EN_TX   HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET)   
#define  UART_EN_RX   HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET)

//#define PRINTF_ENABLE 1

/* ��չ���� ------------------------------------------------------------------*/
extern uint8_t RxBuffer;
/* �������� ------------------------------------------------------------------*/
void Bsp_UART485_SetHandler(VoidFuncVoid func);
void Bsp_UART485_Transmit(uint8_t *pData, uint16_t Size);





#endif  // __bsp_hal_uart485_H__


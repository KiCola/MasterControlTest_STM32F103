#ifndef __bsp_hal_can_H__
#define __bsp_hal_can_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "mytype.h"


/* 宏定义 --------------------------------------------------------------------*/
#ifndef  FALSE
    #define  FALSE    0
#endif
#ifndef  TRUE
    #define  TRUE     1
#endif

#define CAN_QUEUE_NUM 20
/* 类型定义 ------------------------------------------------------------------*/
typedef struct
{
        uint8_t  IDE;
        uint8_t  RTR;
        uint8_t  DLC;
        uint8_t  Data[8];
        uint32_t Id;
}CanRxMemberType; //can接收成员结构




/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void Bsp_Cancom_ConfigurationInit(void);
void Bsp_Cancom_FilterInit(void);
uint32_t Bsp_Cancom_ReadERR(void);
uint8_t CANSend(uint32_t Source,
	               uint32_t IDE,
                   uint32_t RTR,
                   uint8_t* Datum,
                   uint8_t DataLength);
uint8_t CAN_OutQueue(CanRxMemberType * pVal);
void QueueOverflow_Dispose(CanRxMemberType *pQ);


#endif  // __bsp_hal_can_H__


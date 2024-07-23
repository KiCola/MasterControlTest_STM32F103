#ifndef __SLAVE_SteeringEngine_H__
#define __SLAVE_SteeringEngine_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stdint.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef struct SteeringEngine_T
{    
    uint8_t  CAN_Number;

    union 
    {
        #define PG_RXSTEERINGENGINE_SERIAL_NUMBER_LEN 1
        uint8_t Datum[PG_RXSTEERINGENGINE_SERIAL_NUMBER_LEN];
        struct{
            uint8_t Serial_Number      ; 
        }DATE;
    }SteeringEngine_SerialNumber;//序号修改 
    
    union
    {  
        #define PG_TXSTEERINGENGINEDATA_LEN 3
        uint8_t Datum[PG_TXSTEERINGENGINEDATA_LEN];
        struct{
            uint8_t  SpeedFac;
            uint16_t Fault;
        }DATE;
    }SteeringEngine_UploadData;
    
    union 
    {  
        #define PG_RXSTEERINGENGINEDATA_LEN 7
        uint8_t Datum[PG_RXSTEERINGENGINEDATA_LEN];
        struct{
            uint8_t  mode1;
            int8_t   SpeedAndAngle1;
            uint8_t  mode2;
            int8_t   SpeedAndAngle2;
            uint8_t  mode3;
            int8_t   SpeedAndAngle3;
            uint8_t  MemoryFlg;
        }DATE;
    }SteeringEngine_ControlData;

    struct SteeringEngine_T* next;
} SteeringEngineDef_t;
/* 宏定义 --------------------------------------------------------------------*/
#define SLAVE_SteeringEngine_ENABLED 1
#define SteeringEnginePlate_Addr     4  //舵机板地址

#define SteeringEngine_MOTOR_1 0
#define SteeringEngine_MOTOR_2 1
#define SteeringEngine_MOTOR_3 2

#define SteeringEngine_MODE_Angle 0
#define SteeringEngine_MODE_Speed 1


/* 扩展变量 ------------------------------------------------------------------*/

/* 函数声明 ------------------------------------------------------------------*/

uint8_t SLAVE_SteeringEngine_Init(SteeringEngineDef_t* handle,uint8_t list);
void SLAVE_SteeringEngine_NumberSet(SteeringEngineDef_t* handle,uint8_t number);
void SLAVE_SteeringEngine_MotorControl(SteeringEngineDef_t* handle,uint8_t motor,uint8_t mode,int8_t val); //这里虽然修改一个舵机的值，但是发送是发3个舵机的值
void SLAVE_SteeringEngine_MoreMotorControl(SteeringEngineDef_t* handle,
                                           uint8_t mode1,int8_t val1,
                                           uint8_t mode2,int8_t val2,
                                           uint8_t mode3,int8_t val3
                                          );
void SLAVE_SteeringEngine_MovementMemory(SteeringEngineDef_t* handle);



#endif  // __SLAVE_SteeringEngine_H__


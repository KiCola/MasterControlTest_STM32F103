#ifndef __SLAVE_DCMotorMiniwatt_H__
#define __SLAVE_DCMotorMiniwatt_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stdint.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef struct DCMotorMiniwatt_T
{    
    uint8_t  CAN_Number;
    
    union 
    {
        #define PG_RXMOTOR_SERIAL_NUMBER_LEN 2
        uint8_t Datum[PG_RXMOTOR_SERIAL_NUMBER_LEN];
        struct{
            uint16_t Serial_Number      ; //serial number
        }DATE;
    }Motor_SerialNumber;//序号修改 
    
    union
    {
        #define PG_TXMOTORDATA_LEN 8
        uint8_t Datum[PG_TXMOTORDATA_LEN];
        struct
        {
            uint16_t Speed;
            uint16_t EncoderNum; //V1.0.3版本上才有此数据
            uint16_t Coded_Value;
            uint16_t Fault;
        }DATE;
    }Motor_UploadData;//电机反馈数据 
    
    union
    {
        #define PG_RXMOTORDATA_LEN 8
        uint8_t Datum[PG_RXMOTORDATA_LEN];
        struct
        {
            uint16_t SetSpeed                   ; //Motor Set Speed
            int16_t  PwmSet                     ; //Motor Set PWM
            uint16_t Retain2                    ; //Retain2
            uint16_t Retain3                    ; //Retain3
        }DATE; 
    }Motor_ControlData;//电机控制数据
    
    struct DCMotorMiniwatt_T* next;
} DCMotorMiniwattDef_t;
/* 宏定义 --------------------------------------------------------------------*/
#define SLAVE_DCMotorMiniwatt_ENABLED 1
#define MotorPlate_Addr     2  //电机控制板地址




/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/


uint8_t SLAVE_DCMotorMiniwatt_Init(DCMotorMiniwattDef_t* handle,uint8_t list);     //数据初始化
void SLAVE_DCMotorMiniwatt_NumberSet(DCMotorMiniwattDef_t* handle,uint8_t number); //序列号设置（目前修改后不能直接连接，须修改程序，重新上电）
uint16_t SLAVE_DCMotorMiniwatt_PlaceRead(DCMotorMiniwattDef_t* handle);            //编码值读取（目前不是正交编码信号）
int16_t SLAVE_DCMotorMiniwatt_SpeedRead(DCMotorMiniwattDef_t* handle);             //速度读取
void SLAVE_DCMotorMiniwatt_SpeedSet(DCMotorMiniwattDef_t* handle,int16_t val);    //速度设置    
void SLAVE_DCMotorMiniwatt_PWMSet(DCMotorMiniwattDef_t* handle,int16_t val);       //PWM设置（速度反馈不能看方向，忽略符号）

#endif  // __SLAVE_DCMotorMiniwatt_H__


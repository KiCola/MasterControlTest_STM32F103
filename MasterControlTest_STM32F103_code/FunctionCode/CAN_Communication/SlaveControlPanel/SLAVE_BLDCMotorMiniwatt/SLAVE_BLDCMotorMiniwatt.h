#ifndef __SLAVE_BLDCMotorMiniwatt_H__
#define __SLAVE_BLDCMotorMiniwatt_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stdint.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef struct BLDCMotorMiniwatt_T
{    
    uint8_t  CAN_Number;
    
    union 
    {
        #define PG_RXBLMOTOR_SERIAL_NUMBER_LEN 1
        uint8_t Datum[PG_RXBLMOTOR_SERIAL_NUMBER_LEN];
        struct{
            uint8_t Serial_Number      ; //serial number
        }DATE;
    }Motor_SerialNumber;//序号修改 
    
    union
    {
        #define PG_TXBLMOTORDATA_LEN 6
        uint8_t Datum[PG_TXBLMOTORDATA_LEN];
        struct
        {
            int16_t Speed;
            uint16_t Coded_Value;
            uint16_t Fault;
        }DATE;
    }Motor_UploadData;//电机反馈数据 
    
    union
    {
        #define PG_RXBLMOTORDATA_LEN 8
        uint8_t Datum[PG_RXBLMOTORDATA_LEN];
        struct
        {
            int16_t  SetSpeed                   ; //Motor Set Speed
            int16_t  PwmSet                     ; //Motor Set PWM
            uint16_t Retain2                    ; //Retain2
            uint16_t Retain3                    ; //Retain3
        }DATE; 
    }Motor_ControlData;//电机控制数据
    
    struct BLDCMotorMiniwatt_T* next;
} BLDCMotorMiniwattDef_t;
/* 宏定义 --------------------------------------------------------------------*/
#define SLAVE_BLDCMotorMiniwatt_ENABLED 1
#define BLDCMotorPlate_Addr     9  //电机控制板地址




/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/


uint8_t SLAVE_BLDCMotorMiniwatt_Init(BLDCMotorMiniwattDef_t* handle,uint8_t list);     //数据初始化
void SLAVE_BLDCMotorMiniwatt_NumberSet(BLDCMotorMiniwattDef_t* handle,uint8_t number); //序列号设置（目前修改后不能直接连接，须修改程序，重新上电）
uint16_t SLAVE_BLDCMotorMiniwatt_PlaceRead(BLDCMotorMiniwattDef_t* handle);            //编码值读取（目前不是正交编码信号）
int16_t SLAVE_BLDCMotorMiniwatt_SpeedRead(BLDCMotorMiniwattDef_t* handle);             //速度读取
void SLAVE_BLDCMotorMiniwatt_SpeedSet(BLDCMotorMiniwattDef_t* handle,int16_t val);    //速度设置    
void SLAVE_BLDCMotorMiniwatt_PWMSet(BLDCMotorMiniwattDef_t* handle,int16_t val);       //PWM设置（速度反馈不能看方向，忽略符号）

#endif  // __SLAVE_BLDCMotorMiniwatt_H__


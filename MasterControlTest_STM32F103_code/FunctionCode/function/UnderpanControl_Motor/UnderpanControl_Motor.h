#ifndef __UnderpanControl_Motor_H__
#define __UnderpanControl_Motor_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stdint.h"
#include "string.h"
#include "SLAVE_DCMotorMiniwatt/SLAVE_DCMotorMiniwatt.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
	DEBUG_COMMONWHEEL = 0,             //调试模式
	FOUR_DRIVE_COMMONWHEEL,            //4个普通胶轮模式
	FOUR_DRIVE_McNamara,               //麦克纳姆轮
	FOUR_DRIVE_OMNIDIRECTIONALWHEEL,   //四轮45度安装式全向轮 
	THREE_DRIVE_OMNIDIRECTIONALWHEEL,  //三式全向轮   
	TWO_DRIVE_COMMONWHEEL,             //2个普通胶轮模式
	
}UnderpanControlMode_e;

typedef struct UnderpanPosture_T
{
    UnderpanControlMode_e mode;
    int16_t x_axis;
    int16_t y_axis;
    int16_t yaw;
    
    int16_t xnow_axis;
    int16_t ynow_axis;
    int16_t yawnow;
    /*******************************
	 车子轮子设定：头
				 2    1
				 3    4
	********************************/
    int16_t speed1;
    int16_t speed2;
    int16_t speed3;
    int16_t speed4;
    uint16_t MaxLinearSpeed;
    
    int16_t speed1now;
    int16_t speed2now;
    int16_t speed3now;
    int16_t speed4now;
    
	uint16_t yawfac;
    DCMotorMiniwattDef_t *Motor1;
    DCMotorMiniwattDef_t *Motor2;
    DCMotorMiniwattDef_t *Motor3;
    DCMotorMiniwattDef_t *Motor4;
}UnderpanPostureDef_t;

/* 宏定义 --------------------------------------------------------------------*/
#define WHEEL_DIS    200.0f   
#define AXLE_DIS     150.0f
/* 扩展变量 ------------------------------------------------------------------*/



/* 函数声明 ------------------------------------------------------------------*/
void UnderpanControl_Init(  UnderpanPostureDef_t* handle,
                            UnderpanControlMode_e mode,
                            uint16_t Maxlinearspeed,
                            DCMotorMiniwattDef_t *Motor1,
                            DCMotorMiniwattDef_t *Motor2,
                            DCMotorMiniwattDef_t *Motor3,
                            DCMotorMiniwattDef_t *Motor4
                         );
void UnderpanControl_Scan(UnderpanPostureDef_t* handle);                                      











#endif  // __UnderpanControl_Motor_H__


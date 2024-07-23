#ifndef __UnderpanControl_LinePatrol_H__
#define __UnderpanControl_LinePatrol_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stdint.h"
#include "string.h"
#include "mytype.h"
/* 类型定义 ------------------------------------------------------------------*/
typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t  ls8 :1;
        uint8_t  ls7 :1;
        uint8_t  ls6 :1;
        uint8_t  ls5 :1;
        uint8_t  ls4 :1;
        uint8_t  ls3 :1;
        uint8_t  ls2 :1;
        uint8_t  ls1 :1;
    }bit;
}SignalDef_u;

//typedef union
//{
//    uint8_t byte;
//    struct
//    {
//        uint8_t  ls1 :1;
//        uint8_t  ls2 :1;
//        uint8_t  ls3 :1;
//        uint8_t  ls4 :1;
//        uint8_t  ls5 :1;
//        uint8_t  ls6 :1;
//        uint8_t  ls7 :1;
//        uint8_t  ls8 :1;
//    }bit;
//}SignalDef_u;

typedef enum 
{
    Mode_Stop=0,         //停止
    Mode_Await,          //等待
    Mode_ArticlePatrol,  //线条巡线
    Mode_DelayTracking,  //延时巡线
	Mode_EncoderTracking,//编码器巡线
	Mode_StateTracking,  //条件巡线
	Mode_SituAdjust,     //原地矫正
    Mode_Wheel_90,       //90度转弯
    DelayOpenLoopWalk,   //延时开环控制
} ModeDef_e;
typedef enum
{
	CarDirection_Head = 0,  
	CarDirection_Tail,
	CarDirection_Left,
	CarDirection_Right,
}DirectionDef_e;

typedef struct
{
	uint8_t  sta;   //状态
	uint8_t  line;  //目标条数
	DirectionDef_e Car_Direction;
	uint16_t SetVal;  //设置存储器
	uint16_t TimeMem;  //时间存储器
	uint16_t  y_axis_set;//y设置值
	uint16_t  x_axis_set;//x设置值
    int16_t   yaw_set;   //旋转设置值
	uint16_t  *CodedValue_q; //编码器读取数据指针
	uint16_t (*ReadData_func)(void);//数据读取函数指针
}LinePatrolDef;



typedef struct LinePatrol_T
{ 
	uint8_t  active_level; //信号有效电平储存器
	uint8_t  c;     //计数器
	ModeDef_e mode;
	uint16_t Time;     //运行时间
	int16_t  y_axis_out;//y输出值
	int16_t  x_axis_out;//x输出值
    int16_t  yaw_out;   //旋转输出值
	int16_t y_axis_Practicalout;//Y实际输出值
	int16_t x_axis_Practicalout;//X实际输出值
    int16_t yaw_Practicalout;   //旋转实际输出值 
    int16_t L1;
    int16_t L2;	
	float D_Offs;
	float Alpha;
    SignalDef_u *HeadSignal;    //相对方向头信号
	SignalDef_u *Signal1;       //信号1
    SignalDef_u *Signal2;       //信号2
	SignalDef_u *Signal3;       //信号3
    SignalDef_u *Signal4;       //信号4
	LinePatrolDef *TrackingpQ;
}LinePatrolDef_t;

/* 宏定义 --------------------------------------------------------------------*/
#define LINE_PATROL_SPEED_MAX 400 //最大速度
#define LINE_PATROL_WIRE_REFERENCE  4 //横线条件
#define LINE_PATROL_SPEED_START 100     //起步速度
#define LINE_PATROL_SPEED_STOP  80     //止步速度
#define LINE_PATROL_SPEED_WHEEL_FORWARD 50//转弯时前进速度
#define SPEED_CTRL_ADD_FAC          1     //速度控制系数  a=((a*(100-10))+(b*10))/100
#define SPEED_CTRL_ADD_TIME         3     //加速度控制时间
#define SPEED_CTRL_MIN_TIME         2     //减速度控制时间
#define SPEED_CTRL_MIN_FAC          50     //速度控制系数 
#define RL_DISTANCE_FAC         8      //2个光敏电阻之间距离的一半
#define RL_LENGTH_FAC           56
#define B_LENGTH                200.000
#define A_LENGTH                200.000
#define WHEEL_D 76  //轮子直径，单位mm
#define CALC_C  1242 //计算系数，(电机一圈脉冲数*10/3.14)  (253.5*10/3.14)

#define X_FDBCK_P    2.5f
#define X_FDBCK_I    0.01f
#define X_FDBCK_D    0.1f

#define YAW_FDBCK_P  2.2f
#define YAW_FDBCK_I  0.0f
#define YAW_FDBCK_D  0.05f

#define LINE_PATROL_DELAY(x) HAL_Delay(x)  

/* 扩展变量 ------------------------------------------------------------------*/
extern LinePatrolDef_t LinePatrolData_S;


/* 函数声明 ------------------------------------------------------------------*/
void LinePatrol_Init( uint8_t active_level,
	                  uint8_t *Signal1q,
				 	  uint8_t *Signal2q,
				 	  uint8_t *Signal3q,
					  uint8_t *Signal4q);
void LinePatrol_Scan(void); 
void Execute_LinePatrol(DirectionDef_e Car_Direction,uint16_t spd, uint8_t wirenum,uint8_t mode);
void Execute_DelayTracking(DirectionDef_e Car_Direction,uint16_t spd, uint16_t time);
void Execute_EncoderTracking(DirectionDef_e Car_Direction,uint16_t spd,uint16_t millimeter);
void Execute_SituAdjust(uint16_t OutTime);
void Execute_Wheel_90(DirectionDef_e Car_Direction,int16_t spd, uint16_t time);
void ExecuteOpenLoop_DelayWalk(DirectionDef_e Car_Direction,
							   uint16_t y_val,
							   uint16_t x_val,
							   int16_t  yaw_val,
							   uint16_t time);
void WaitForTheCarToStop(void);




#endif  // __UnderpanControl_LinePatrol_H__


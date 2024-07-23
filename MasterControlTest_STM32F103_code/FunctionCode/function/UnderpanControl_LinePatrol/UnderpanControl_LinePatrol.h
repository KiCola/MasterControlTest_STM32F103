#ifndef __UnderpanControl_LinePatrol_H__
#define __UnderpanControl_LinePatrol_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stdint.h"
#include "string.h"
#include "mytype.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
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
    Mode_Stop=0,         //ֹͣ
    Mode_Await,          //�ȴ�
    Mode_ArticlePatrol,  //����Ѳ��
    Mode_DelayTracking,  //��ʱѲ��
	Mode_EncoderTracking,//������Ѳ��
	Mode_StateTracking,  //����Ѳ��
	Mode_SituAdjust,     //ԭ�ؽ���
    Mode_Wheel_90,       //90��ת��
    DelayOpenLoopWalk,   //��ʱ��������
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
	uint8_t  sta;   //״̬
	uint8_t  line;  //Ŀ������
	DirectionDef_e Car_Direction;
	uint16_t SetVal;  //���ô洢��
	uint16_t TimeMem;  //ʱ��洢��
	uint16_t  y_axis_set;//y����ֵ
	uint16_t  x_axis_set;//x����ֵ
    int16_t   yaw_set;   //��ת����ֵ
	uint16_t  *CodedValue_q; //��������ȡ����ָ��
	uint16_t (*ReadData_func)(void);//���ݶ�ȡ����ָ��
}LinePatrolDef;



typedef struct LinePatrol_T
{ 
	uint8_t  active_level; //�ź���Ч��ƽ������
	uint8_t  c;     //������
	ModeDef_e mode;
	uint16_t Time;     //����ʱ��
	int16_t  y_axis_out;//y���ֵ
	int16_t  x_axis_out;//x���ֵ
    int16_t  yaw_out;   //��ת���ֵ
	int16_t y_axis_Practicalout;//Yʵ�����ֵ
	int16_t x_axis_Practicalout;//Xʵ�����ֵ
    int16_t yaw_Practicalout;   //��תʵ�����ֵ 
    int16_t L1;
    int16_t L2;	
	float D_Offs;
	float Alpha;
    SignalDef_u *HeadSignal;    //��Է���ͷ�ź�
	SignalDef_u *Signal1;       //�ź�1
    SignalDef_u *Signal2;       //�ź�2
	SignalDef_u *Signal3;       //�ź�3
    SignalDef_u *Signal4;       //�ź�4
	LinePatrolDef *TrackingpQ;
}LinePatrolDef_t;

/* �궨�� --------------------------------------------------------------------*/
#define LINE_PATROL_SPEED_MAX 400 //����ٶ�
#define LINE_PATROL_WIRE_REFERENCE  4 //��������
#define LINE_PATROL_SPEED_START 100     //���ٶ�
#define LINE_PATROL_SPEED_STOP  80     //ֹ���ٶ�
#define LINE_PATROL_SPEED_WHEEL_FORWARD 50//ת��ʱǰ���ٶ�
#define SPEED_CTRL_ADD_FAC          1     //�ٶȿ���ϵ��  a=((a*(100-10))+(b*10))/100
#define SPEED_CTRL_ADD_TIME         3     //���ٶȿ���ʱ��
#define SPEED_CTRL_MIN_TIME         2     //���ٶȿ���ʱ��
#define SPEED_CTRL_MIN_FAC          50     //�ٶȿ���ϵ�� 
#define RL_DISTANCE_FAC         8      //2����������֮������һ��
#define RL_LENGTH_FAC           56
#define B_LENGTH                200.000
#define A_LENGTH                200.000
#define WHEEL_D 76  //����ֱ������λmm
#define CALC_C  1242 //����ϵ����(���һȦ������*10/3.14)  (253.5*10/3.14)

#define X_FDBCK_P    2.5f
#define X_FDBCK_I    0.01f
#define X_FDBCK_D    0.1f

#define YAW_FDBCK_P  2.2f
#define YAW_FDBCK_I  0.0f
#define YAW_FDBCK_D  0.05f

#define LINE_PATROL_DELAY(x) HAL_Delay(x)  

/* ��չ���� ------------------------------------------------------------------*/
extern LinePatrolDef_t LinePatrolData_S;


/* �������� ------------------------------------------------------------------*/
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


/* 包含头文件 ----------------------------------------------------------------*/
#include "UnderpanControl_LinePatrol/UnderpanControl_LinePatrol.h"
#include "S_curve.h"
#include "SLAVE_DCMotorMiniwatt/SLAVE_DCMotorMiniwatt.h"
#include "math.h"
#include "pid.h" 
#include <stdlib.h>
/* 私有宏定义 ----------------------------------------------------------------*/
#define LINEPATROL_QUEUE_NUM 20
/* 私有类型定义 --------------------------------------------------------------*/
typedef struct
{
	uint8_t front;
    uint8_t rear;
	LinePatrolDef data[LINEPATROL_QUEUE_NUM];
}LinePateolQueueType;
/* 私有变量 ------------------------------------------------------------------*/
LinePatrolDef *LinePatrol_S = 0;
pid_t xpid;
pid_t yawpid;
static STEPPINGMOTOR_T ScurveDelay_s =
{
    Stop,          //算法状态
    Invariability, //速度增长方向   
    StepMode,      //工作模式
    0,             //步数
    0,             //步数缓存
    0,             //加加速度
    0,             //最大加加速度
    0,             //允许最大加速度
    0,             //起始速度
    0,             //输出速度
    0,             //目标速度
    0,             //加加速阶段起始速度
    0,             //加加速阶段目标速度
    0,             //匀加速阶段最大速度值
    0,             //最终速度 
    0,             //时钟计数
};
/* 扩展变量 ------------------------------------------------------------------*/
extern DCMotorMiniwattDef_t DCMotorMiniwatt1_S;
LinePatrolDef_t LinePatrolData_S;
/* 私有函数原形 --------------------------------------------------------------*/
/*****************************
*  <summary>
*  get current time;
*  <returns> current time tick
*****************************/
static uint8_t Tim_CheckTimeOut(uint16_t TimeStart, uint16_t TimeNow, uint16_t TimeOut){
  #define ActivateCounter ((uint16_t)(TimeOut+TimeStart))
  if (ActivateCounter >= TimeStart)
  {
    if ((TimeNow >= ActivateCounter) ||
        (TimeNow < TimeStart))
    {
      return 1;
    }
  }
  else if ((TimeNow >= ActivateCounter) &&
           (TimeNow < TimeStart))
  {
    return 1;
  }
  return 0;
}

/* 巡线函数 --------------------------------------------------------------*/
static uint16_t LinePatrol_DistanceCalc_(SignalDef_u *Signal,
	                                    uint8_t active_level) //此代码目前没有使用
{
	uint16_t Distance;
	
//    else if((Signal->bit.ls6 == active_level)
//	  &&(Signal->bit.ls3 == active_level)) Distance = 7;
	//================================================================
	if((Signal->bit.ls1 == active_level)
		  &&(Signal->bit.ls2 != active_level)) Distance = 0;
	else if((Signal->bit.ls8 == active_level)
		  &&(Signal->bit.ls7 != active_level)) Distance = 14;
	else if((Signal->bit.ls2 == active_level)
		  &&(Signal->bit.ls1 == active_level)) Distance = 1;
	else if((Signal->bit.ls7 == active_level)
		  &&(Signal->bit.ls8 == active_level)) Distance = 13;
	
	else if((Signal->bit.ls2 == active_level)
		  &&(Signal->bit.ls3 != active_level)) Distance = 2;
	else if((Signal->bit.ls7 == active_level)
		  &&(Signal->bit.ls6 != active_level)) Distance = 12;
	else if((Signal->bit.ls3 == active_level)
		  &&(Signal->bit.ls2 == active_level)) Distance = 3;
	else if((Signal->bit.ls6 == active_level)
		  &&(Signal->bit.ls7 == active_level)) Distance = 11;
	
	else if((Signal->bit.ls3 == active_level)
		  &&(Signal->bit.ls4 != active_level)) Distance = 4;
	else if((Signal->bit.ls6 == active_level)
		  &&(Signal->bit.ls5 != active_level)) Distance = 10;
	else if((Signal->bit.ls4 == active_level)
	      &&(Signal->bit.ls3 == active_level)) Distance = 5;
	else if((Signal->bit.ls5 == active_level)
		  &&(Signal->bit.ls6 == active_level)) Distance = 9;
	
	else if(Signal->bit.ls4 == active_level) Distance = 6;
	else if(Signal->bit.ls5 == active_level) Distance = 8;
	
	else if((Signal->bit.ls5 == active_level)
	 &&(Signal->bit.ls4 == active_level)) Distance = 7;	
	//================================================================

	return (RL_DISTANCE_FAC*Distance);
}
static uint16_t LinePatrol_DistanceCalc(SignalDef_u *Signal,
	                                    uint8_t active_level)
{
	uint16_t Distance;
	
	if((Signal->bit.ls5 == active_level)
	 &&(Signal->bit.ls4 == active_level)) Distance = 7;	
    else if((Signal->bit.ls6 == active_level)
	  &&(Signal->bit.ls3 == active_level)) Distance = 7;
	//================================================================
	else if((Signal->bit.ls1 == active_level)
		  &&(Signal->bit.ls2 != active_level)) Distance = 0;
	else if((Signal->bit.ls8 == active_level)
		  &&(Signal->bit.ls7 != active_level)) Distance = 14;
	else if((Signal->bit.ls2 == active_level)
		  &&(Signal->bit.ls1 == active_level)) Distance = 1;
	else if((Signal->bit.ls7 == active_level)
		  &&(Signal->bit.ls8 == active_level)) Distance = 13;
	
	else if((Signal->bit.ls2 == active_level)
		  &&(Signal->bit.ls3 != active_level)) Distance = 2;
	else if((Signal->bit.ls7 == active_level)
		  &&(Signal->bit.ls6 != active_level)) Distance = 12;
	else if((Signal->bit.ls3 == active_level)
		  &&(Signal->bit.ls2 == active_level)) Distance = 3;
	else if((Signal->bit.ls6 == active_level)
		  &&(Signal->bit.ls7 == active_level)) Distance = 11;
	
	else if((Signal->bit.ls3 == active_level)
		  &&(Signal->bit.ls4 != active_level)) Distance = 4;
	else if((Signal->bit.ls6 == active_level)
		  &&(Signal->bit.ls5 != active_level)) Distance = 10;
	else if((Signal->bit.ls4 == active_level)
	      &&(Signal->bit.ls3 == active_level)) Distance = 5;
	else if((Signal->bit.ls5 == active_level)
		  &&(Signal->bit.ls6 == active_level)) Distance = 9;
	
	else if(Signal->bit.ls4 == active_level) Distance = 6;
	else if(Signal->bit.ls5 == active_level) Distance = 8;
	//================================================================

	return (RL_DISTANCE_FAC*Distance);
}
static void LinePatrol_ChassisPostureCalc(SignalDef_u *HeadSignal,   
										  SignalDef_u *TailSignal){
	double 	Alpha;	
											    
    if((HeadSignal->byte != 0x00)&&(HeadSignal->byte != 0xff))
	{		
		LinePatrolData_S.L1 = RL_LENGTH_FAC*2-LinePatrol_DistanceCalc(HeadSignal,LinePatrolData_S.active_level);
	}
	if((TailSignal->byte != 0x00)&&(TailSignal->byte != 0xff))
	{
		LinePatrolData_S.L2 = LinePatrol_DistanceCalc(TailSignal,LinePatrolData_S.active_level);
    }
	LinePatrolData_S.D_Offs = RL_LENGTH_FAC-((LinePatrolData_S.L1+LinePatrolData_S.L2)/2);	
	if(LinePatrolData_S.TrackingpQ->Car_Direction < CarDirection_Left)										  
		Alpha = atan((LinePatrolData_S.L1-LinePatrolData_S.L2)/B_LENGTH)*57.3f;
    else 
        Alpha = atan((LinePatrolData_S.L1-LinePatrolData_S.L2)/A_LENGTH)*57.3f;		
	LinePatrolData_S.Alpha	= Alpha;											  
}
static void TrackCorrectiveCtrl(uint8_t mode)
{
    int16_t xval,yval;
	int16_t yawval1,yawval2;
	if(mode == 0)
	{
		//================循迹纠正=====================
		if((LinePatrolData_S.Signal1 != 0)
		 &&(LinePatrolData_S.Signal3 != 0)
		 &&(LinePatrolData_S.TrackingpQ->Car_Direction < CarDirection_Left))
		{
			LinePatrol_ChassisPostureCalc(LinePatrolData_S.Signal1,LinePatrolData_S.Signal3);
		}
		else if((LinePatrolData_S.Signal2 != 0)
			  &&(LinePatrolData_S.Signal4 != 0))
		{
			LinePatrol_ChassisPostureCalc(LinePatrolData_S.Signal2,LinePatrolData_S.Signal4);
		}
		LinePatrolData_S.x_axis_out = pid_calc(&xpid, LinePatrolData_S.D_Offs, 0.0f);	
		LinePatrolData_S.yaw_out = pid_calc(&yawpid, LinePatrolData_S.Alpha, 0.0f);	
	}
	else    //原地十字纠正
	{
		if((LinePatrolData_S.Signal1 != 0)
		 &&(LinePatrolData_S.Signal3 != 0)
		 &&(LinePatrolData_S.Signal2 != 0)
		 &&(LinePatrolData_S.Signal4 != 0))
		{
			yawpid.f_pid_reset(&yawpid,1.0f,0.01f,0.0f);
			xpid.f_pid_reset(&xpid,1.5f,0.0f,0.1f);
			LinePatrol_ChassisPostureCalc(LinePatrolData_S.Signal1,LinePatrolData_S.Signal3);
			xval = pid_calc(&xpid, LinePatrolData_S.D_Offs, 0.0f);
			yawval1 = pid_calc(&yawpid, LinePatrolData_S.Alpha, 0.0f);
			
			LinePatrol_ChassisPostureCalc(LinePatrolData_S.Signal2,LinePatrolData_S.Signal4);
			xpid.f_pid_reset(&xpid,1.2f,0.0f,0.0f);
			yval = pid_calc(&xpid, LinePatrolData_S.D_Offs, 0.0f);	
			yawval2 = pid_calc(&yawpid, LinePatrolData_S.Alpha, 0.0f);
			
			LinePatrolData_S.yaw_out = yawval1+yawval2;
			LinePatrolData_S.x_axis_out = xval;
			LinePatrolData_S.y_axis_out = yval;				
		}
	}
}

static void LinePatrol_ArticlePatrol(void) 
{
	uint8_t active_level;
	static uint8_t LineSta = 0; 
	static uint16_t y_axis_val;
	static uint8_t sdtime = 0;
    uint8_t num;
	SignalDef_u *Signalq;
	
	if(LinePatrolData_S.TrackingpQ != 0)
	{
		//=================加减速======================
		switch(LinePatrolData_S.TrackingpQ->sta)
		{
			case 0:
				if(LinePatrolData_S.TrackingpQ->y_axis_set < LINE_PATROL_SPEED_START)
					y_axis_val = LinePatrolData_S.TrackingpQ->y_axis_set;
				else
					y_axis_val = LINE_PATROL_SPEED_START;
			    LinePatrolData_S.TrackingpQ->sta = 1;
				break;
			case 1:
				if(sdtime >= SPEED_CTRL_ADD_TIME)
				{
					sdtime = 0;
					y_axis_val = ((y_axis_val*(100-SPEED_CTRL_ADD_FAC))
								 +(LinePatrolData_S.TrackingpQ->y_axis_set*SPEED_CTRL_ADD_FAC))/100;
				}
				break;
			case 2:
				if((sdtime >= SPEED_CTRL_MIN_TIME)&&(y_axis_val != LINE_PATROL_SPEED_STOP))
				{
					sdtime = 0;
					if(y_axis_val >= (LINE_PATROL_SPEED_STOP+SPEED_CTRL_MIN_FAC))
					{
						y_axis_val -= SPEED_CTRL_MIN_FAC;
					}
					else
					{
						y_axis_val = LINE_PATROL_SPEED_STOP;
					}
				}					
				break;
			default:
				y_axis_val = LinePatrolData_S.TrackingpQ->y_axis_set;
				break;
		}
		sdtime++;
		//================循迹纠正=====================
		TrackCorrectiveCtrl(0);
		//================循迹读横线=======================
		active_level = LinePatrolData_S.active_level;
		num = 0;
		if(LinePatrolData_S.HeadSignal->bit.ls1 == active_level) num++;	
		if(LinePatrolData_S.HeadSignal->bit.ls2 == active_level) num++;
		if(LinePatrolData_S.HeadSignal->bit.ls3 == active_level) num++;
		if(LinePatrolData_S.HeadSignal->bit.ls4 == active_level) num++;
		if(LinePatrolData_S.HeadSignal->bit.ls5 == active_level) num++;
		if(LinePatrolData_S.HeadSignal->bit.ls6 == active_level) num++;
		if(LinePatrolData_S.HeadSignal->bit.ls7 == active_level) num++;
		if(LinePatrolData_S.HeadSignal->bit.ls8 == active_level) num++;
		if((num >= LINE_PATROL_WIRE_REFERENCE)//巡到一条线
		// &&((LinePatrolData_S.HeadSignal->bit.ls1 == active_level)||(LinePatrolData_S.HeadSignal->bit.ls8 == active_level))
		 &&(0 == LineSta)) 
		{
			LinePatrolData_S.c++;
			LinePatrolData_S.TrackingpQ->TimeMem = 0;
//			LinePatrolData_S.TrackingpQ->TimeMem = LinePatrolData_S.Time;
			LineSta = 1;//在线上面
			if((LinePatrolData_S.c == (LinePatrolData_S.TrackingpQ->line-1))
			||((LinePatrolData_S.c == LinePatrolData_S.TrackingpQ->line) && (1 == LinePatrolData_S.TrackingpQ->line)))
			{
				LinePatrolData_S.TrackingpQ->sta = 2;
			}
			else if((LinePatrolData_S.c == (LinePatrolData_S.TrackingpQ->line-2)) 
				 && (LinePatrolData_S.TrackingpQ->line > 3)
			     &&(y_axis_val > 350))
			{
				y_axis_val = 350;
				LinePatrolData_S.TrackingpQ->sta = 2;
			}
			if(LinePatrolData_S.TrackingpQ->SetVal == 0)
			{
				if(LinePatrolData_S.c == LinePatrolData_S.TrackingpQ->line)
				{
					LinePatrolData_S.mode = Mode_Await;
					LinePatrolData_S.c = 0;
					LinePatrolData_S.TrackingpQ->y_axis_set = 0;
					LinePatrolData_S.TrackingpQ->x_axis_set = 0;
					LinePatrolData_S.TrackingpQ->yaw_set = 0;
					LinePatrolData_S.y_axis_out = 0;
					LinePatrolData_S.x_axis_out = 0;
					LinePatrolData_S.yaw_out = 0;
					LineSta = 0;
					y_axis_val = 0;
					sdtime = 0;
				}
			}
		}
		else if(1 == LineSta)
		{
			if((num < LINE_PATROL_WIRE_REFERENCE))
			{
				LinePatrolData_S.TrackingpQ->TimeMem++;
				if(LinePatrolData_S.TrackingpQ->TimeMem > 5)
				{
					LinePatrolData_S.TrackingpQ->TimeMem = 0;
					if(LinePatrolData_S.c != LinePatrolData_S.TrackingpQ->line)
						LineSta = 2;
					else
						LineSta = 3;
				}
			}
			else
			{
				LinePatrolData_S.TrackingpQ->TimeMem = 0;
			}
		}
		if(2 == LineSta)
		{
//			if(LinePatrolData_S.TrackingpQ->Car_Direction < CarDirection_Left)
//			{
//				if((LinePatrolData_S.Signal2->byte != 0)
//				 ||(LinePatrolData_S.Signal4->byte != 0))
//				{
//					LineSta = 0;
//				}
//			}
//			else
//			{
//				if((LinePatrolData_S.Signal1->byte != 0)
//				 ||(LinePatrolData_S.Signal3->byte != 0))
//				{
//					LineSta = 0;
//				}
//			}
			
			if(LinePatrolData_S.TrackingpQ->Car_Direction == CarDirection_Head)      
				Signalq = LinePatrolData_S.Signal3;      
			else if(LinePatrolData_S.TrackingpQ->Car_Direction == CarDirection_Tail)
				Signalq = LinePatrolData_S.Signal1;
			else if(LinePatrolData_S.TrackingpQ->Car_Direction == CarDirection_Left)
				Signalq = LinePatrolData_S.Signal4;
			else if(LinePatrolData_S.TrackingpQ->Car_Direction == CarDirection_Right)
				Signalq = LinePatrolData_S.Signal2;
			num = 0;
			if(Signalq->bit.ls1 == active_level) num++;	
			if(Signalq->bit.ls2 == active_level) num++;
			if(Signalq->bit.ls3 == active_level) num++;
			if(Signalq->bit.ls4 == active_level) num++;
			if(Signalq->bit.ls5 == active_level) num++;
			if(Signalq->bit.ls6 == active_level) num++;
			if(Signalq->bit.ls7 == active_level) num++;
			if(Signalq->bit.ls8 == active_level) num++;
			if(num >= LINE_PATROL_WIRE_REFERENCE)
			{
				LineSta = 0;
			}
		}
		else if(3 == LineSta)
		{
//			if(LinePatrolData_S.TrackingpQ->Car_Direction < CarDirection_Left)
//			{
//				if(((LinePatrolData_S.Signal2->byte&0x3c) != 0)
//				 &&((LinePatrolData_S.Signal4->byte&0x3c) != 0))
//				{
//					LineSta = 0;
//				}
//			}
//			else
//			{
//				if(((LinePatrolData_S.Signal1->byte&0x3c) != 0)
//				 &&((LinePatrolData_S.Signal3->byte&0x3c) != 0))
//				{
//					LineSta = 0;
//				}
//			}
			if(LinePatrolData_S.TrackingpQ->Car_Direction < CarDirection_Left)
			{
				if((LinePatrolData_S.Signal2->bit.ls4 == active_level)
				 ||(LinePatrolData_S.Signal4->bit.ls4 == active_level)
				 ||(LinePatrolData_S.Signal2->bit.ls5 == active_level)
				 ||(LinePatrolData_S.Signal4->bit.ls5 == active_level)
//				 ||(LinePatrolData_S.Signal2->bit.ls3 == active_level)
//				 ||(LinePatrolData_S.Signal4->bit.ls3 == active_level)
//				 ||(LinePatrolData_S.Signal2->bit.ls6 == active_level)
//				 ||(LinePatrolData_S.Signal4->bit.ls6 == active_level)
				)
				{
					LineSta = 0;
				}
			}
			else
			{
				if((LinePatrolData_S.Signal1->bit.ls4 == active_level)
				 ||(LinePatrolData_S.Signal3->bit.ls4 == active_level)
				 ||(LinePatrolData_S.Signal1->bit.ls5 == active_level)
				 ||(LinePatrolData_S.Signal3->bit.ls5 == active_level)
//				 ||(LinePatrolData_S.Signal1->bit.ls3 == active_level)
//				 ||(LinePatrolData_S.Signal3->bit.ls3 == active_level)
//				 ||(LinePatrolData_S.Signal1->bit.ls6 == active_level)
//				 ||(LinePatrolData_S.Signal3->bit.ls6 == active_level)
				)
				{
					LineSta = 0;
				}
			}
			if(0 == LineSta)
			{
				LinePatrolData_S.mode = Mode_Await;
				LinePatrolData_S.c = 0;
				LinePatrolData_S.TrackingpQ->y_axis_set = 0;
				LinePatrolData_S.TrackingpQ->x_axis_set = 0;
				LinePatrolData_S.TrackingpQ->yaw_set = 0;
				LinePatrolData_S.y_axis_out = 0;
				LinePatrolData_S.x_axis_out = 0;
				LinePatrolData_S.yaw_out = 0;
				LineSta = 0;
				y_axis_val = 0;
				sdtime = 0;
			}
		}
		if(LineSta == 1)
		{
			LinePatrolData_S.x_axis_out = 0;
			LinePatrolData_S.yaw_out = 0;
		}
		LinePatrolData_S.y_axis_out = y_axis_val;
	}
	else
	{
		LinePatrolData_S.mode = Mode_Await;
		LinePatrolData_S.c = 0;
		LinePatrolData_S.TrackingpQ->y_axis_set = 0;
		LinePatrolData_S.TrackingpQ->x_axis_set = 0;
		LinePatrolData_S.TrackingpQ->yaw_set = 0;
		LinePatrolData_S.y_axis_out = 0;
		LinePatrolData_S.x_axis_out = 0;
		LinePatrolData_S.yaw_out = 0;
		LineSta = 0;
		y_axis_val = 0;
		sdtime = 0;
	}
}
static void LinePatrol_DelayTracking(void) 
{
	if(LinePatrolData_S.TrackingpQ != 0)
	{
		if(0 == LinePatrolData_S.TrackingpQ->sta)
        {
            ScurveDelay_s.V_Target = LinePatrolData_S.TrackingpQ->y_axis_set*100;
            ScurveDelay_s.Vs = LINE_PATROL_SPEED_START*100;
            ScurveDelay_s.StepNum = LinePatrolData_S.TrackingpQ->SetVal;
            ScurveDelay_s.Sta = Stop;
            ScurveDelay_s.wordmode = StepMode;
            LinePatrolData_S.TrackingpQ->sta = 1;
        }
        else if(1 == LinePatrolData_S.TrackingpQ->sta)
        {
            /******************S型加减速计算********************/
            S_ModelChangeSpeed_Calc(&ScurveDelay_s);
            LinePatrolData_S.y_axis_out = ScurveDelay_s.V_Actual/100;
            if(Stop == ScurveDelay_s.Sta)
            {
				LinePatrolData_S.mode = Mode_Await;
				LinePatrolData_S.y_axis_out = 0;
				LinePatrolData_S.x_axis_out = 0;
				LinePatrolData_S.yaw_out = 0;
            }
            else
            {
                //================循迹纠正=====================
				TrackCorrectiveCtrl(0);
            }
        }
	}
	else
	{
		LinePatrolData_S.mode = Mode_Await;
		LinePatrolData_S.y_axis_out = 0;
		LinePatrolData_S.x_axis_out = 0;
		LinePatrolData_S.yaw_out = 0;
	}
}
static void LinePatrol_StateTracking(void) 
{
	static uint8_t sdtime = 0;
	static uint16_t y_axis_val = 0;
	
	if(LinePatrolData_S.TrackingpQ != 0)
	{
		if(0 == LinePatrolData_S.TrackingpQ->sta)
		{
			if(LINE_PATROL_SPEED_START < LinePatrolData_S.TrackingpQ->y_axis_set)
				y_axis_val = LINE_PATROL_SPEED_START;
			else
				y_axis_val = LinePatrolData_S.TrackingpQ->y_axis_set;
			LinePatrolData_S.TrackingpQ->sta = 1;
			sdtime = 0;
		}
		else if(LinePatrolData_S.y_axis_out != LinePatrolData_S.TrackingpQ->y_axis_set)
		{
			if(sdtime >= SPEED_CTRL_ADD_TIME)
			{
				sdtime = 0;
				y_axis_val = ((y_axis_val*(100-SPEED_CTRL_ADD_FAC))
							 +(LinePatrolData_S.TrackingpQ->y_axis_set*SPEED_CTRL_ADD_FAC))/100;
			}
			sdtime++;
		}
		if(LinePatrolData_S.TrackingpQ->ReadData_func() == LinePatrolData_S.TrackingpQ->SetVal)
		{
			LinePatrolData_S.mode = Mode_Await;
			LinePatrolData_S.y_axis_out = 0;
			LinePatrolData_S.x_axis_out = 0;
			LinePatrolData_S.yaw_out = 0;
		}
		else
		{
			LinePatrolData_S.y_axis_out = y_axis_val;
			//================循迹纠正=====================
			TrackCorrectiveCtrl(0);
		}
	}
	else
	{
		LinePatrolData_S.mode = Mode_Await;
		LinePatrolData_S.y_axis_out = 0;
		LinePatrolData_S.x_axis_out = 0;
		LinePatrolData_S.yaw_out = 0;
	}
}
static void LinePatrol_EncoderTracking(void) 
{
	static uint16_t OldEncode;
    static uint8_t sdtime = 0;
	if(LinePatrolData_S.TrackingpQ != 0)
	{
		if(0 == LinePatrolData_S.TrackingpQ->sta)
		{
			LinePatrolData_S.TrackingpQ->SetVal = (CALC_C*LinePatrolData_S.TrackingpQ->SetVal/WHEEL_D)/10;
			OldEncode = *(LinePatrolData_S.TrackingpQ->CodedValue_q);
			LinePatrolData_S.y_axis_out = LINE_PATROL_SPEED_START;
			LinePatrolData_S.TrackingpQ->sta = 1;
		}
		else if(1 == LinePatrolData_S.TrackingpQ->sta)
		{
			if(Tim_CheckTimeOut(OldEncode,
				                *(LinePatrolData_S.TrackingpQ->CodedValue_q),
			                    LinePatrolData_S.TrackingpQ->SetVal*7/10))
			{
				LinePatrolData_S.TrackingpQ->sta = 2;
			}
			else   //加速度
			{
				if(sdtime >= SPEED_CTRL_ADD_TIME)
				{
					sdtime = 0;
					LinePatrolData_S.y_axis_out = ((LinePatrolData_S.y_axis_out*(100-SPEED_CTRL_ADD_FAC))
								                  +(LinePatrolData_S.TrackingpQ->y_axis_set*SPEED_CTRL_ADD_FAC))/100;
				}
			    sdtime++;
			}
		}
		else if(2 == LinePatrolData_S.TrackingpQ->sta) //减速度
		{
			if((sdtime >= SPEED_CTRL_MIN_TIME)
			 &&(LinePatrolData_S.y_axis_out != LINE_PATROL_SPEED_STOP))
			{
				sdtime = 0;
				if(LinePatrolData_S.y_axis_out >= (LINE_PATROL_SPEED_STOP+SPEED_CTRL_MIN_FAC))
				{
					LinePatrolData_S.y_axis_out -= SPEED_CTRL_MIN_FAC;
				}
				else
				{
					LinePatrolData_S.y_axis_out = LINE_PATROL_SPEED_STOP;
				}
			}
			sdtime++;
//			LinePatrolData_S.y_axis_out = ((LinePatrolData_S.y_axis_out*(100-SPEED_CTRL_MIN_FAC))
//			                              +(LINE_PATROL_SPEED_STOP*SPEED_CTRL_MIN_FAC))/100; 
		}
		//================循迹纠正=====================
		TrackCorrectiveCtrl(0);
        if(Tim_CheckTimeOut(OldEncode,
				            *(LinePatrolData_S.TrackingpQ->CodedValue_q),
			                LinePatrolData_S.TrackingpQ->SetVal))
		{
			LinePatrolData_S.mode = Mode_Await;
			LinePatrolData_S.y_axis_out = 0;
			LinePatrolData_S.x_axis_out = 0;
			LinePatrolData_S.yaw_out = 0;
			sdtime = 0;
		}
	}
	else
	{
		LinePatrolData_S.mode = Mode_Await;
		LinePatrolData_S.y_axis_out = 0;
		LinePatrolData_S.x_axis_out = 0;
		LinePatrolData_S.yaw_out = 0;
		sdtime = 0;
	}
}
static void LinePatrol_SituAdjust(void) 
{
	uint8_t en;
	static uint8_t i = 0;
	if(LinePatrolData_S.TrackingpQ != 0)
	{
		en = 0;
		if(0 == LinePatrolData_S.TrackingpQ->sta)
		{
			LinePatrolData_S.TrackingpQ->sta = 1;
			LinePatrolData_S.TrackingpQ->TimeMem = LinePatrolData_S.Time;
		}
		else 
		{
			//================循迹纠正=====================
			TrackCorrectiveCtrl(1);
			if(Tim_CheckTimeOut(LinePatrolData_S.TrackingpQ->TimeMem,LinePatrolData_S.Time,LinePatrolData_S.TrackingpQ->SetVal))
			{
				en = 1;
			}
			if((ABS(LinePatrolData_S.y_axis_out) < 7)
			 &&(ABS(LinePatrolData_S.x_axis_out) < 7)
			 &&(ABS(LinePatrolData_S.yaw_out) == 0))
			{
				i++;
				if(i > 10)
				{
					en = 1;
				}
			}
			else
			{
				i = 0;
			}
			if(en == 1)
			{
				xpid.f_pid_reset(&xpid,X_FDBCK_P,X_FDBCK_I,X_FDBCK_D);
				yawpid.f_pid_reset(&yawpid,YAW_FDBCK_P,YAW_FDBCK_I,YAW_FDBCK_D);
				LinePatrolData_S.mode = Mode_Await;
				LinePatrolData_S.y_axis_out = 0;
				LinePatrolData_S.x_axis_out = 0;
				LinePatrolData_S.yaw_out = 0;
			}
		}
	}
	else
	{
		xpid.f_pid_reset(&xpid,X_FDBCK_P,X_FDBCK_I,X_FDBCK_D);
		yawpid.f_pid_reset(&yawpid,YAW_FDBCK_P,YAW_FDBCK_I,YAW_FDBCK_D);
		LinePatrolData_S.mode = Mode_Await;
		LinePatrolData_S.y_axis_out = 0;
		LinePatrolData_S.x_axis_out = 0;
		LinePatrolData_S.yaw_out = 0;
	}
}
static void UnderpanControl_LinePatrol_Wheel_90(void)
{
	if(LinePatrolData_S.TrackingpQ != 0)
	{
		switch(LinePatrolData_S.TrackingpQ->sta)
		{
			case 0:  //开始
				if(LinePatrolData_S.TrackingpQ->SetVal == 0)
				{
					LinePatrolData_S.y_axis_out = 0;
					LinePatrolData_S.yaw_out= LinePatrolData_S.TrackingpQ->yaw_set;
					LinePatrolData_S.c = 0;
					LinePatrolData_S.TrackingpQ->sta = 2;
				}
				else
				{
					LinePatrolData_S.y_axis_out = LINE_PATROL_SPEED_WHEEL_FORWARD;
					LinePatrolData_S.yaw_out = 0;
					LinePatrolData_S.TrackingpQ->TimeMem = LinePatrolData_S.Time;
					LinePatrolData_S.TrackingpQ->sta = 1;
				}
			    
				break;
			case 1:   //前进动作
				if(Tim_CheckTimeOut(LinePatrolData_S.TrackingpQ->TimeMem,
								    LinePatrolData_S.Time,
				                    LinePatrolData_S.TrackingpQ->SetVal))
				{
					LinePatrolData_S.y_axis_out = 0;
					LinePatrolData_S.yaw_out = LinePatrolData_S.TrackingpQ->yaw_set;
					LinePatrolData_S.c = 0;
					LinePatrolData_S.TrackingpQ->sta = 2;
				}
				else
				{
					//================循迹纠正=====================
					TrackCorrectiveCtrl(0);
				}
				break;
		    case 2:   //转弯动作，屏蔽当前的线
//				if(
////				  (LinePatrolData_S.HeadSignal->bit.ls3 != LinePatrolData_S.active_level)&&	
//                  (LinePatrolData_S.HeadSignal->bit.ls4 != LinePatrolData_S.active_level)&&
//                  (LinePatrolData_S.HeadSignal->bit.ls5 != LinePatrolData_S.active_level)//&&
////				  (LinePatrolData_S.HeadSignal->bit.ls6 != LinePatrolData_S.active_level)
//                  )
//                {
					LinePatrolData_S.c++;
					
					if(LinePatrolData_S.c >= 60)
					{
						if(LinePatrolData_S.yaw_out < 0)
						{
							LinePatrolData_S.yaw_out = -100;
						}
						else
						{
							LinePatrolData_S.yaw_out = 100;
						}
						LinePatrolData_S.TrackingpQ->sta = 3;
					}
//                }
//				else
//				{
//					LinePatrolData_S.c = 0;
//				}
				break;
			case 3:   //转弯动作，巡下一条线
				if((LinePatrolData_S.HeadSignal->bit.ls4 == LinePatrolData_S.active_level)||
//                    (LinePatrolData_S.HeadSignal->bit.ls3 == LinePatrolData_S.active_level)||
//                    (LinePatrolData_S.HeadSignal->bit.ls6 == LinePatrolData_S.active_level)||
                 (LinePatrolData_S.HeadSignal->bit.ls5 == LinePatrolData_S.active_level))
                {
                    LinePatrolData_S.mode = Mode_Await;
					LinePatrolData_S.c = 0;
					LinePatrolData_S.y_axis_out = 0;
					LinePatrolData_S.x_axis_out = 0;
					LinePatrolData_S.yaw_out = 0;
                }
				break;
		}
	}
	else
	{
		LinePatrolData_S.mode = Mode_Await;
		LinePatrolData_S.y_axis_out = 0;
		LinePatrolData_S.x_axis_out = 0;
		LinePatrolData_S.yaw_out = 0;
	}
}
static void UnderpanControl_OpenLoop_DelayWalk(void)
{
    if(LinePatrolData_S.TrackingpQ != 0)
	{
        switch(LinePatrolData_S.TrackingpQ->sta)
        {
            case 0:
				LinePatrolData_S.x_axis_out = LinePatrolData_S.TrackingpQ->x_axis_set;
			    LinePatrolData_S.y_axis_out = LinePatrolData_S.TrackingpQ->y_axis_set;
			    LinePatrolData_S.yaw_out = LinePatrolData_S.TrackingpQ->yaw_set;
                LinePatrolData_S.TrackingpQ->TimeMem = LinePatrolData_S.Time;
                
                LinePatrolData_S.TrackingpQ->sta = 1;
                break;
            case 1:
                if(Tim_CheckTimeOut(LinePatrolData_S.TrackingpQ->TimeMem,
					                LinePatrolData_S.Time,
				                    LinePatrolData_S.TrackingpQ->SetVal))
                {
                    LinePatrolData_S.mode = Mode_Await;
					LinePatrolData_S.y_axis_out = 0;
					LinePatrolData_S.x_axis_out = 0;
					LinePatrolData_S.yaw_out = 0;
					LinePatrolData_S.TrackingpQ->sta = 0;
                }
                break;
        }
    }
	else
	{
		LinePatrolData_S.mode = Mode_Await;
		LinePatrolData_S.y_axis_out = 0;
		LinePatrolData_S.x_axis_out = 0;
		LinePatrolData_S.yaw_out = 0;
	}
}
/* 函数体 --------------------------------------------------------------------*/
void LinePatrol_Scan(void){
	switch(LinePatrolData_S.mode)
	{
		case Mode_Stop:
			
			break;
		case Mode_Await:
			free(LinePatrol_S);
	        LinePatrol_S = 0;
		    if(LinePatrol_S == 0)
			{
				LinePatrolData_S.mode = Mode_Stop;
			}
			break;
		case Mode_ArticlePatrol:
			LinePatrol_ArticlePatrol();
			break;
        case Mode_DelayTracking:
			LinePatrol_DelayTracking();
			break;
		case Mode_StateTracking:
			LinePatrol_StateTracking();
			break;
        case Mode_EncoderTracking:
			LinePatrol_EncoderTracking();
			break;
        case Mode_SituAdjust:
			LinePatrol_SituAdjust();
			break;			
        case Mode_Wheel_90:
			UnderpanControl_LinePatrol_Wheel_90();
			break;	
        case DelayOpenLoopWalk:
			UnderpanControl_OpenLoop_DelayWalk();
			break;			
	}
	LinePatrolData_S.yaw_Practicalout = LinePatrolData_S.yaw_out;
	if(LinePatrolData_S.TrackingpQ->Car_Direction < CarDirection_Left)
	{
		LinePatrolData_S.x_axis_Practicalout = LinePatrolData_S.x_axis_out;
		if(LinePatrolData_S.TrackingpQ->Car_Direction == CarDirection_Head)
		{
			LinePatrolData_S.y_axis_Practicalout = LinePatrolData_S.y_axis_out;
		}
		else
		{
			LinePatrolData_S.y_axis_Practicalout = -LinePatrolData_S.y_axis_out;
		}
	}
	else
	{
		LinePatrolData_S.y_axis_Practicalout = LinePatrolData_S.x_axis_out;
		if(LinePatrolData_S.TrackingpQ->Car_Direction == CarDirection_Left)
		{
			LinePatrolData_S.x_axis_Practicalout = -LinePatrolData_S.y_axis_out;
		}
		else
		{
			LinePatrolData_S.x_axis_Practicalout = LinePatrolData_S.y_axis_out;
		}
	}
	LinePatrolData_S.Time++;
}
void LinePatrol_Init( uint8_t active_level,
	                  uint8_t *Signal1q,
				 	  uint8_t *Signal2q,
				 	  uint8_t *Signal3q,
					  uint8_t *Signal4q){
	memset(&LinePatrolData_S, 0, sizeof(LinePatrolDef_t));
	LinePatrolData_S.active_level = active_level;
	LinePatrolData_S.Signal1 = (SignalDef_u*)Signal1q;
	LinePatrolData_S.Signal2 = (SignalDef_u*)Signal2q;
	LinePatrolData_S.Signal3 = (SignalDef_u*)Signal3q;
	LinePatrolData_S.Signal4 = (SignalDef_u*)Signal4q;
	LinePatrolData_S.mode = Mode_Stop;
						  
	PID_struct_init(&xpid,POSITION_PID,LINE_PATROL_SPEED_MAX,200,X_FDBCK_P,X_FDBCK_I,X_FDBCK_D);//2.2
	PID_struct_init(&yawpid,POSITION_PID,LINE_PATROL_SPEED_MAX,200,YAW_FDBCK_P,YAW_FDBCK_I,YAW_FDBCK_D);//1.2
}
				  
void Execute_LinePatrol(DirectionDef_e Car_Direction,uint16_t spd, uint8_t wirenum,uint8_t mode)
{
	while((LinePatrol_S != 0)||(LinePatrolData_S.mode != Mode_Stop))
		LINE_PATROL_DELAY(50);
	LinePatrol_S = malloc(sizeof(LinePatrolDef));
	LinePatrolData_S.TrackingpQ = LinePatrol_S;
	if(Car_Direction == CarDirection_Head)      
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal1;      
	else if(Car_Direction == CarDirection_Tail)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal3;
	else if(Car_Direction == CarDirection_Left)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal2;
    else if(Car_Direction == CarDirection_Right)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal4;
	LinePatrolData_S.TrackingpQ->Car_Direction = Car_Direction;
	
	LinePatrol_S->SetVal = mode;
    LinePatrol_S->line = wirenum;
	LinePatrol_S->sta = 0;
	LinePatrol_S->TimeMem = 0;
	LinePatrol_S->x_axis_set = 0;
	LinePatrol_S->yaw_set = 0;
	LinePatrol_S->y_axis_set = spd;
	LinePatrolData_S.c = 0;
	LinePatrolData_S.mode = Mode_ArticlePatrol;
//	while(LinePatrolData_S.mode != Mode_Stop)
//		LINE_PATROL_DELAY(9);
}
void Execute_DelayTracking(DirectionDef_e Car_Direction,uint16_t spd, uint16_t time)
{
	while((LinePatrol_S != 0)||(LinePatrolData_S.mode != Mode_Stop))
		LINE_PATROL_DELAY(50);
	LinePatrol_S = malloc(sizeof(LinePatrolDef));
	LinePatrolData_S.TrackingpQ = LinePatrol_S;
	
	if(Car_Direction == CarDirection_Head)      
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal1;      
	else if(Car_Direction == CarDirection_Tail)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal3;
	else if(Car_Direction == CarDirection_Left)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal2;
    else if(Car_Direction == CarDirection_Right)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal4;
	
	LinePatrolData_S.TrackingpQ->Car_Direction = Car_Direction;
	LinePatrol_S->sta = 0;
	LinePatrol_S->SetVal = time;
	LinePatrol_S->x_axis_set = 0;
	LinePatrol_S->yaw_set = 0;
	LinePatrol_S->y_axis_set = spd;
	LinePatrolData_S.mode = Mode_DelayTracking;
//	while(LinePatrolData_S.mode != Mode_Stop)
//		LINE_PATROL_DELAY(9);
}
void Execute_StateTracking(DirectionDef_e Car_Direction,
	                       uint16_t spd,
                           uint16_t (*ReadData_func)(void),
						   uint16_t TargetState)
{
	while((LinePatrol_S != 0)||(LinePatrolData_S.mode != Mode_Stop))
		LINE_PATROL_DELAY(50);
	LinePatrol_S = malloc(sizeof(LinePatrolDef));
	LinePatrolData_S.TrackingpQ = LinePatrol_S;
	if(Car_Direction == CarDirection_Head)      
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal1;      
	else if(Car_Direction == CarDirection_Tail)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal3;
	else if(Car_Direction == CarDirection_Left)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal2;
    else if(Car_Direction == CarDirection_Right)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal4;
	LinePatrolData_S.TrackingpQ->Car_Direction = Car_Direction;
	
	LinePatrol_S->ReadData_func = ReadData_func;
	LinePatrol_S->TimeMem = 0;
	LinePatrol_S->sta = 0;
	LinePatrol_S->SetVal = TargetState;
	LinePatrol_S->x_axis_set = 0;
	LinePatrol_S->yaw_set = 0;
	LinePatrol_S->y_axis_set = spd;
	LinePatrolData_S.mode = Mode_StateTracking;
//	while(LinePatrolData_S.mode != Mode_Stop)
//		LINE_PATROL_DELAY(9);
}
void Execute_EncoderTracking(DirectionDef_e Car_Direction,uint16_t spd,uint16_t millimeter)
{
	uint16_t *CodedValue_q;
	
	CodedValue_q = &(DCMotorMiniwatt1_S.Motor_UploadData.DATE.Coded_Value);
	while((LinePatrol_S != 0)||(LinePatrolData_S.mode != Mode_Stop))
		LINE_PATROL_DELAY(50);
	LinePatrol_S = malloc(sizeof(LinePatrolDef));
	LinePatrolData_S.TrackingpQ = LinePatrol_S;
	if(Car_Direction == CarDirection_Head)      
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal1;      
	else if(Car_Direction == CarDirection_Tail)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal3;
	else if(Car_Direction == CarDirection_Left)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal2;
    else if(Car_Direction == CarDirection_Right)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal4;
	LinePatrolData_S.TrackingpQ->Car_Direction = Car_Direction;
	LinePatrol_S->CodedValue_q = CodedValue_q;
	LinePatrol_S->sta = 0;
	LinePatrol_S->SetVal = millimeter;
	LinePatrol_S->x_axis_set = 0;
	LinePatrol_S->yaw_set = 0;
	LinePatrol_S->y_axis_set = spd;
	LinePatrolData_S.mode = Mode_EncoderTracking;
//	while(LinePatrolData_S.mode != Mode_Stop)
//		LINE_PATROL_DELAY(9);
}
void Execute_SituAdjust(uint16_t OutTime)
{
	while((LinePatrol_S != 0)||(LinePatrolData_S.mode != Mode_Stop))
		LINE_PATROL_DELAY(50);
	LinePatrol_S = malloc(sizeof(LinePatrolDef));
	LinePatrolData_S.TrackingpQ = LinePatrol_S;
	LinePatrolData_S.TrackingpQ->Car_Direction = CarDirection_Head;
	LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal1;
	
	LinePatrol_S->sta = 0;
	LinePatrol_S->x_axis_set = 0;
	LinePatrol_S->yaw_set = 0;
	LinePatrol_S->y_axis_set = 0;
	LinePatrol_S->SetVal = OutTime;
	LinePatrolData_S.mode = Mode_SituAdjust;
	while(LinePatrolData_S.mode != Mode_Stop)
		LINE_PATROL_DELAY(9);
}

void Execute_Wheel_90(DirectionDef_e Car_Direction,int16_t spd, uint16_t time)
{
	while((LinePatrol_S != 0)||(LinePatrolData_S.mode != Mode_Stop))
		LINE_PATROL_DELAY(50);
	LinePatrol_S = malloc(sizeof(LinePatrolDef));
	LinePatrolData_S.TrackingpQ = LinePatrol_S;
	
	if(Car_Direction == CarDirection_Head)      
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal1;      
	else if(Car_Direction == CarDirection_Tail)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal3;
	else if(Car_Direction == CarDirection_Left)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal2;
    else if(Car_Direction == CarDirection_Right)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal4;
	LinePatrol_S->Car_Direction = Car_Direction;
    LinePatrol_S->line = 0;
	LinePatrol_S->sta = 0;
	LinePatrol_S->TimeMem = 0;
	LinePatrol_S->SetVal = time;
	LinePatrol_S->x_axis_set = 0;
	LinePatrol_S->yaw_set = spd;
	LinePatrol_S->y_axis_set = 0;
	LinePatrolData_S.mode = Mode_Wheel_90;
//	while(LinePatrolData_S.mode != Mode_Stop)
//		LINE_PATROL_DELAY(9);
}

void ExecuteOpenLoop_DelayWalk(DirectionDef_e Car_Direction,
							   uint16_t y_val,
							   uint16_t x_val,
							   int16_t  yaw_val,
							   uint16_t time)
{
	while((LinePatrol_S != 0)||(LinePatrolData_S.mode != Mode_Stop))
		LINE_PATROL_DELAY(50);
	LinePatrol_S = malloc(sizeof(LinePatrolDef));
	LinePatrolData_S.TrackingpQ = LinePatrol_S;
	
	if(Car_Direction == CarDirection_Head)      
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal1;      
	else if(Car_Direction == CarDirection_Tail)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal3;
	else if(Car_Direction == CarDirection_Left)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal2;
    else if(Car_Direction == CarDirection_Right)
		LinePatrolData_S.HeadSignal = LinePatrolData_S.Signal4;
	
	LinePatrol_S->Car_Direction = Car_Direction;
    LinePatrol_S->line = 0;
	LinePatrol_S->sta = 0;
	LinePatrol_S->TimeMem = 0;
	LinePatrol_S->SetVal = time;
	LinePatrol_S->x_axis_set = x_val;
	LinePatrol_S->yaw_set = yaw_val;
	LinePatrol_S->y_axis_set = y_val;
	LinePatrolData_S.mode = DelayOpenLoopWalk;
//	while(LinePatrolData_S.mode != Mode_Stop)
//		LINE_PATROL_DELAY(9);
}

void WaitForTheCarToStop(void)
{
	while((LinePatrol_S != 0)||(LinePatrolData_S.mode != Mode_Stop))
		LINE_PATROL_DELAY(2);
}


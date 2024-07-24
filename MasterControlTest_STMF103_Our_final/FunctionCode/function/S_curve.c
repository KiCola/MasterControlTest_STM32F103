#include "S_curve.h"
#include <math.h>
#include "stdio.h"



#define SpdModeFac 450
#define StepModeFac 40

static void S_ModelChangeSpeed_Allot(STEPPINGMOTOR_T *hand)
{
    int32_t Vbuf;
    /*******************判断速度的趋势************************/
    if(hand->V_Target == hand->V_Actual)  
    {
        hand->direction = Invariability;  //速度不变
        hand->Sta = ConstSpd; //恒速阶段
    }
    else if(hand->V_Target > hand->V_Actual)
    {
        hand->direction = Largen;  //速度变大
        hand->Sta = AddAdd;   //加加速阶段
    }
    else
    {                  
        hand->direction = Wane;  //速度变小
        hand->Sta = AddAdd;   //加加速阶段
    }
    /**************速度分配**************/
    Vbuf = (hand->V_Target-hand->V_Actual)/3;//分成三个阶段
    hand->V0 = hand->V_Actual;        //起始速度
    hand->V1 = hand->V_Actual+Vbuf;   //加加速阶段最大速度值
    hand->V2 = hand->V_Actual+Vbuf*2; //匀加速阶段最大速度值
    hand->V3 = hand->V_Target;        //最终速度 
    if(SpdMode == hand->wordmode) //速度控制模式
    {
        hand->J_MAX = (hand->V_Target-hand->V_Actual)/SpdModeFac;
    }
    else //步数控制模式
    {
        hand->J_MAX = (hand->V1 - hand->V_Actual)*2/(hand->StepNum*hand->StepNum/StepModeFac);//81
    }
    if(hand->J_MAX == 0)
    {             
        if(Largen == hand->direction)
        {
            hand->J_MAX = 1;
        }
        else if(Wane == hand->direction)
        {
            hand->J_MAX = -1;
        }
    }
    hand->T = 0;
}
void S_ModelChangeSpeed_Calc(STEPPINGMOTOR_T *hand)
{
    int32_t Vbuf;
    static uint16_t StepNumdiff = 0;
    switch(hand->Sta)
    {
        case Stop:      //停止模式
            /*************开始启动***********************/
            if((hand->V_Target != 0)||(hand->V_Actual != hand->V_Target))
            {
                hand->V_Actual = hand->Vs;//真实速度等于起始速度
                S_ModelChangeSpeed_Allot(hand);//数据初始化
                if(StepMode == hand->wordmode)
                {
                    hand->StepNumBuf = hand->StepNum;
                }
            }
            break;
        case AddAdd:    //加加速阶段
            hand->J = hand->J_MAX;
            /*************加加速算法*****************/
            hand->A = hand->T*hand->J;
            hand->V_Actual = hand->V0+(hand->A*hand->T/2);
            /*************判断是否到达预期值*****************/
            if(((Largen == hand->direction)&&(hand->V_Actual > hand->V1))
             ||((Wane == hand->direction)&&(hand->V_Actual < hand->V1)))
            {
                hand->Sta = MeanAdd;   //匀加速阶段
                hand->T = 0;
            }
            break;
        case MeanAdd:   //匀加速阶段
            hand->J = 0;
            hand->V_Actual += hand->A;
            if(((Largen == hand->direction)&&(hand->V_Actual > hand->V2))
             ||((Wane == hand->direction)&&(hand->V_Actual < hand->V2)))
            {
                hand->Sta = MinusAdd;   //减加速阶段
                hand->V2 = hand->V_Actual;
                hand->T = 0;
            }
            break;
        case MinusAdd:  //减加速阶段
            hand->J = -hand->J_MAX;
            Vbuf = ((pow(hand->T,2)*hand->J)/2);
            hand->V_Actual = hand->V2
                       + hand->A*hand->T
                       + Vbuf;
            if(((Largen == hand->direction)&&(hand->V_Actual > hand->V3))
             ||((Wane == hand->direction)&&(hand->V_Actual < hand->V3)))
            {
                hand->V_Actual = hand->V3;
                if(StepMode == hand->wordmode)
                {
                    StepNumdiff = hand->StepNum-hand->StepNumBuf;
                }
                hand->Sta = ConstSpd;   //恒速阶段
                hand->T = 0;
            }
            break;
        case ConstSpd:  //恒速阶段
            if(hand->V_Actual != hand->V_Target)
            {
                S_ModelChangeSpeed_Allot(hand);
            }
            else if(hand->V_Target == 0)
            {
                hand->Sta = Stop;
            }
            if(StepMode == hand->wordmode)
            {
                if(hand->StepNumBuf == StepNumdiff)
                {
                    hand->V_Target = hand->Vs;
                    S_ModelChangeSpeed_Allot(hand);
                }
            }
            break;
        default:        //防逻辑错误
            hand->V_Actual = 0;
            hand->V_Target = 0;
            hand->Vs = 0;
            hand->Sta = Stop;
            break;
    }
    if(hand->Sta != Stop)   //启动模式
    {
        if(StepMode == hand->wordmode)  //步数模式
        {
            if(hand->StepNumBuf > 0)  //未走完继续执行
            {
                hand->StepNumBuf--;
            }
            else
            {
                hand->V_Actual = 0;
                hand->V_Target = 0;
                hand->Sta = Stop;
            }
        }
        hand->T++;
    }
}




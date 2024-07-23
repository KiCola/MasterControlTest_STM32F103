#ifndef _S_curve_H_
#define _S_curve_H_


#include "stdint.h"



typedef struct STEPPINGMOTOR_T
{
    enum{Stop=0,AddAdd,MeanAdd,MinusAdd,ConstSpd} Sta; 
    enum{Invariability,Largen,Wane} direction;  
    enum{SpdMode,StepMode} wordmode;
    uint16_t StepNum;
    uint16_t StepNumBuf;
    int32_t J;
    int32_t J_MAX;
    int32_t A; 
    int32_t Vs;        //起始速度
    int32_t V_Actual;  //输出速度
    int32_t V_Target;  //目标速度
    int32_t V0;
    int32_t V1;
    int32_t V2;
    int32_t V3;
    uint32_t T;  
    
}STEPPINGMOTOR_T;

void S_ModelChangeSpeed_Calc(STEPPINGMOTOR_T *hand);

#endif

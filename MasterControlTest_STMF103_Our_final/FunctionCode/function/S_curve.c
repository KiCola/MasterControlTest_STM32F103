#include "S_curve.h"
#include <math.h>
#include "stdio.h"



#define SpdModeFac 450
#define StepModeFac 40

static void S_ModelChangeSpeed_Allot(STEPPINGMOTOR_T *hand)
{
    int32_t Vbuf;
    /*******************�ж��ٶȵ�����************************/
    if(hand->V_Target == hand->V_Actual)  
    {
        hand->direction = Invariability;  //�ٶȲ���
        hand->Sta = ConstSpd; //���ٽ׶�
    }
    else if(hand->V_Target > hand->V_Actual)
    {
        hand->direction = Largen;  //�ٶȱ��
        hand->Sta = AddAdd;   //�Ӽ��ٽ׶�
    }
    else
    {                  
        hand->direction = Wane;  //�ٶȱ�С
        hand->Sta = AddAdd;   //�Ӽ��ٽ׶�
    }
    /**************�ٶȷ���**************/
    Vbuf = (hand->V_Target-hand->V_Actual)/3;//�ֳ������׶�
    hand->V0 = hand->V_Actual;        //��ʼ�ٶ�
    hand->V1 = hand->V_Actual+Vbuf;   //�Ӽ��ٽ׶�����ٶ�ֵ
    hand->V2 = hand->V_Actual+Vbuf*2; //�ȼ��ٽ׶�����ٶ�ֵ
    hand->V3 = hand->V_Target;        //�����ٶ� 
    if(SpdMode == hand->wordmode) //�ٶȿ���ģʽ
    {
        hand->J_MAX = (hand->V_Target-hand->V_Actual)/SpdModeFac;
    }
    else //��������ģʽ
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
        case Stop:      //ֹͣģʽ
            /*************��ʼ����***********************/
            if((hand->V_Target != 0)||(hand->V_Actual != hand->V_Target))
            {
                hand->V_Actual = hand->Vs;//��ʵ�ٶȵ�����ʼ�ٶ�
                S_ModelChangeSpeed_Allot(hand);//���ݳ�ʼ��
                if(StepMode == hand->wordmode)
                {
                    hand->StepNumBuf = hand->StepNum;
                }
            }
            break;
        case AddAdd:    //�Ӽ��ٽ׶�
            hand->J = hand->J_MAX;
            /*************�Ӽ����㷨*****************/
            hand->A = hand->T*hand->J;
            hand->V_Actual = hand->V0+(hand->A*hand->T/2);
            /*************�ж��Ƿ񵽴�Ԥ��ֵ*****************/
            if(((Largen == hand->direction)&&(hand->V_Actual > hand->V1))
             ||((Wane == hand->direction)&&(hand->V_Actual < hand->V1)))
            {
                hand->Sta = MeanAdd;   //�ȼ��ٽ׶�
                hand->T = 0;
            }
            break;
        case MeanAdd:   //�ȼ��ٽ׶�
            hand->J = 0;
            hand->V_Actual += hand->A;
            if(((Largen == hand->direction)&&(hand->V_Actual > hand->V2))
             ||((Wane == hand->direction)&&(hand->V_Actual < hand->V2)))
            {
                hand->Sta = MinusAdd;   //�����ٽ׶�
                hand->V2 = hand->V_Actual;
                hand->T = 0;
            }
            break;
        case MinusAdd:  //�����ٽ׶�
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
                hand->Sta = ConstSpd;   //���ٽ׶�
                hand->T = 0;
            }
            break;
        case ConstSpd:  //���ٽ׶�
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
        default:        //���߼�����
            hand->V_Actual = 0;
            hand->V_Target = 0;
            hand->Vs = 0;
            hand->Sta = Stop;
            break;
    }
    if(hand->Sta != Stop)   //����ģʽ
    {
        if(StepMode == hand->wordmode)  //����ģʽ
        {
            if(hand->StepNumBuf > 0)  //δ�������ִ��
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




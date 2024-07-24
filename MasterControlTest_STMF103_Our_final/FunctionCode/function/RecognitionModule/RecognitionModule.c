/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "RecognitionModule/RecognitionModule.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define ORDER_LEN 2
#define STARTORDER_ 0
#define STOPORDER_  1
/* ˽�б��� ------------------------------------------------------------------*/
static const uint8_t order[2][ORDER_LEN] = 
{
	{0x1b,0x31},
	{0x1b,0x30},
};

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/


void RecognitionModule_Init(RecognitionModule_s *hand,
							void (*pQ_Transmit)(uint8_t *pData, uint16_t Size))
{
	hand->err = 0;
	hand->time = 0;
	hand->RecognitionModuleSte = RM_leisure;
	hand->RecognitionModule_Transmit = pQ_Transmit;
}


void RecognitionModule_Scan1Ms(RecognitionModule_s *hand)
{
	if(hand->RecognitionModule_Transmit != 0)
	{
		switch(hand->RecognitionModuleSte)
		{
			case RM_start:   //���Ϳ�ʼ����
				hand->RecognitionModule_Transmit((uint8_t*)order[STARTORDER_],ORDER_LEN);
			    hand->time = 0;
			    hand->RecognitionModuleSte = RM_starting;
				break;
			case RM_starting:  //�ȴ�������ջ�Ӧ
				if(hand->time >= 500)
				{
					hand->err = ERR_disconnect;
					hand->RecognitionModuleSte = RM_error;
				}
				break;
			case RM_Identify:  //����ʶ����
				if(hand->time >= 8000)
				{
					hand->err = ERR_Identification_failure;
					hand->RecognitionModule_Transmit((uint8_t*)order[STOPORDER_],ORDER_LEN);
					hand->RecognitionModuleSte = RM_error;
				}
				break;
//			case RM_error:     //ʶ�����
//				
//				break;
			default:
				break;
		}
		hand->time++;
	}
}

void RecognitionModule_ReceivingProcess(RecognitionModule_s *hand,uint8_t *datahend,uint16_t datalen)
{
	if((1 == datalen)&&(RM_starting == hand->RecognitionModuleSte))
	{
		if(*datahend == 0x06)
		{
			hand->time = 0;
			hand->RecognitionModuleSte = RM_Identify;
		}
	}
	else if(datalen >= 2)
	{
		if(datahend[datalen-1] == 0x0d)
		{
			hand->RecognitionModuleSte = RM_succeed;
		}
	}
}



void RecognitionModule_Start(RecognitionModule_s *hand)
{
	if((hand->RecognitionModuleSte == RM_leisure)
	 ||(hand->RecognitionModuleSte == RM_error)
   	 ||(hand->RecognitionModuleSte == RM_succeed))
	{
		hand->RecognitionModuleSte = RM_start;
	}
}















#ifndef __RecognitionModule_H__
#define __RecognitionModule_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "mytype.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
typedef enum 
{
	RM_leisure=0,
	RM_start,
	RM_starting,
	RM_Identify,
	RM_error,
	RM_succeed,
}RecognitionModuleSte_e;
typedef struct
{
	volatile RecognitionModuleSte_e RecognitionModuleSte;
	volatile uint8_t err;
	volatile uint16_t time;
	void (*RecognitionModule_Transmit)(uint8_t *pData, uint16_t Size);
}RecognitionModule_s;
/* �궨�� --------------------------------------------------------------------*/
#define ERR_disconnect 1
#define ERR_Identification_failure 2



/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void RecognitionModule_Init(RecognitionModule_s *hand,
							void (*pQ_Transmit)(uint8_t *pData, uint16_t Size));
void RecognitionModule_Scan1Ms(RecognitionModule_s *hand);
void RecognitionModule_ReceivingProcess(RecognitionModule_s *hand,uint8_t *datahend,uint16_t datalen);
void RecognitionModule_Start(RecognitionModule_s *hand);   



                    
#endif  // __RecognitionModule_H__


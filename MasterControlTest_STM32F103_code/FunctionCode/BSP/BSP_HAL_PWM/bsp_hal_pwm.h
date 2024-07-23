#ifndef __BSP_HAL_PWM_H__
#define __BSP_HAL_PWM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "mytype.h"

/* ���Ͷ��� ------------------------------------------------------------------*/

/* �궨�� --------------------------------------------------------------------*/
#define TIMPWMHEAD htim2

#define PWMPERIOD (TIMPWMHEAD.Init.Period+1) 
#define FTM_1 1
#define FTM_2 2
#define FTM_3 3
#define FTM_4 4
#define PWM_DUTYCYCLE_LOAD_OK     1
#define PWM_ILLEGALITY_DUTYCYCLE  2
#define PWM_GALLERY_NONE          3

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void BSP_PWM_Init(void);
uint8_t BSP_PWM_SetCCR(uint8_t ch,uint16_t val);
uint8_t BSP_PWM_SetDuty(uint8_t pwm_gallery,uint16_t dutycycle);


#endif  // __BSP_HAL_PWM_H__


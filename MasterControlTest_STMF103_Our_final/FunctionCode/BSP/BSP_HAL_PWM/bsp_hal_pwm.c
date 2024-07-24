/* 包含头文件 ----------------------------------------------------------------*/
#include "BSP_HAL_PWM/bsp_hal_pwm.h"
#include "tim.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/


#define PWM_MAX 1000
#define PWM_MIN 0
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
void BSP_PWM_Init(void)
{
    HAL_TIM_PWM_Start(&TIMPWMHEAD,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TIMPWMHEAD,TIM_CHANNEL_2);
}
/********************PWM********************/
/**
  * @brief  设置输出PWM的CCR
  * @param  ch：通道名称
            可写入：1-FTM_1
            val：占空比
            可写入：0~PWMPERIOD
  * @retval 设置占空比操作的反馈
            1-PWM_DUTYCYCLE_LOAD_OK
            2-PWM_ILLEGALITY_DUTYCYCLE
            3-PWM_GALLERY_NONE
  */
uint8_t BSP_PWM_SetCCR(uint8_t ch,uint16_t val)
{
    uint8_t err;
    if(val > PWMPERIOD)
	{
		err = PWM_ILLEGALITY_DUTYCYCLE;
	}
	else
	{
		switch(ch)
		{
			case FTM_1:
				TIMPWMHEAD.Instance->CCR1 = val;
				err = PWM_DUTYCYCLE_LOAD_OK;
				break;
			case FTM_2:
				TIMPWMHEAD.Instance->CCR2 = val;
				err = PWM_DUTYCYCLE_LOAD_OK;
				break;
			default:
				err = PWM_GALLERY_NONE;
				break;
		}
	}
    
    return err;
}
/********************PWM********************/
/**
  * @brief  设置输出PWM的占空比
  * @param  pwm_gallery：通道名称
            可写入：1-FTM_1
            dutycycle：占空比
            可写入：0~100
  * @retval 设置占空比操作的反馈
            1-PWM_DUTYCYCLE_LOAD_OK
            2-PWM_ILLEGALITY_DUTYCYCLE
            3-PWM_GALLERY_NONE
  */
uint8_t BSP_PWM_SetDuty(uint8_t pwm_gallery,uint16_t dutycycle)
{
	uint16_t DutyCycle;
	uint8_t err;
	if(dutycycle > 100)
	{
		err = PWM_ILLEGALITY_DUTYCYCLE;
	}
	else
	{
		DutyCycle = PWMPERIOD*dutycycle/100;
		switch(pwm_gallery)
		{
			case FTM_1:
				TIMPWMHEAD.Instance->CCR1 = DutyCycle;
				err = PWM_DUTYCYCLE_LOAD_OK;
				break;
			case FTM_2:
				TIMPWMHEAD.Instance->CCR2 = DutyCycle;
				err = PWM_DUTYCYCLE_LOAD_OK;
				break;
			default:
				err = PWM_GALLERY_NONE;
				break;
		}
	}
	
	return err;
}


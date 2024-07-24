/* 包含头文件 ----------------------------------------------------------------*/
#include "BSP_HAL_TIME/bsp_hal_time.h"

/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
static VoidFuncVoid TimeCallBackFunc = NULL;
static VoidFuncVoid Time2CallBackFunc = NULL;
/* 扩展变量 ------------------------------------------------------------------*/

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/
/***************定时中断初始化************************/
void Timer_SetHandler(VoidFuncVoid func)
{
	TimeCallBackFunc = func;
	HAL_TIM_Base_Start_IT(&TIMER);
}
void Timer2_SetHandler(VoidFuncVoid func)
{
	Time2CallBackFunc = func;
	HAL_TIM_Base_Start_IT(&htim2);
}
/***********TIM溢出中断****************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//HAL
{
	if(TIMER.Instance == htim->Instance)
	{
		if(NULL != TimeCallBackFunc)
		{
			TimeCallBackFunc();
		}
	}
	else if(htim2.Instance == htim->Instance)
	{
		if(NULL != Time2CallBackFunc)
		{
			Time2CallBackFunc();
		}
	}
}




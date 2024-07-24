#ifndef __BSP_HAL_GPIO_H__
#define __BSP_HAL_GPIO_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "mytype.h"

/* 类型定义 ------------------------------------------------------------------*/

/* 宏定义 --------------------------------------------------------------------*/
#define bright     GPIO_PIN_RESET
#define extinguish GPIO_PIN_SET
#ifdef LED2_GPIO_Port
#define LED1(x)    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, x)
#define LED1_Toggle() HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin)
#endif
#ifdef LED3_GPIO_Port
#define LED2(x)    HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, x)
#define LED2_Toggle() HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin)
#endif
/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
#ifdef LED2_GPIO_Port
void led1_level(uint8_t level);
#endif
#ifdef LED3_GPIO_Port
void led2_level(uint8_t level);
#endif
#ifdef KEY1_GPIO_Port
uint8_t KEY_1(void);
#endif
#ifdef KEY2_GPIO_Port
uint8_t KEY_2(void);
#endif
#ifdef KEY3_GPIO_Port
uint8_t KEY_3(void);
#endif
#ifdef KEY4_GPIO_Port
uint8_t KEY_4(void);
#endif
#ifdef KEY5_GPIO_Port
uint8_t KEY_5(void);
#endif
#ifdef KEY6_GPIO_Port
uint8_t KEY_6(void);
#endif
#ifdef KEY7_GPIO_Port
uint8_t KEY_7(void);
#endif
#ifdef IN1_GPIO_Port
uint8_t IN_1(void);
#endif
#ifdef IN2_GPIO_Port
uint8_t IN_2(void);
#endif
#ifdef IN3_GPIO_Port
uint8_t IN_3(void);
#endif
#ifdef IN4_GPIO_Port
uint8_t IN_4(void);
#endif

#endif  // __BSP_HAL_GPIO_H__


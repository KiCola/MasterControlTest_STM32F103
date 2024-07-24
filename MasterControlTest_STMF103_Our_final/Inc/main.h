/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY5_Pin GPIO_PIN_13
#define KEY5_GPIO_Port GPIOC
#define KEY6_Pin GPIO_PIN_14
#define KEY6_GPIO_Port GPIOC
#define KEY7_Pin GPIO_PIN_15
#define KEY7_GPIO_Port GPIOC
#define IN1_Pin GPIO_PIN_0
#define IN1_GPIO_Port GPIOC
#define IN2_Pin GPIO_PIN_1
#define IN2_GPIO_Port GPIOC
#define IN3_Pin GPIO_PIN_2
#define IN3_GPIO_Port GPIOC
#define IN4_Pin GPIO_PIN_3
#define IN4_GPIO_Port GPIOC
#define IRQ_Pin GPIO_PIN_0
#define IRQ_GPIO_Port GPIOA
#define SDN_Pin GPIO_PIN_1
#define SDN_GPIO_Port GPIOA
#define NSS_Pin GPIO_PIN_4
#define NSS_GPIO_Port GPIOA
#define W25Q16CS_Pin GPIO_PIN_0
#define W25Q16CS_GPIO_Port GPIOB
#define LCD_RESET_Pin GPIO_PIN_1
#define LCD_RESET_GPIO_Port GPIOB
#define LCD_A0_Pin GPIO_PIN_2
#define LCD_A0_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOB
#define CLK_Pin GPIO_PIN_6
#define CLK_GPIO_Port GPIOC
#define CS_Pin GPIO_PIN_7
#define CS_GPIO_Port GPIOC
#define DO_Pin GPIO_PIN_8
#define DO_GPIO_Port GPIOC
#define DI_Pin GPIO_PIN_9
#define DI_GPIO_Port GPIOC
#define RS485_EN_Pin GPIO_PIN_8
#define RS485_EN_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_3
#define LED2_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_4
#define KEY1_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_5
#define KEY2_GPIO_Port GPIOB
#define KEY3_Pin GPIO_PIN_6
#define KEY3_GPIO_Port GPIOB
#define KEY4_Pin GPIO_PIN_7
#define KEY4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

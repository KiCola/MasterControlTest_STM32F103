/* 包含头文件 ----------------------------------------------------------------*/
#include "BSP_HAL_GPIO/bsp_hal_gpio.h"
/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/

/* 扩展变量 ------------------------------------------------------------------*/

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/
#ifdef LED2_GPIO_Port
void led1_level(uint8_t level)
{
    GPIO_PinState PinState;
    if(level)
    {
        PinState = GPIO_PIN_SET;
    }
    else
    {
        PinState = GPIO_PIN_RESET;
    }
    LED1(PinState);
}
#endif
#ifdef LED3_GPIO_Port
void led2_level(uint8_t level)
{
    GPIO_PinState PinState;
    if(level)
    {
        PinState = GPIO_PIN_SET;
    }
    else
    {
        PinState = GPIO_PIN_RESET;
    }
    LED2(PinState);
}
#endif
#ifdef KEY1_GPIO_Port
uint8_t KEY_1(void)
{
    if ((KEY1_GPIO_Port->IDR & 0x0010) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
#ifdef KEY2_GPIO_Port
uint8_t KEY_2(void)
{
    if ((KEY2_GPIO_Port->IDR & 0x0020) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
#ifdef KEY3_GPIO_Port
uint8_t KEY_3(void)
{
    if ((KEY3_GPIO_Port->IDR & 0x0040) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
#ifdef KEY4_GPIO_Port
uint8_t KEY_4(void)
{
    if ((KEY4_GPIO_Port->IDR & 0x0080) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
#ifdef KEY5_GPIO_Port
uint8_t KEY_5(void)
{
    if ((KEY5_GPIO_Port->IDR & 0x2000) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
#ifdef KEY6_GPIO_Port
uint8_t KEY_6(void)
{
    if ((KEY6_GPIO_Port->IDR & 0x4000) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
#ifdef KEY7_GPIO_Port
uint8_t KEY_7(void)
{
    if ((KEY7_GPIO_Port->IDR & 0x8000) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
#ifdef IN1_GPIO_Port
uint8_t IN_1(void)
{
    if ((IN1_GPIO_Port->IDR & IN1_Pin) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
#ifdef IN2_GPIO_Port
uint8_t IN_2(void)
{
    if ((IN2_GPIO_Port->IDR & IN2_Pin) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
#ifdef IN3_GPIO_Port
uint8_t IN_3(void)
{
    if ((IN3_GPIO_Port->IDR & IN3_Pin) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
#ifdef IN4_GPIO_Port
uint8_t IN_4(void)
{
    if ((IN4_GPIO_Port->IDR & IN4_Pin) != (uint32_t)GPIO_PIN_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
















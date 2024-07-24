#ifndef __separate_led_H__
#define __separate_led_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "mytype.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef struct LED_T
{
    uint8_t  lighten_level :1;          //LED点亮的状态
    uint8_t  level_now     :1;          //目前的LED状态
	uint8_t  level_old     :1;          //上一次LED状态
    uint8_t  state         :5;
    uint8_t  Flicker_Num;               //闪烁次数
    uint8_t  Flicker_NumCount;          //闪烁次数计数器
    uint16_t ONOFF_Speed;               //亮灭频率
    uint16_t Flicker_Speed;             //闪烁频率
    uint16_t MoreFlicker_Speed;         //多闪烁频率
    uint32_t TIME;
    void  (*separate_led_Level)(uint8_t level); /* 按键按下的判断函数*/
    struct LED_T* next;
}LED_T;
/* 宏定义 --------------------------------------------------------------------*/
#define TICKS_INTERVAL    1	//ms



/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/

void SingleLed_init(LED_T* handle, 
                    void(*pin_level)(uint8_t),
                    uint8_t active_level
                   );
void SingleLed_time(LED_T* handle, 
                    uint8_t Flicker_Num,
                    uint16_t Flicker_Speed,
                    uint16_t ONOFF_Speed,
                    uint16_t MoreFlicker_Speed
                   );                    
int SingleLed_start(LED_T* handle);
void SingleLed_stop(LED_T* handle);
void SingleLed_ticks(void);
                    
                    
                    
#endif  // __separate_led_H__


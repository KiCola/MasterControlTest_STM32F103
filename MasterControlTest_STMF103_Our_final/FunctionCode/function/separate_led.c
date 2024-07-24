/* 包含头文件 ----------------------------------------------------------------*/
#include "separate_led.h"
#include "stdint.h"
#include "string.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
static struct LED_T* head_handle = NULL;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

static void SingleLed_handler(LED_T* handle)
{
    if((0 == handle->ONOFF_Speed)
     &&((handle->level_now != handle->lighten_level)||(0 == handle->Flicker_Speed)))//常亮
    {
        handle->level_now = handle->lighten_level;
    }
    else if((0 == handle->Flicker_Speed)&&(handle->level_now == handle->lighten_level))//长灭
    {
        handle->level_now = !handle->lighten_level;
    }
    else
    {
        handle->TIME++;
        switch(handle->state)
        {
            case 0:
                if(handle->TIME == handle->Flicker_Speed)
                {
                    handle->level_now = handle->lighten_level;
                }
                else if(handle->TIME >= (handle->Flicker_Speed+handle->ONOFF_Speed))
                {
                    handle->level_now = !handle->lighten_level;
                    handle->TIME = 0;
                    if((handle->Flicker_Num != 0)&&(handle->MoreFlicker_Speed != 0))
                    {
                        handle->Flicker_NumCount++;
                        if(handle->Flicker_NumCount == handle->Flicker_Num)
                        {
                            handle->state = 1; 
                            handle->Flicker_NumCount = 0;
                        }
                    }
                }
                break;
            case 1:
                if(handle->TIME >= (handle->MoreFlicker_Speed-1))
                {
                    handle->state = 0;
                    handle->TIME = (handle->Flicker_Speed-1);
                }
                break;
        }
    }
    if(handle->level_old != handle->level_now)
    {
        handle->separate_led_Level(handle->level_now);
        handle->level_old = handle->level_now;
    }
}

void SingleLed_init(LED_T* handle, 
                    void(*pin_level)(uint8_t),
                    uint8_t active_level
                   )
{
    memset(handle, 0, sizeof(LED_T));
	handle->separate_led_Level = pin_level;
    handle->lighten_level = active_level;
    handle->level_now = !handle->lighten_level;
    handle->separate_led_Level(handle->level_now);
    handle->level_old = handle->level_now;
}


void SingleLed_time(LED_T* handle, 
                    uint8_t Flicker_Num,
                    uint16_t Flicker_Speed,
                    uint16_t ONOFF_Speed,
                    uint16_t MoreFlicker_Speed
                   )
{
    handle->Flicker_Num = Flicker_Num;
    handle->Flicker_Speed = Flicker_Speed/TICKS_INTERVAL;
    handle->ONOFF_Speed = ONOFF_Speed/TICKS_INTERVAL;
    handle->MoreFlicker_Speed = MoreFlicker_Speed/TICKS_INTERVAL;
}


int SingleLed_start(LED_T* handle)
{
	LED_T* target = head_handle;
    handle->TIME = 0;
	while(target) 
    {
		if(target == handle) 
            return -1;	//already exist.
		target = target->next;
	}
	handle->next = head_handle;
	head_handle = handle;
	return 0;
}

void SingleLed_stop(LED_T* handle)
{
	LED_T** curr;
	LED_T* entry;
	curr = &head_handle;
	while(*curr)
	{
		entry = *curr;
		if(handle == entry)
		{
            handle->separate_led_Level(!handle->lighten_level);
			*curr = entry->next;
			break;
		}
		else
		{
			curr = &entry->next;
		}
	}
}
void SingleLed_ticks(void)
{
	LED_T* target;
	for(target=head_handle; target; target=target->next) 
    {
		SingleLed_handler(target);
	}
}



#include "separate_button.h"





static struct KEY_T* head_handle = NULL;

static void button_handler(KEY_T* handle)
{
	uint8_t read_gpio_level = handle->separate_button_Level();
	uint8_t state_val;
	uint8_t i,mask;
	/*------------button debounce handle---------------*/
	if(read_gpio_level != handle->button_level) //not equal to prev one
	{ 
		//continue read 3 times same new level change
		if(++(handle->debounce_cnt) >= DEBOUNCE_TICKS) {
			handle->button_level = read_gpio_level;
			handle->debounce_cnt = 0;
		}
	} 
	else //leved not change ,counter reset.
	{ 
		handle->debounce_cnt = 0;
	}
    /*-----------------State machine-------------------*/
	handle->ReadData = (handle->button_level == handle->active_level);
	handle->Trg = handle->ReadData & (handle->ReadData ^ handle->Cont);
	handle->Cont = handle->ReadData; 
	
	state_val = (handle->ReadData+handle->Trg+handle->Cont);
	switch(state_val)
	{
        case 0:
		    /************松开事件*******************/
		    if(handle->state_old > 0)
			{
				handle->signal |= SIGNAL_PRESS_UP; 	
			}
			/************多按事件*******************/
			if(handle->RepeatCount < handle->RepeatSpeed)
			{
				handle->RepeatCount++;
			}
			else if(handle->RepeatNum != 0)
			{
                handle->signal |= SIGNAL_PRESS_UP_DELAY;
				if(1 == handle->RepeatNum)
				{
					handle->signal |= SIGNAL_SINGLE_CLICK; 	
				}
				else if(2 == handle->RepeatNum)
				{
					handle->signal |= SIGNAL_DOUBLE_CLICK;
				}
                else
                {
                    handle->RepeatNumOld = handle->RepeatNum;
                    handle->signal |= SIGNAL_MUCH_CLICK;
                }
				handle->RepeatNum = 0;
			}
			break;
		case 3: 
		    /************按下事件*******************/
		    handle->signal |= SIGNAL_PRESS_DOWN; 
			/************长按事件*******************/
			handle->LongCount = 0;
			/************多按事件*******************/
			handle->RepeatNum += 1;
			handle->RepeatCount = 0;
		    break;
		case 2:
		    /************长按事件*******************/
		    handle->LongCount++;
			if(handle->LongTime != 0)
			{
				if(handle->LongCount >= handle->LongTime)
				{
					handle->signal |= SIGNAL_LONG_PRESS_HOLD;
				    handle->LongCount = 0;	
				}
			}
		    break;
	}
	handle->state_old = state_val;
	if(handle->signal)
	{
		mask=1;
		for (i=0;i<number_of_event;i++)
		{
			if (handle->signal&mask)
            {
				if(handle->cb[i] != 0)
				{
                    handle->event_flg = mask;
                    handle->cb[i](handle);
				}
                handle->signal &= ~mask;
            }
            mask<<=1;
		}
	}
}


void button_init(KEY_T* handle, uint8_t(*pin_level)(void),uint8_t active_level,uint16_t RepeatSpeed,uint16_t LongTime)
{
    memset(handle, 0, sizeof(KEY_T));
	handle->separate_button_Level = pin_level;
	handle->button_level = handle->separate_button_Level();
	handle->active_level = active_level;
	handle->RepeatSpeed = RepeatSpeed/KEYTICKS_INTERVAL;
	handle->LongTime = LongTime/KEYTICKS_INTERVAL;
}

void button_time(KEY_T* handle,uint16_t RepeatSpeed,uint16_t LongTime)
{
    handle->RepeatSpeed = RepeatSpeed/KEYTICKS_INTERVAL;
	handle->LongTime = LongTime/KEYTICKS_INTERVAL;
}

void button_attach(KEY_T* handle, PressEvent event, BtnCallback cb)
{
	handle->cb[event] = cb;
}

int button_start(KEY_T* handle)
{
	KEY_T* target = head_handle;
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
void button_stop(KEY_T* handle)
{
	KEY_T** curr;
	KEY_T* entry;
	curr = &head_handle;
	while(*curr)
	{
		entry = *curr;
		if(handle == entry)
		{
			*curr = entry->next;
			break;
		}
		else
		{
			curr = &entry->next;
		}
	}
}
void button_ticks(void)
{
	KEY_T* target;
	for(target=head_handle; target; target=target->next) 
    {
		button_handler(target);
	}
}










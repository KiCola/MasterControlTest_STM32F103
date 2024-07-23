#ifndef _Separate_BUTTON_H_
#define _Separate_BUTTON_H_




#include "stdint.h"
#include "string.h"



typedef void (*BtnCallback)(void*);



typedef enum 
{
    PRESS_DOWN = 0,//按下
    PRESS_UP,      //松开
    SINGLE_CLICK,  //单击
    DOUBLE_CLICK,  //双击
    MUCH_CLICK,    //多击
    LONG_PRESS_HOLD,//长按保持
    PRESS_UP_DELAY, //松开延时
	number_of_event,
	NONE_PRESS
}PressEvent;


typedef struct KEY_T
{
	uint8_t  (*separate_button_Level)(void); /* 按键按下的判断函数*/
	uint8_t  debounce_cnt :3;         //防反跳计数
	uint8_t  active_level :1;          //按下的状态
	uint8_t  button_level :1;          //上一次按钮状态
	uint8_t  ReadData     :1;
	uint8_t  Trg          :1;
    uint8_t  Cont         :1;
	uint8_t  signal;
    uint8_t  event_flg;
	uint8_t  state_old;
	uint8_t  RepeatNum;	
    uint8_t  RepeatNumOld;	
    uint16_t  RepeatCount;	/* 连续按键计数器 */
	uint16_t  RepeatSpeed;	/* 连续按键周期 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	struct KEY_T* next;
	BtnCallback  cb[number_of_event];   //事件函数	
}KEY_T;

//According to your need to modify the constants.
#define KEYTICKS_INTERVAL    1	//ms
#define DEBOUNCE_TICKS    7	//MAX 7


#define SIGNAL_PRESS_DOWN		 (1<<PRESS_DOWN)
#define SIGNAL_PRESS_UP			 (1<<PRESS_UP)
#define SIGNAL_SINGLE_CLICK	     (1<<SINGLE_CLICK)
#define SIGNAL_DOUBLE_CLICK	     (1<<DOUBLE_CLICK)
#define SIGNAL_MUCH_CLICK        (1<<MUCH_CLICK)
#define SIGNAL_LONG_PRESS_HOLD	 (1<<LONG_PRESS_HOLD)
#define SIGNAL_PRESS_UP_DELAY	 (1<<PRESS_UP_DELAY)

void button_init(KEY_T* handle, uint8_t(*pin_level)(void),uint8_t active_level,uint16_t RepeatSpeed,uint16_t LongTime);
void button_time(KEY_T* handle,uint16_t RepeatSpeed,uint16_t LongTime);
void button_attach(KEY_T* handle, PressEvent event, BtnCallback cb);
int button_start(KEY_T* handle);
void button_stop(KEY_T* handle);
void button_ticks(void);




#endif

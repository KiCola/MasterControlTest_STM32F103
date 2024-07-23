/* 包含头文件 ----------------------------------------------------------------*/
#include "app.h"
#include "data.h"
#include "separate_button.h"
/* 私有类型定义 --------------------------------------------------------------*/
#define WINDOW_NUM  9 //液晶屏显示页数

#define SERVO_NUM_MAX 6
#define MOTOR_NUM_MAX 4
#define TRACKINGSENSOR_NUM_MAX 4

#define SIZE 12
#define W25Q16_FLASH_SIZE ((uint32_t)(4*1024*1024))	//FLASH 大小为8M字节

#define USART_RX_LEN  	20
#define USART_REC_LEN  	200
#define UART_TIME       2
#define VALID_NUM       1

#define STEPPINGMOTORLIST 3
#define WORKMODE_SPEED  0
#define WORKMODE_STEP   1
/* 私有宏定义 ----------------------------------------------------------------*/
typedef enum
{
	DisplayAdd = 0,
	DisplayMinus = 1,
}_e_DisplayOperate;
/* 私有变量 ------------------------------------------------------------------*/
uint8_t in_data;
/*******时间数据声明*******/
uint32_t Time_Cnt = 0;
uint32_t Time_buff = 0;
/*******w25q16数据声明*******/
uint8_t w25q16_flg;
uint8_t testdata[SIZE] = "Hello world";
uint8_t datatemp[SIZE];
/*******串口数据声明*******/
uint8_t  USART_RX_BUF[USART_REC_LEN];
uint16_t USART_RX_NUM=0;
uint8_t  USART_RX_sj=0;
uint8_t USART2_RX_BUF[USART_RX_LEN]; 
uint8_t USART3_RX_BUF[USART_RX_LEN];
uint8_t uart1displayflg = 0;
uint8_t uart2displayflg = 0;
uint8_t uart3displayflg = 0;
uint8_t uartttldisplayflg = 0;
uint8_t ttland232flg;
uint8_t ttland232flgbuff;
/*******独立按键数据声明*******/
static KEY_T KEY1_S;
static KEY_T KEY2_S;
static KEY_T KEY3_S;
static KEY_T KEY4_S;
static KEY_T KEY5_S;
static KEY_T KEY6_S;
static KEY_T KEY7_S;
/*******步进电机驱动模块数据声明*******/
SteppingMotorDef_t SteppingMotor_S;
/*******直流电机驱动模块数据声明*******/
DCMotorMiniwattDef_t DCMotorMiniwatt1_S;
DCMotorMiniwattDef_t DCMotorMiniwatt2_S;
DCMotorMiniwattDef_t DCMotorMiniwatt3_S;
DCMotorMiniwattDef_t DCMotorMiniwatt4_S;
int16_t SpeedValue[MOTOR_NUM_MAX] = {0,0,0,0};
int16_t SpeedValuebuff[MOTOR_NUM_MAX] = {0xffff,0xffff,0xffff,0xffff};
int16_t ReadSpeedValue[MOTOR_NUM_MAX] = {0,0,0,0};
int16_t ReadSpeedValuebuff[MOTOR_NUM_MAX] = {0xffff,0xffff,0xffff,0xffff};
/*******巡线条模块数据声明*******/
uint8_t* adcval_head;
TrackingDef_t Tracking_Device1;
TrackingDef_t Tracking_Device2;
TrackingDef_t Tracking_Device3;
TrackingDef_t Tracking_Device4;
uint8_t TrackingState[TRACKINGSENSOR_NUM_MAX] = {TRACKING_MODE_WORK,TRACKING_MODE_WORK,TRACKING_MODE_WORK,TRACKING_MODE_WORK};
uint8_t TrackingValue[TRACKINGSENSOR_NUM_MAX] = {0,0,0,0};
uint16_t TrackingValuebuff[TRACKINGSENSOR_NUM_MAX] = {0xffff,0xffff,0xffff,0xffff};
/*******6通道舵机模块数据声明*******/
SteeringEngine6CHDef_t Servo_Device;
uint16_t AngleValue[SERVO_NUM_MAX] = {1500,1500,1500,1500,1500,1500};
uint16_t AngleValuebuff[SERVO_NUM_MAX] = {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff};
uint16_t AngleTimeValue[SERVO_NUM_MAX] = {2000,2000,2000,2000,2000,2000};
/*******液晶显示变量*******/
static void Display_Window_Clear(void);
static void Display_Window_Test(void);
static void Display_Window_TrackingSensor(void);
static void Display_Window_Servo(void);
static void Display_Window_DCBrushMotor(void);
static void Display_Window_StepMotor(void);
static void Display_Window_TrackingSensorBool(void);
static void Display_Window_Sensor(void);
static void Display_Window_PS2(void);
const VoidFuncVoid DisplayWindow[WINDOW_NUM]=
{
	Display_Window_Clear,
	Display_Window_Test,
	Display_Window_TrackingSensor,
	Display_Window_Servo,
	Display_Window_DCBrushMotor,
	Display_Window_StepMotor,
	Display_Window_TrackingSensorBool,
	Display_Window_PS2,
	Display_Window_Sensor,
};
const uint8_t display[][16]=
{
	{"Servo1 Angle:"},  //0
	{"Servo2 Angle:"},  //1
	{"Servo3 Angle:"},  //2
	{"Servo4 Angle:"},  //3
	{"Servo5 Angle:"},  //4
	{"Servo6 Angle:"},  //5
	{"Motor1SetSpd:"},  //6
	{"Motor2SetSpd:"},  //7
	{"Motor3SetSpd:"},  //8
	{"Motor4SetSpd:"},  //9
	{"RSpd1:"},  //10
	{"RSpd2:"},  //11
	{"RSpd3:"},  //12
	{"RSpd4:"},  //13
	{"Tracking1:"},  //14
	{"Tracking2:"},  //15
	{"Tracking3:"},  //16
	{"Tracking4:"},  //17
	{"UT Data:"},    //18
	{"Motor WorkMode:"},//19
	{"Motor Speed_V0:"},//20
	{"Motor Speed_Vt:"},//21
	{"Motor Step_Out:"},//22
};
uint8_t lcd_page = 0;
uint8_t lcd_line = 0;
uint8_t lcd_linebuff = 0xff;
uint8_t window_flg = 0;
VoidFuncVoid LCD_Display_Hand = 0;
/*******按键标记*******/
uint8_t keymark = 0;
/*******步进电机数据*******/
uint8_t  WorkMode = WORKMODE_STEP;
int16_t  speed_v0 = 2000;     
int16_t  speed_vt = 6000;     
uint16_t step_out = 200; 

static uint8_t  WorkModebuff = 0xff;
static int16_t  speed_v0buff = 0xff;     
static int16_t  speed_vtbuff = 0xff;     
static uint16_t step_outbuff = 0xff;
/* 扩展变量 ------------------------------------------------------------------*/



/* 函数体 --------------------------------------------------------------------*/
/***********************************************************************************************

                                       硬件配置函数

************************************************************************************************/
static u8 Tim_CheckTimeOut(u32 TimeStart, u32 TimeNow, u32 TimeOut)
{
  #define ActivateCounter ((u32)(TimeOut+TimeStart))
  if (ActivateCounter >= TimeStart)
  {
    if ((TimeNow >= ActivateCounter) ||
        (TimeNow < TimeStart))
    {
      return TRUE;
    }
  }
  else if ((TimeNow >= ActivateCounter) &&
           (TimeNow < TimeStart))
  {
    return TRUE;
  }
  return FALSE;
}
void UartTime(void)
{
	//说明串口已经结束发送
 	if((USART_RX_sj&0x7f) >= UART_TIME) 
	{
        if(USART_RX_NUM >= VALID_NUM) 
		{
			if(uart1displayflg == 0)
			{
				Bsp_UART485_Transmit(USART_RX_BUF,USART_RX_NUM); 
			}
			USART_RX_BUF[USART_RX_NUM] = 0;
			uart1displayflg = 2;
		}
		USART_RX_NUM = 0;//字节计数清零
        USART_RX_sj = 0; //计时清零
	}
	//USART_RX_sj等于10说明已经接收到第一个数据了
   else if((USART_RX_sj&0x80) == 0x80)
	{
        USART_RX_sj++;
	} 
}
void SCIRW(void)
{
    if(USART_RX_NUM >= USART_REC_LEN)//接收数据长度超标
    {
        USART_RX_NUM=0;
    }
	USART_RX_sj = 0x80;
	USART_RX_BUF[USART_RX_NUM] = RxBuffer;
    USART_RX_NUM++;
}
/*******电机控制*******/
static void DCMotor_Ctrl_Scan(void)
{
//	if((LCD_Display_Hand == Display_Window_DCBrushMotor)
//	 &&(WirelessData_U.DATE.time >= 300))
	if(LCD_Display_Hand == Display_Window_DCBrushMotor)
	{
		SLAVE_DCMotorMiniwatt_SpeedSet(&DCMotorMiniwatt1_S,SpeedValue[0]);
		SLAVE_DCMotorMiniwatt_SpeedSet(&DCMotorMiniwatt2_S,SpeedValue[1]);
		SLAVE_DCMotorMiniwatt_SpeedSet(&DCMotorMiniwatt3_S,SpeedValue[2]);
		SLAVE_DCMotorMiniwatt_SpeedSet(&DCMotorMiniwatt4_S,SpeedValue[3]);
	}
	ReadSpeedValue[0] = SLAVE_DCMotorMiniwatt_SpeedRead(&DCMotorMiniwatt1_S);
	ReadSpeedValue[1] = SLAVE_DCMotorMiniwatt_SpeedRead(&DCMotorMiniwatt2_S);
	ReadSpeedValue[2] = SLAVE_DCMotorMiniwatt_SpeedRead(&DCMotorMiniwatt3_S);
	ReadSpeedValue[3] = SLAVE_DCMotorMiniwatt_SpeedRead(&DCMotorMiniwatt4_S);
}

/*******循迹传感器读取*******/
static void TrackingSensor_Ctrl_Scan(void)
{
    TrackingValue[0] = SLAVE_Tracking_BoolRead(&Tracking_Device1);
	TrackingValue[1] = SLAVE_Tracking_BoolRead(&Tracking_Device2);
	TrackingValue[2] = SLAVE_Tracking_BoolRead(&Tracking_Device3);
	TrackingValue[3] = SLAVE_Tracking_BoolRead(&Tracking_Device4);
	TrackingState[0] = SLAVE_Tracking_ModeRead(&Tracking_Device1);
	TrackingState[1] = SLAVE_Tracking_ModeRead(&Tracking_Device2);
	TrackingState[2] = SLAVE_Tracking_ModeRead(&Tracking_Device3);
	TrackingState[3] = SLAVE_Tracking_ModeRead(&Tracking_Device4);
}
/*******舵机控制*******/
static void Servo_Ctrl_Scan(void)
{
	SLAVE_SteeringEngine6CH_MoreMotorControl(&Servo_Device,
                                           AngleValue[0],AngleTimeValue[0],
									       AngleValue[1],AngleTimeValue[1],
                                           AngleValue[2],AngleTimeValue[2],
										   AngleValue[3],AngleTimeValue[3],
									       AngleValue[4],AngleTimeValue[4],
                                           AngleValue[5],AngleTimeValue[5]);                           
}
/***********************************************************************************************

                                       应用任务函数

************************************************************************************************/
static void Display_Data_Init(void)
{
	lcd_page = 0;
	lcd_line = 0;
	lcd_linebuff = 0xff;
	window_flg = 0;
	LCD_Display_Hand = DisplayWindow[0];
}
/*******LCD显示*******/
static void Display_Window_Clear(void) //清屏显示
{
	if(0 == window_flg)
	{
		LCD_Clear(WHITE);//清屏
		lcd_line = 0;//行到初始值
		lcd_linebuff = 0xff; 
		if(lcd_page == 0)
		{//如果是清屏页转换到第一数据页
			lcd_page = 1;
		}
		switch(lcd_page)
		{
			case 3:  //舵机数据显示缓存清除
				AngleValuebuff[0] = 0xffff;
				AngleValuebuff[1] = 0xffff;
				AngleValuebuff[2] = 0xffff;
				AngleValuebuff[3] = 0xffff;
				AngleValuebuff[4] = 0xffff;
				AngleValuebuff[5] = 0xffff;
				break;
			case 4:  //直流数据显示缓存清除
				SpeedValuebuff[0] = 0xffff;
				SpeedValuebuff[1] = 0xffff;
				SpeedValuebuff[2] = 0xffff;
				SpeedValuebuff[3] = 0xffff;
			    ReadSpeedValuebuff[0] = 0xffff;
				ReadSpeedValuebuff[1] = 0xffff;
				ReadSpeedValuebuff[2] = 0xffff;
				ReadSpeedValuebuff[3] = 0xffff;
				break;
			case 5:
				WorkModebuff = 0xff;
				speed_v0buff = 0xff;     
				speed_vtbuff = 0xff;     
				step_outbuff = 0xff;
				break;
			case 2: 
			case 6:  //循迹数据显示缓存清除
			    TrackingValuebuff[0] = 0xffff;
				TrackingValuebuff[1] = 0xffff;
				TrackingValuebuff[2] = 0xffff;
				TrackingValuebuff[3] = 0xffff;
				break;
		}
		LCD_Display_Hand = DisplayWindow[lcd_page];
		window_flg = 1;
	}
}
static void Display_Window_Test(void)//主控测试界面
{
	static uint8_t DATA[3];
	uint8_t i;
	
	if(lcd_linebuff != lcd_line)
	{
		lcd_linebuff = lcd_line;
		Gui_StrCenter(2,2+36,RED,WHITE,"KEY:",12,0);
		if(w25q16_flg)
		{
			Gui_StrCenter(2, 2+12, BLACK, WHITE, "W25Q32 NO !",12,0);
		}
		else
		{
			Gui_StrCenter(2, 2+12, BLACK, WHITE, "W25Q32 YES!",12,0);
			Gui_StrCenter(2, 2+24, BLACK, WHITE, datatemp,12,0);
		}
		Gui_StrCenter(2,2+76,RED,WHITE,"UART2:",12,0);
		Gui_StrCenter(2,2+88,RED,WHITE,"UART3:",12,0);
		Gui_StrCenter(2,2+100,RED,WHITE,"UART1:",12,0);
		Gui_StrCenter(2,2+112,RED,WHITE,"UART3TTL:",12,0);
		DATA[0] = 0XFF;
		DATA[1] = 0XFF;
		DATA[2] = 0XFF;
	}
	if(DATA[1] != ttland232flgbuff)
	{
		DATA[1] = ttland232flgbuff;
		if(DATA[1] == UART3_TTL)
			Gui_StrCenter(2, 2, BLACK, WHITE, "UART3_TTL",12,0);
		else
			Gui_StrCenter(2, 2, BLACK, WHITE, "UART3_232",12,0);
	}
	if(DATA[0] != keymark)
	{
		DATA[0] = keymark;
		LCD_ShowxNum(2+8*9,2+36,DATA[0],5,12,0,BLACK,WHITE);
	}
	
	if(uart2displayflg)
	{
		Gui_StrCenter(2+6*9,2+76,RED,WHITE,USART2_RX_BUF,12,0);
		uart2displayflg = 0;
	}
	if(uart3displayflg)
	{
		Gui_StrCenter(2+6*9,2+88,RED,WHITE,USART3_RX_BUF,12,0);
		uart3displayflg = 0;
	}
	if(uart1displayflg==2)
	{
		Gui_StrCenter(2+6*9,2+100,RED,WHITE,USART_RX_BUF,12,0);
		uart1displayflg = 0;
	}
	if(uartttldisplayflg == 2)
	{
		Gui_StrCenter(2+6*9,2+112,RED,WHITE,USART3_RX_BUF,12,0);
		uartttldisplayflg = 0;
	}
	if(in_data != DATA[2])
	{
		DATA[2] = in_data;
		for(i=0; i<4; i++)
		{
			if(((DATA[2]>>i)&0x01) == 0x01)
			{
				Gui_StrCenter(2+16*i,2+56, BLUE, WHITE, "●",16,0);
			}
			else
			{
				Gui_StrCenter(2+16*i,2+56, BLUE, WHITE, "○",16,0);
			}
		}
	}
}
static void Display_Window_TrackingSensor(void)
{
	uint8_t i;
	uint16_t Color;
	
	if(lcd_linebuff != lcd_line)
	{
		lcd_linebuff = lcd_line;
		SLAVE_Tracking_AdcModeEnable(&Tracking_Device1,&adcval_head);
		Gui_StrCenter(6,2, BLACK, WHITE, "Tracking Signal",16,0);
		Gui_StrCenter(6,25+20, BLACK, WHITE, "Tracking AdcVal",16,0);
		Gui_StrCenter(2, 2+16*4, BLACK, WHITE, "DATA0:",12,0);
		Gui_StrCenter(66, 2+16*4, BLACK, WHITE, "DATA1:",12,0);
	  
		Gui_StrCenter(2, 2+16*5, BLACK, WHITE, "DATA2:",12,0);
		Gui_StrCenter(66, 2+16*5, BLACK, WHITE, "DATA3:",12,0);
	  
		Gui_StrCenter(2, 2+16*6, BLACK, WHITE, "DATA4:",12,0);
		Gui_StrCenter(66, 2+16*6, BLACK, WHITE, "DATA5:",12,0);
	  
		Gui_StrCenter(2, 2+16*7, BLACK, WHITE, "DATA6:",12,0);
		Gui_StrCenter(66, 2+16*7, BLACK, WHITE, "DATA7:",12,0);
		Time_buff = Time_Cnt-100;
	}
	if(TrackingValue[0] != TrackingValuebuff[0])
	{
		TrackingValuebuff[0] = TrackingValue[0];
		for(i=0; i<8; i++)
		{
			if(TrackingState[0] == TRACKING_MODE_CALC)
				Color = RED;
			else
				Color = GOOD;
			if(((TrackingValuebuff[0]>>i)&0x01) == 0x01)
			{
				Gui_StrCenter(2+16*i,25, Color, WHITE, "●",16,0);
			}
			else
			{
				Gui_StrCenter(2+16*i,25, Color, WHITE, "○",16,0);
			}
		}
	}
	if(Tim_CheckTimeOut(Time_buff, Time_Cnt, 100))
	{
		
		LCD_ShowxNum(2+6*7, 2+16*4, adcval_head[0],3,12,0, BLACK, WHITE);
		LCD_ShowxNum(66+6*7, 2+16*4, adcval_head[1],3,12,0, BLACK, WHITE);

		LCD_ShowxNum(2+6*7, 2+16*5, adcval_head[2],3,12,0, BLACK, WHITE);
		LCD_ShowxNum(66+6*7, 2+16*5, adcval_head[3],3,12,0, BLACK, WHITE);
	  
		LCD_ShowxNum(2+6*7, 2+16*6, adcval_head[4],3,12,0, BLACK, WHITE);
		LCD_ShowxNum(66+6*7, 2+16*6, adcval_head[5],3,12,0, BLACK, WHITE);
	  
		LCD_ShowxNum(2+6*7, 2+16*7, adcval_head[6],3,12,0, BLACK, WHITE);
		LCD_ShowxNum(66+6*7, 2+16*7,adcval_head[7],3,12,0, BLACK, WHITE);	
	}
}
static void Display_Window_Servo(void) //舵机操作界面
{
	uint8_t i;
	
	if(lcd_linebuff != lcd_line)
	{
		lcd_linebuff = lcd_line;
		for(i=0; i<SERVO_NUM_MAX; i++)
		{
			if(lcd_linebuff == i)
			{
				Gui_StrCenter(2, 2+20*i, WHITE, BLACK, (uint8_t*)display[i],12,0);
			}
			else
			{
				Gui_StrCenter(2, 2+20*i, BLACK, WHITE, (uint8_t*)display[i],12,0);
			}
		}
	}
	/*******************舵机数值***********************/
	for(i=0; i<SERVO_NUM_MAX; i++)
	{
		if(AngleValuebuff[i] != AngleValue[i])
		{
			AngleValuebuff[i] = AngleValue[i];
			LCD_ShowxPMNum(2+6*14,2+(20*i),AngleValue[i],4,12,0,BLACK, WHITE);
		}
	}
}
static void Display_Window_DCBrushMotor(void) //直流电机操作界面
{
	uint8_t i;
	
	if(lcd_linebuff != lcd_line)
	{
		lcd_linebuff = lcd_line;
		for(i=0; i<MOTOR_NUM_MAX; i++)
		{
			if(lcd_linebuff == i)
			{
				Gui_StrCenter(2, 2+20*i, WHITE, BLACK, (uint8_t*)display[6+i],12,0);
			}
			else
			{
				Gui_StrCenter(2, 2+20*i, BLACK, WHITE, (uint8_t*)display[6+i],12,0);
			}
			Gui_StrCenter(2+64*(i%2), 2+20*(i/2+4), BLACK, WHITE, (uint8_t*)display[10+i],12,0);
		}
	}
	/*******************直流电机数值***********************/
	for(i=0; i<MOTOR_NUM_MAX; i++)
	{
		if(SpeedValuebuff[i] != SpeedValue[i])
		{
			SpeedValuebuff[i] = SpeedValue[i];
			LCD_ShowxPMNum(86,2+(20*i),SpeedValue[i],4,12,0,BLACK, WHITE);
		}
		if(ReadSpeedValuebuff[i] != ReadSpeedValue[i])
		{
			ReadSpeedValuebuff[i] = ReadSpeedValue[i];
			LCD_ShowxPMNum(40+64*(i%2), 2+20*(i/2+4),ReadSpeedValue[i],4,12,0,BLACK, WHITE);
		}
	}
}
static void Display_Window_StepMotor(void)
{
	s32 val;
	
	if(lcd_linebuff != lcd_line)
	{
		lcd_linebuff = lcd_line;
		for(uint8_t i=0; i<4; i++)
		{
			if(lcd_linebuff == i)
			{
				Gui_StrCenter(2, 2+20*i, WHITE, BLACK, (uint8_t*)display[i+19],12,0);
			}
			else
			{
				Gui_StrCenter(2, 2+20*i, BLACK, WHITE, (uint8_t*)display[i+19],12,0);
			}
		}
	}
	/***************工作模式*******************/
	if(WorkModebuff != WorkMode)
	{
		WorkModebuff = WorkMode;
		if(WORKMODE_SPEED == WorkModebuff)
		{
			Gui_StrCenter(2+6*16, 2, BLACK, WHITE, "Speed",12,0);
		}
		else
		{
			Gui_StrCenter(2+6*16, 2, BLACK, WHITE, "Step ",12,0);
		}
	}
	
	/***************初始速度显示*******************/
	if(speed_v0buff != speed_v0)
	{
		speed_v0buff = speed_v0;
		val = ABS(speed_v0buff);
		if(speed_v0buff < 0)
		{
			val = -val;
		}
		LCD_ShowxPMNum(2+6*15,2+20,val,6,12,0,BLACK, WHITE);
	}
	/***************最高速度显示*******************/
	if(speed_vtbuff != speed_vt)
	{
		speed_vtbuff = speed_vt;
		val = ABS(speed_vtbuff);
		if(speed_vtbuff < 0)
		{
			val = -val;
		}
		LCD_ShowxPMNum(2+6*15,2+40,val,6,12,0,BLACK, WHITE);
	}
	/***************步数显示*******************/
	if(step_outbuff != step_out)
	{
		step_outbuff = step_out;

		LCD_ShowxPMNum(2+6*15,2+60,step_outbuff,6,12,0,BLACK, WHITE);
	}
}
static void Display_Window_TrackingSensorBool(void) //循迹传感器操作界面
{
	uint8_t i,j;
	uint16_t Color;
	
	if(lcd_linebuff != lcd_line)
	{
		lcd_linebuff = lcd_line;
		for(i=0; i<TRACKINGSENSOR_NUM_MAX; i++)
		{
			if(lcd_linebuff == i)
			{
				Gui_StrCenter(2, 2+30*i, WHITE, BLACK, (uint8_t*)display[14+i],12,0);
			}
			else
			{
				Gui_StrCenter(2, 2+30*i, BLACK, WHITE, (uint8_t*)display[14+i],12,0);
			}
		}
	}
	/*******************循迹传感器数值***********************/
	for(i=0; i<TRACKINGSENSOR_NUM_MAX; i++)
	{
		if((TrackingValue[i] != TrackingValuebuff[i])
		 ||(TrackingState[i] == TRACKING_MODE_CALC))
		{
			if(TrackingState[i] == TRACKING_MODE_CALC)
			{
				Color = RED;
			}
			else
			{
				Color = GOOD;
			}
			TrackingValuebuff[i] = TrackingValue[i];
			for(j=0; j<8; j++)
			{
				if(((TrackingValuebuff[i]>>j)&0x01) == 0x01)
				{
					Gui_StrCenter(2+16*j,16+(30*i), Color, WHITE, "●",16,0);
				}
				else
				{
					Gui_StrCenter(2+16*j,16+(30*i), Color, WHITE, "○",16,0);
				}
			}
		}
	}
}
static void Display_Window_Sensor(void)
{
	if(lcd_linebuff != lcd_line)
	{
		lcd_linebuff = lcd_line;
		Gui_StrCenter(2, 2+20, BLACK, WHITE, (uint8_t*)display[18],12,0);
	}
	
//	if(DistanceValuebuff != DistanceValue)
//	{
//		DistanceValuebuff = DistanceValue;
//		LCD_Float(2+6*10,2+20,DistanceValue,6,BLACK, WHITE);
//	}
}
static void Display_Window_PS2(void)
{
	static uint8_t comflg = 0xf0;
	static uint8_t DATA[6];
	uint8_t i;
	
	if(lcd_linebuff != lcd_line)
	{
		lcd_linebuff = lcd_line;
		Gui_StrCenter(2,2+16,BLACK,WHITE,"LX:",12,0);
		Gui_StrCenter(66,2+16,BLACK,WHITE,"LY:",12,0);
		Gui_StrCenter(2,2+16*2,BLACK,WHITE,"RX:",12,0);
		Gui_StrCenter(66,2+16*2,BLACK,WHITE,"RY:",12,0);
		Gui_StrCenter(2,2+16*3,BLACK,WHITE,"SELECT:",12,0);
		Gui_StrCenter(66,2+16*3,BLACK,WHITE,"START:",12,0);
		Gui_StrCenter(2,2+16*4,BLACK,WHITE,"L1:",12,0);
		Gui_StrCenter(2+45,2+16*4,BLACK,WHITE,"L2:",12,0);
		Gui_StrCenter(2+90,2+16*4,BLACK,WHITE,"L3:",12,0);
		Gui_StrCenter(2,2+16*5,BLACK,WHITE,"R1:",12,0);
		Gui_StrCenter(2+45,2+16*5,BLACK,WHITE,"R2:",12,0);
		Gui_StrCenter(2+90,2+16*5,BLACK,WHITE,"R3:",12,0);
		comflg = 0xf0;
		for(i=0; i<2; i++)
		{
			DATA[i] = 0x5a;
		}
	}
	if(comflg != PS2_Data.DATE.ID)
	{
		comflg = PS2_Data.DATE.ID;
		if(comflg == 0X73)
		{//判断手柄是否为红灯模式，是，指示灯LED闪烁
			Gui_StrCenter(2,2,BLACK,WHITE,"PS2 RED ON ",12,0);
		}
		else if(comflg == 0X41)
		{//判断手柄不是红灯模式，指示灯LED点亮
			Gui_StrCenter(2,2,BLACK,WHITE,"PS2 RED OFF",12,0);
		}
		else
		{
			Gui_StrCenter(2,2,RED,WHITE,"PS2 OFF    ",12,0);
		}
	}
	if((PS2_Data.DATE.ID == 0X73) || (PS2_Data.DATE.ID == 0X41))
	{
		if(DATA[2] != PS2_Data.DATE.PSS_LX)
		{
			DATA[2] = PS2_Data.DATE.PSS_LX;
			LCD_ShowxNum(2+8*4,2+16,DATA[2],3,12,0,BLACK,WHITE);
		}
		if(DATA[3] != PS2_Data.DATE.PSS_LY)
		{
			DATA[3] = PS2_Data.DATE.PSS_LY;
			LCD_ShowxNum(66+8*4,2+16,DATA[3],3,12,0,BLACK,WHITE);
		}
		if(DATA[4] != PS2_Data.DATE.PSS_RX)
		{
			DATA[4] = PS2_Data.DATE.PSS_RX;
			LCD_ShowxNum(2+8*4,2+16*2,DATA[4],3,12,0,BLACK,WHITE);
		}
		if(DATA[5] != PS2_Data.DATE.PSS_RY)
		{
			DATA[5] = PS2_Data.DATE.PSS_RY;
			LCD_ShowxNum(66+8*4,2+16*2,DATA[5],3,12,0,BLACK,WHITE);
		}
		if(DATA[0] != PS2_Data.DATE.key1_U.byte)
		{
			DATA[0] = PS2_Data.DATE.key1_U.byte;
			if(PS2_Data.DATE.key1_U.bit.UP)
				Gui_StrCenter(2+20,2+16*6,BLACK,WHITE,"∧",12,0);
			else
				Gui_StrCenter(2+20,2+16*6,BLACK,WHITE,"↑",12,0);
			if(PS2_Data.DATE.key1_U.bit.DOWN)
				Gui_StrCenter(2+40,2+16*6,BLACK,WHITE,"∨",12,0);
			else
				Gui_StrCenter(2+40,2+16*6,BLACK,WHITE,"↓",12,0);
			if(PS2_Data.DATE.key1_U.bit.LEFT)
				Gui_StrCenter(2+60,2+16*6,BLACK,WHITE,"〈",12,0);
			else
				Gui_StrCenter(2+60,2+16*6,BLACK,WHITE,"←",12,0);
			if(PS2_Data.DATE.key1_U.bit.RIGHT)
				Gui_StrCenter(2+80,2+16*6,BLACK,WHITE,"〉",12,0);
			else 
				Gui_StrCenter(2+80,2+16*6,BLACK,WHITE,"→",12,0);
			LCD_ShowxNum(2+90+4*5,2+16*4,PS2_Data.DATE.key1_U.bit.L3,1,12,0,BLACK,WHITE);
			LCD_ShowxNum(2+90+4*5,2+16*5,PS2_Data.DATE.key1_U.bit.R3,1,12,0,BLACK,WHITE);
			LCD_ShowxNum(2+6*7,2+16*3,PS2_Data.DATE.key1_U.bit.SELECT,1,12,0,BLACK,WHITE);
			LCD_ShowxNum(66+6*7,2+16*3,PS2_Data.DATE.key1_U.bit.START,1,12,0,BLACK,WHITE);
		}
		if(DATA[1] != PS2_Data.DATE.key2_U.byte)
		{
			DATA[1] = PS2_Data.DATE.key2_U.byte;
			LCD_ShowxNum(2+4*5,2+16*4,PS2_Data.DATE.key2_U.bit.L1,1,12,0,BLACK,WHITE);
			LCD_ShowxNum(2+4*5,2+16*5,PS2_Data.DATE.key2_U.bit.R1,1,12,0,BLACK,WHITE);
			LCD_ShowxNum(2+45+4*5,2+16*4,PS2_Data.DATE.key2_U.bit.L2,1,12,0,BLACK,WHITE);
			LCD_ShowxNum(2+45+4*5,2+16*5,PS2_Data.DATE.key2_U.bit.R2,1,12,0,BLACK,WHITE);
			if(PS2_Data.DATE.key2_U.bit.CIRCLE)
				Gui_StrCenter(2+20,2+16*7,BLACK,WHITE,"○",12,0);
			else
				Gui_StrCenter(2+20,2+16*7,BLACK,WHITE,"●",12,0);
			if(PS2_Data.DATE.key2_U.bit.REC)
				Gui_StrCenter(2+40,2+16*7,BLACK,WHITE,"□",12,0);
			else
				Gui_StrCenter(2+40,2+16*7,BLACK,WHITE,"■",12,0);
			if(PS2_Data.DATE.key2_U.bit.TRI)
				Gui_StrCenter(2+60,2+16*7,BLACK,WHITE,"△",12,0);
			else
				Gui_StrCenter(2+60,2+16*7,BLACK,WHITE,"▲",12,0);
			if(PS2_Data.DATE.key2_U.bit.FORK)
				Gui_StrCenter(2+80,2+16*7,BLACK,WHITE,"×",12,0);
			else 
				Gui_StrCenter(2+80,2+16*7,BLACK,WHITE,"※",12,0);
		}
	}
}
static void Display_Data(void)  //显示数据
{
	if(LCD_Display_Hand != 0)
	{
		LCD_Display_Hand();
	}
}
static void Display_TurnPage(_e_DisplayOperate mode) //翻页操作
{
	uint8_t err;//标记为页是否发生改变

	if((1 == window_flg)&&(lcd_linebuff == lcd_line))
	{
		err = 1;	
		if((DisplayAdd == mode)&&(lcd_page < (WINDOW_NUM-1))) 
		{//最大为（WINDOW_NUM-1）页
			err = 0;
			lcd_page++;
		}
		else if((DisplayMinus == mode)&&(lcd_page > 1))
		{//最小为第一页，0页为清屏页，别的才为数据页
			err = 0;
			lcd_page--;
		}
		if(0 == err)
		{
			window_flg = 0;
			LCD_Display_Hand = DisplayWindow[0];//开始清屏准备换页
		}
	}
}
static void Display_NewLine(_e_DisplayOperate mode) //换行操作
{
	uint8_t nummax;
	
	//区别是哪一个显示界面
	if(LCD_Display_Hand == Display_Window_Servo)
	{
		nummax = SERVO_NUM_MAX-1;//6个，编号0~5
	}
	else if(LCD_Display_Hand == Display_Window_DCBrushMotor)
	{
		nummax = MOTOR_NUM_MAX-1;//4个，编号0~3
	}
	else if(LCD_Display_Hand == Display_Window_StepMotor)
	{
		nummax = 4-1;//4个，编号0~3
	}
	if((LCD_Display_Hand == Display_Window_Servo) ||
	   (LCD_Display_Hand == Display_Window_DCBrushMotor) ||
	   (LCD_Display_Hand == Display_Window_StepMotor) ||
	   (LCD_Display_Hand == Display_Window_TrackingSensorBool))
	{
		if((DisplayAdd == mode)&&(lcd_line < nummax))
		{
			lcd_line++;
		}
		else if((DisplayMinus == mode)&&(lcd_line > 0))
		{
			lcd_line--;
		}
	}
}
static void Display_ChangeData(_e_DisplayOperate mode)
{
	void *datap;
	uint16_t DataMAX;
	int16_t  DataMin;
	uint16_t DataStep;
	
	//区别是哪一个显示界面
	if(LCD_Display_Hand == Display_Window_Servo)
	{
		if(lcd_line >= SERVO_NUM_MAX)
		{
			lcd_line = SERVO_NUM_MAX-1;
		}
		datap = &AngleValue[lcd_line];
		DataMAX = 2500;
		DataMin = 500;
		DataStep = 10;
	}
	else if(LCD_Display_Hand == Display_Window_DCBrushMotor)
	{
		if(lcd_line >= MOTOR_NUM_MAX)
		{
			lcd_line = MOTOR_NUM_MAX-1;
		}
		datap = &SpeedValue[lcd_line];
		DataMAX = 800;
		DataMin = -800;
		DataStep = 20;
	}
	if(DisplayAdd == mode)
	{
		if(LCD_Display_Hand == Display_Window_Servo)
		{
			if(*((uint16_t*)datap) <= (DataMAX-DataStep))
			{
				*((uint16_t*)datap) += DataStep;
			}
			else if(*((uint16_t*)datap) < DataMAX)
			{
				*((uint16_t*)datap) = DataMAX;
			}
		}
		else if(LCD_Display_Hand == Display_Window_DCBrushMotor)
		{
			if(*((int16_t*)datap) <= (DataMAX-DataStep))
			{
				*((int16_t*)datap) += DataStep;
			}
			else if(*((int16_t*)datap) < DataMAX)
			{
				*((int16_t*)datap) = DataMAX;
			}
		}
		else if(LCD_Display_Hand == Display_Window_StepMotor)
		{
			switch(lcd_line)
			{
				case 0:
					if(WorkMode != WORKMODE_STEP)
					{
						WorkMode = WORKMODE_STEP;
					}
					else
					{
						WorkMode = WORKMODE_SPEED;
					}
					break;
				case 1:
					if(speed_v0 < 12000)
					{
						speed_v0 += 100;
						if(speed_v0 > 12000)
						{
							speed_v0 = 12000;
						}
					}
					break;
				case 2:
					if(speed_vt < 12000)
					{
						speed_vt += 100;
						if(speed_vt > 12000)
						{
							speed_vt = 12000;
						}
					}
					break;
				case 3:
					if(step_out < 60000)
					{
						step_out += 200;
						if(step_out > 60000)
						{
							step_out = 60000;
						}
					}
					break;
			}
		}
	}
	else if(DisplayMinus == mode)
	{
		if(LCD_Display_Hand == Display_Window_Servo)
		{
			if(*((uint16_t*)datap) >= (DataMin+DataStep))
			{
				*((uint16_t*)datap) -= DataStep;
			}
			else if(*((uint16_t*)datap) > DataMin)
			{
				*((uint16_t*)datap) = DataMin;
			}
		}
		else if(LCD_Display_Hand == Display_Window_DCBrushMotor)
		{
			if(*((int16_t*)datap) >= (DataMin+DataStep))
			{
				*((int16_t*)datap) -= DataStep;
			}
			else if(*((int16_t*)datap) > DataMin)
			{
				*((int16_t*)datap) = DataMin;
			}
		}
		else if(LCD_Display_Hand == Display_Window_StepMotor)
		{
			switch(lcd_line)
			{
				case 0:
					if(WorkMode != WORKMODE_SPEED)
					{
						WorkMode = WORKMODE_SPEED;
					}
					else
					{
						WorkMode = WORKMODE_STEP;
					}
					break;
				case 1:
					if(speed_v0 > (-12000))
					{
						speed_v0 -= 100;
						if(speed_v0 < (-12000))
						{
							speed_v0 =-12000;
						}
					}
					break;
				case 2:
					if(speed_vt > (-12000))
					{
						speed_vt -= 100;
						if(speed_vt < (-12000))
						{
							speed_vt =-12000;
						}
					}
					break;
				case 3:
					if(step_out > 40)
					{
						step_out -= 20;
					}
					else if(step_out > 20)
					{
						step_out = 20;
					}
					break;
			}
		}
	}
}
static void Display_ENTER(void)
{
	uint16_t stepnum;
	
	if((LCD_Display_Hand == Display_Window_TrackingSensorBool) ||
	   (LCD_Display_Hand == Display_Window_TrackingSensor))
	{
		switch(lcd_line)
		{
			case 0:
				SLAVE_Tracking_CheckModeEnable(&Tracking_Device1);
				break;
			case 1:
				SLAVE_Tracking_CheckModeEnable(&Tracking_Device2);
				break;
			case 2:
				SLAVE_Tracking_CheckModeEnable(&Tracking_Device3);
				break;
			case 3:
				SLAVE_Tracking_CheckModeEnable(&Tracking_Device4);
				break;
		}
	}
	else if(LCD_Display_Hand == Display_Window_StepMotor)
	{
		if(WorkMode != WORKMODE_STEP)
			{
				stepnum = 0;
			}
			else
			{
				stepnum = step_out;
			}
			SLAVE_SteppingMotor_Control(&SteppingMotor_S,
										speed_v0,     
										speed_vt,     
										stepnum,     
										6000
									   );
	}
}
/***********************************************************************************************

                                       应用事件（中断）函数

************************************************************************************************/
static void UART2BreakExecution_Handler(void)
{
	uint8_t i;
	if(USART2_RX_STA == SIZE)
	{
		for(i=0;i<SIZE;i++)
		{
			if(USART2_RX_BUF[i] != testdata[i])
			{
				break;
			}
		}
		if(i == SIZE)
		{
			uart2displayflg = 1;
		}
	}	
}
static void UART3BreakExecution_Handler(void)
{
	if(ttland232flg == UART3_232)
	{
		uart3displayflg = 1;
		Bsp_UART3_Transmit(USART3_RX_BUF,USART3_RX_STA);
	}
	else
	{
		if(uartttldisplayflg == 0)
		{
			Bsp_UART3_Transmit(USART3_RX_BUF,USART3_RX_STA); 
		}
		uartttldisplayflg = 2;
	}
	
}
/**
  * @brief  定时中断事件
  * @param  None
  * @retval None
  */

static void TimeBreakExecution_Handler(void) 
{
	static uint16_t time = 0;
	
	Time_Cnt++;
	//=========按键扫描函数==============================
	button_ticks();
	//=========循迹条数据=============================
	TrackingSensor_Ctrl_Scan();
	//=========直流电机控制===========================
	DCMotor_Ctrl_Scan();
	//=========舵机控制函数==============================
	time++;
	if(time == 20)
	{
		time = 0;
		Servo_Ctrl_Scan();
	}
	
}
void Time2BreakExecution_Handler(void)
{
	//=========CAN通讯协议============================
	Tim_GetCurrentTimeAdd_Scan1MS();    //必须1ms进行扫描此函数
	CANCommunication_Scan();            //最好也1ms进行扫描
}
/** @brief  按键事件
  * @param  None
  * @retval None
  */
static void KEY1_Handler(void* btn)
{
    switch(((KEY_T*)btn)->event_flg)
    {
        case SIGNAL_PRESS_DOWN:  
			if(LCD_Display_Hand != Display_Window_Test)
			{
				Display_TurnPage(DisplayAdd);
			}
			keymark = 1;
            break;
		case SIGNAL_DOUBLE_CLICK: 
			Display_TurnPage(DisplayAdd);
			break;
    }
}
static void KEY2_Handler(void* btn)
{
    switch(((KEY_T*)btn)->event_flg)
    {
        case SIGNAL_PRESS_DOWN:
			keymark = 2;
		    Display_ChangeData(DisplayAdd);
            break;
    }
}
static void KEY3_Handler(void* btn)
{
    switch(((KEY_T*)btn)->event_flg)
    {
        case SIGNAL_PRESS_DOWN: 
            Display_NewLine(DisplayAdd);			
			keymark = 3;
            break; 
    }
}
static void KEY4_Handler(void* btn)
{
    switch(((KEY_T*)btn)->event_flg)
    {
		case SIGNAL_PRESS_DOWN: 
			keymark = 4;
			break;
        case SIGNAL_SINGLE_CLICK:  
			Display_ENTER();
			
		    if(LCD_Display_Hand == Display_Window_Test)
			{
				if(ttland232flg == UART3_232)
					ttland232flg = UART3_TTL;
				else
					ttland232flg = UART3_232;
			}
            break;
		case SIGNAL_DOUBLE_CLICK:	
			if(LCD_Display_Hand == Display_Window_StepMotor)
			{
				SLAVE_SteppingMotor_Control(&SteppingMotor_S,0,0,0,1);	
			}				
			break;
    }
}
static void KEY5_Handler(void* btn)
{
    switch(((KEY_T*)btn)->event_flg)
    {
        case SIGNAL_PRESS_DOWN: 
			Display_NewLine(DisplayMinus);
		    if(LCD_Display_Hand == Display_Window_Test)
			{
				if(ttland232flgbuff == UART3_TTL)
				{
					uartttldisplayflg = 1;
					Bsp_UART3_Transmit(testdata,SIZE-1);
				}
				Bsp_UART485_Transmit(testdata,SIZE-1); 
				uart1displayflg = 1;
			}
			keymark = 5;
            break;
    }
}
static void KEY6_Handler(void* btn)
{
    switch(((KEY_T*)btn)->event_flg)
    {
        case SIGNAL_PRESS_DOWN:
			Display_TurnPage(DisplayMinus);
			keymark = 6;			
            break; 		
    }
}
static void KEY7_Handler(void* btn)
{
    switch(((KEY_T*)btn)->event_flg)
    {
        case SIGNAL_PRESS_DOWN: 
			keymark = 7;
		    Display_ChangeData(DisplayMinus);
            break;
    }
}
static void UP_Handler(void* btn)
{
	keymark = 0;
}
/***********************************************************************************************

                                       应用主函数

************************************************************************************************/
/**
  * @brief  应用初始化
  * @param  None
  * @retval None
  */
static void ApplicationProgram_Iint(void)
{ 
	/**********************按键初始化**************************/  
    button_init(&KEY1_S,KEY_1,0,300,2500);
	button_attach(&KEY1_S, PRESS_DOWN,KEY1_Handler);
	button_attach(&KEY1_S, DOUBLE_CLICK,KEY1_Handler);
    button_init(&KEY2_S,KEY_2,0,300,2500);
	button_attach(&KEY2_S, PRESS_DOWN,KEY2_Handler);
    button_init(&KEY3_S,KEY_3,0,300,2500);
	button_attach(&KEY3_S, PRESS_DOWN,KEY3_Handler);
    button_init(&KEY4_S,KEY_4,0,300,2500);
	button_attach(&KEY4_S, PRESS_DOWN,KEY4_Handler);
	button_attach(&KEY4_S, SINGLE_CLICK,KEY4_Handler);
	button_attach(&KEY4_S, DOUBLE_CLICK,KEY4_Handler);
    button_init(&KEY5_S,KEY_5,0,300,2500);
	button_attach(&KEY5_S, PRESS_DOWN,KEY5_Handler);
    button_init(&KEY6_S,KEY_6,0,300,2500);
	button_attach(&KEY6_S, PRESS_DOWN,KEY6_Handler);
    button_init(&KEY7_S,KEY_7,0,300,2500);
	button_attach(&KEY7_S, PRESS_DOWN,KEY7_Handler);
	
	button_attach(&KEY1_S, PRESS_UP,UP_Handler);
	button_attach(&KEY2_S, PRESS_UP,UP_Handler);
	button_attach(&KEY3_S, PRESS_UP,UP_Handler);
	button_attach(&KEY4_S, PRESS_UP,UP_Handler);
	button_attach(&KEY5_S, PRESS_UP,UP_Handler);
	button_attach(&KEY6_S, PRESS_UP,UP_Handler);
	button_attach(&KEY7_S, PRESS_UP,UP_Handler);
    
    button_start(&KEY1_S);
    button_start(&KEY2_S);
    button_start(&KEY3_S);
    button_start(&KEY4_S);
    button_start(&KEY5_S);
    button_start(&KEY6_S);
    button_start(&KEY7_S);
	/**********************数据初始化**************************/
	SLAVE_DCMotorMiniwatt_Init(&DCMotorMiniwatt1_S,1);//ID号
	SLAVE_DCMotorMiniwatt_Init(&DCMotorMiniwatt2_S,2);//ID号
	SLAVE_DCMotorMiniwatt_Init(&DCMotorMiniwatt3_S,3);//ID号3
	SLAVE_DCMotorMiniwatt_Init(&DCMotorMiniwatt4_S,4);//ID号4
	SLAVE_Tracking_Init(&Tracking_Device1,1,0x00); //初始化巡线条模块,ID为1
	SLAVE_Tracking_Init(&Tracking_Device2,2,0x00); //初始化巡线条模块,ID为2
	SLAVE_Tracking_Init(&Tracking_Device3,3,0x00); //初始化巡线条模块,ID为3
	SLAVE_Tracking_Init(&Tracking_Device4,4,0x00); //初始化巡线条模块,ID为4
	SLAVE_SteeringEngine6CH_Init(&Servo_Device,1); //初始舵机模块数据,ID为1
	SLAVE_SteppingMotor_Init(&SteppingMotor_S,STEPPINGMOTORLIST);
	/**********************PS2遥控初始化***********************/
	PS2_SetInit();
	/**********************通讯初始化**************************/
	ttland232flg = UART3_232;
	ttland232flgbuff = UART3_232;
	Bsp_USART3_MODE_Init(ttland232flg);
    Bsp_UART2_SetHandler(UART2BreakExecution_Handler,USART2_RX_BUF,USART_RX_LEN);
	Bsp_UART3_SetHandler(UART3BreakExecution_Handler,USART3_RX_BUF,USART_RX_LEN);
	Bsp_UART485_SetHandler(SCIRW);
	/**********************CAN初始化***************************/
	CANCommunication_Init();
	/**********************LCD初始化***************************/
    ST7735_LAIBAO177_INITIAL();
    DISPLAY_RGB();
    LCD_Clear(WHITE);
	Display_Data_Init();
	/**********************W25Q16初始化***************************/
	w25q16_flg = SPI_Flash_Init();
	if(w25q16_flg == 0)
	{
		SPI_Flash_Write(testdata,W25Q16_FLASH_SIZE-100,SIZE);     //从倒数第100个地址处开始,写入SIZE个字节
		SPI_Flash_Read(datatemp,W25Q16_FLASH_SIZE-100,SIZE);		//从倒数第100个地址处开始,读出SIZE个字节
	}
	/********************定时器初始化**************************/
	Timer_SetHandler(TimeBreakExecution_Handler);//1ms
	Timer2_SetHandler(Time2BreakExecution_Handler);//1ms
}

void ApplicationProgram_main(void)
{
	uint16_t time = 0;
	
    ApplicationProgram_Iint();
	

    while(1)
	{	
		if(ttland232flgbuff != ttland232flg)
		{
			ttland232flgbuff = ttland232flg;
			Bsp_USART3_MODE_Init(ttland232flgbuff);
		}
		UartTime();
		PS2_ClearData();
		PS2_ReadData();
		if(PS2_Data.DATE.ID == 0X73)
		{//判断手柄是否为红灯模式，是，指示灯LED闪烁
			LED1_Toggle();
		}
		else if(PS2_Data.DATE.ID == 0X41)
		{//判断手柄不是红灯模式，指示灯LED点亮
			LED1(bright);
		}
		else
		{
			LED1(extinguish);
		}
		HAL_Delay(50);	 //延时很重要不可去
		time++;
		if(time >= 10)
		{
			if(uart2displayflg == 0)
			{
				Bsp_UART2_Transmit(testdata,12);	
			}
			time = 0;
		}
		Display_Data();
		
		in_data = IN_1();
		in_data |= (IN_2()<<1);
		in_data |= (IN_3()<<2);
		in_data |= (IN_4()<<3);
	}
}

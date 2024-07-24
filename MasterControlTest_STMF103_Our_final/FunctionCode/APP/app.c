/* 包含头文件 ----------------------------------------------------------------*/
#include "app.h"
#include "data.h"
#include "separate_button.h"
/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
int16_t g_1_speed;
int16_t g_2_speed;
int16_t g_3_speed;
int16_t g_4_speed;

int16_t g_1_set_speed=0;
int16_t g_2_set_speed=0;
int16_t g_3_set_speed=0;
int16_t g_4_set_speed=0;

float speed_kp=0.1;
float speed_ki=0.01;

int16_t err=0;
uint8_t dir=1;

uint8_t captrue_state=0;
uint8_t catch_flag=0;
uint8_t count=0;
uint8_t end_state=0;

int pwm[3][6]={{1500,1500,2000,2000,2000,2140},{1500,1500,2000,1450,2150,1800},{1500,1500,2000,1450,2150,2140}};
int tim[6]={30,30,30,30,30,30};

/*******直流电机驱动模块数据声明*******/
DCMotorMiniwattDef_t motor1;
DCMotorMiniwattDef_t motor2;
DCMotorMiniwattDef_t motor3;
DCMotorMiniwattDef_t motor4;

SteeringEngine6CHDef_t servo;

/*******巡线条模块数据声明*******/
uint8_t* adcval_head;
TrackingDef_t Tracking_Device1;
TrackingDef_t Tracking_Device2;
TrackingDef_t Tracking_Device3;
TrackingDef_t Tracking_Device4;

uint8_t Tracking1;
uint8_t Tracking2;
uint8_t Tracking3;
uint8_t Tracking4;

/*******6通道舵机模块数据声明*******/

/*******液晶显示变量*******/


/* 扩展变量 ------------------------------------------------------------------*/



/* 函数体 --------------------------------------------------------------------*/
void get_wheel_speed(void)
{
	  g_1_speed=SLAVE_DCMotorMiniwatt_SpeedRead(&motor1);
	  g_2_speed=SLAVE_DCMotorMiniwatt_SpeedRead(&motor2);
	  g_3_speed=SLAVE_DCMotorMiniwatt_SpeedRead(&motor3);
	  g_4_speed=SLAVE_DCMotorMiniwatt_SpeedRead(&motor4);
}

void car_move(int8_t speed_x,int8_t speed_y)
{
	   g_1_set_speed=speed_x+speed_y;
	   g_2_set_speed=-speed_x+speed_y;
	   g_3_set_speed=speed_x+speed_y;
	   g_4_set_speed=-speed_x+speed_y;
}
void motor_control(int16_t g_1_set_speed,int16_t g_2_set_speed,int16_t g_3_set_speed,int16_t g_4_set_speed)
{
	  int speed1=0,speed2=0,speed3=0,speed4=0;
	
	  speed1=g_1_set_speed+err;
	  speed2=g_2_set_speed-err;
	  speed3=g_3_set_speed-err;
	  speed4=g_4_set_speed+err;
	
	  SLAVE_DCMotorMiniwatt_SpeedSet(&motor1,speed1);
	  SLAVE_DCMotorMiniwatt_SpeedSet(&motor2,-speed2);
	  SLAVE_DCMotorMiniwatt_SpeedSet(&motor3,-speed3);
	  SLAVE_DCMotorMiniwatt_SpeedSet(&motor4,speed4);
}
/*
void motor_control(int16_t g_1_set_speed,int16_t g_2_set_speed,int16_t g_3_set_speed,int16_t g_4_set_speed)
{
	  static float integral_1=0,integral_2= 0,integral_3= 0,integral_4 = 0;
	  int16_t err_1,err_2,err_3,err_4;
	  int speed1=0,speed2=0,speed3=0,speed4=0;
	
	  err_1 = g_1_set_speed - g_1_speed;
	  err_2 = g_2_set_speed - g_2_speed;
	  err_3 = g_3_set_speed - g_3_speed;
	  err_4 = g_4_set_speed - g_4_speed;
	
	  integral_1 += err_1;
    integral_2 += err_2;
    integral_3 += err_3;
    integral_4 += err_4;
	 
	  speed1=speed_kp*err_1+speed_ki*integral_1;
	  speed2=speed_kp*err_2+speed_ki*integral_2;
	  speed3=speed_kp*err_3+speed_ki*integral_3;
	  speed4=speed_kp*err_4+speed_ki*integral_4;
	
	  SLAVE_DCMotorMiniwatt_SpeedSet(&motor1,speed1);
		SLAVE_DCMotorMiniwatt_SpeedSet(&motor2,speed2);
		SLAVE_DCMotorMiniwatt_SpeedSet(&motor3,speed3);
		SLAVE_DCMotorMiniwatt_SpeedSet(&motor4,speed4);
	  
}
*/

void catch1(void)
{
	  SLAVE_SteeringEngine6CH_MoreMotorControl(&servo,pwm[0][0],tim[0],pwm[0][1],tim[1],pwm[1][2],tim[2],pwm[1][3],tim[3],pwm[1][4],tim[4],
			                                                  pwm[1][5],tim[5]);
	  HAL_Delay(800);
	
	  SLAVE_SteeringEngine6CH_MoreMotorControl(&servo,pwm[0][0],tim[0],pwm[0][1],tim[1],pwm[2][2],tim[2],pwm[2][3],tim[3],pwm[2][4],tim[4],
			                                                  pwm[2][5],tim[5]);
	  HAL_Delay(800);
	
	  SLAVE_SteeringEngine6CH_MoreMotorControl(&servo,pwm[0][0],tim[0],pwm[0][1],tim[1],pwm[0][2],tim[2],pwm[0][3],tim[3],pwm[0][4],tim[4],
			                                                  pwm[0][5],tim[5]);
	  HAL_Delay(800);
	
}

void put(void)
{
	  SLAVE_SteeringEngine6CH_MoreMotorControl(&servo,pwm[0][0],tim[0],pwm[0][1],tim[1],pwm[2][2],tim[2],pwm[2][3],tim[3],pwm[2][4],tim[4],
			                                                  pwm[2][5],tim[5]);
	  HAL_Delay(800);
	
	  SLAVE_SteeringEngine6CH_MoreMotorControl(&servo,pwm[0][0],tim[0],pwm[0][1],tim[1],pwm[1][2],tim[2],pwm[1][3],tim[3],pwm[1][4],tim[4],
			                                                  pwm[1][5],tim[5]);
	  HAL_Delay(800);
	
	 SLAVE_SteeringEngine6CH_MoreMotorControl(&servo,pwm[0][0],tim[0],pwm[0][1],tim[1],pwm[0][2],tim[2],pwm[0][3],tim[3],pwm[0][4],tim[4],
			                                                  1800,tim[5]);
	  HAL_Delay(800);
}

void car_spin(int direction)
{
	  if(direction==1)
		{
			  g_1_set_speed=-40;
	      g_2_set_speed=40;
	      g_3_set_speed=40;
	      g_4_set_speed=-40;
		}
		else
		{
			  g_1_set_speed=40;
	      g_2_set_speed=-40;
	      g_3_set_speed=-40;
	      g_4_set_speed=40;
		}
		HAL_Delay(1000);
	  
}

void car_move1(int16_t x,int16_t y)
{
	   int16_t speed_x,speed_y;
	   int time=0;
	   
	   if(x==0 && y>0)
		 {
			   speed_x=0;
	       speed_y=50;
			   time=y*20;
		 }
		 else if(x==0 && y<0)
		 {
			   speed_x=0;
	       speed_y=-50;        //???
			   time=-y*20;
		 }
		 else if(x>0)
		 {
			   speed_x=50;
			   speed_y=50*y/x;
			   time=x*20;
		 }
		 else
		 {
			   speed_x=-50;
			   speed_y=-50*y/x;
			   time=-x*20;
		 }
	   
	   g_1_set_speed=speed_x+speed_y;
	   g_2_set_speed=-speed_x+speed_y;
	   g_3_set_speed=speed_x+speed_y;
	   g_4_set_speed=-speed_x+speed_y;
	
	   HAL_Delay(time);
	
	   g_1_set_speed=0;
	   g_2_set_speed=0;
	   g_3_set_speed=0;
	   g_4_set_speed=0;
}

void error_calculate(void)
{
	  Tracking1=SLAVE_Tracking_BoolRead(&Tracking_Device1);
	  Tracking2=SLAVE_Tracking_BoolRead(&Tracking_Device2);
	  Tracking3=SLAVE_Tracking_BoolRead(&Tracking_Device3);
	  Tracking4=SLAVE_Tracking_BoolRead(&Tracking_Device4);
	
	  if(dir==1)
		{
				if(Tracking1==255)
				{
					  err=-2;
					  if(captrue_state) 
						{
							  count++;
							  captrue_state=0;
						}
				}
				else if(Tracking1>=96)
				{
					  err=13;
					  captrue_state=1;
				}
				else if(Tracking1<=6 && Tracking1>0)
				{
					  err=-17;
					  captrue_state=1;
				}
				else
				{
					  err=-2;
				}
				
		}/*
		else if(dir==2)
		{
			  err=Tracking2-231;
		}
		else if(dir==3)
		{
			  err=Tracking3-231;
		}
		else if(dir==4)
		{
			  err=Tracking4-231;
		}
	  */
}
/***********************************************************************************************

                                       硬件配置函数

************************************************************************************************/
/*
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
*/

/***********************************************************************************************

                                       应用任务函数

************************************************************************************************/
/***********************************************************************************************

                                       应用事件（中断）函数

************************************************************************************************/

/**
  * @brief  定时中断事件
  * @param  None
  * @retval None
  */

static void TimeBreakExecution_Handler(void) 
{
	  error_calculate();
	  if(end_state)
		{
			  motor_control(0,0,0,0);
		}
		else
		{
			  motor_control(g_1_set_speed,g_2_set_speed,g_3_set_speed,g_4_set_speed);
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
/*
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
*/
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
	
	SLAVE_DCMotorMiniwatt_Init(&motor1,2);//ID号
	SLAVE_DCMotorMiniwatt_Init(&motor2,3);//ID号
	SLAVE_DCMotorMiniwatt_Init(&motor3,4);//ID号3
	SLAVE_DCMotorMiniwatt_Init(&motor4,1);//ID号4
	SLAVE_Tracking_Init(&Tracking_Device1,2,0x00); //初始化巡线条模块,ID为1
	SLAVE_Tracking_Init(&Tracking_Device2,3,0x00); //初始化巡线条模块,ID为2
	SLAVE_Tracking_Init(&Tracking_Device3,4,0x00); //初始化巡线条模块,ID为3
	SLAVE_Tracking_Init(&Tracking_Device4,1,0x00); //初始化巡线条模块,ID为4
	SLAVE_SteeringEngine6CH_Init(&servo,3); //初始舵机模块数据,ID为3
	//SLAVE_SteppingMotor_Init(&SteppingMotor_S,STEPPINGMOTORLIST);
  ST7735_LAIBAO177_INITIAL();

	/**********************CAN初始化***************************/
	CANCommunication_Init();
	/********************定时器初始化**************************/
	Timer_SetHandler(TimeBreakExecution_Handler);//1ms
	Timer2_SetHandler(Time2BreakExecution_Handler);//1ms
}

void ApplicationProgram_main(void)
{
    ApplicationProgram_Iint();
	  SLAVE_SteeringEngine6CH_MoreMotorControl(&servo,pwm[0][0],tim[0],pwm[0][1],tim[1],pwm[0][2],tim[2],pwm[0][3],tim[3],pwm[0][4],tim[4],
			                                                  pwm[0][5],tim[5]);
	  car_move(0,25);

    while(1)
	  {	
			  LCD_ShowNum(30,30,Tracking1,4,16);
			  LCD_ShowNum(70,30,count,2,16);
			  if(count>=2) 
				{
					  catch_flag++;
					  car_move(0,0);
					  HAL_Delay(500);
					  car_move1(0,15);
					  HAL_Delay(500);
					  if(catch_flag==1)
				    {
					      catch_flag++;
					      catch1();
					      HAL_Delay(1800);
				    }
				    if(catch_flag==5)
				    {
							  car_spin(1);
					      HAL_Delay(600);
							  car_move(0,0);
					      count=0;
							  
					      catch_flag++;
					      put();
							  HAL_Delay(2000);
							  end_state=1;
				    }
						
					  car_spin(1);
					  HAL_Delay(600);
					  car_move(0,20);
					  count=0;
				}
				
			  
	  }
}

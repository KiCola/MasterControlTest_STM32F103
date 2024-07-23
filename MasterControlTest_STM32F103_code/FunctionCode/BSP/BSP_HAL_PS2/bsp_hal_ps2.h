#ifndef __BSP_HAL_PS2_H__
#define __BSP_HAL_PS2_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "mytype.h"
/* 宏定义 --------------------------------------------------------------------*/
//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
	PSS_RX = 0,
	PSS_RY,
	PSS_LX,
	PSS_LY,
}_e_RockerSelect;
typedef union
{
	uint8_t byte;
	struct
	{
		uint8_t SELECT  :1;
		uint8_t L3      :1;
		uint8_t R3      :1;
		uint8_t START   :1;
		uint8_t UP      :1;
		uint8_t RIGHT   :1;
		uint8_t DOWN    :1;
		uint8_t LEFT    :1;
	}bit;
}_u_key1;
typedef union
{
	uint8_t byte;
	struct
	{
		uint8_t L2        :1;
		uint8_t R2        :1;
		uint8_t L1        :1;
		uint8_t R1        :1;
		uint8_t TRI       :1;
		uint8_t CIRCLE    :1;
		uint8_t FORK      :1;
		uint8_t REC       :1;
	}bit;
}_u_key2;
typedef union
{
	uint8_t byte[8];
	struct
	{
		uint8_t ID;
		uint8_t verify;
		_u_key1 key1_U;
		_u_key2 key2_U;
		uint8_t PSS_RX;
		uint8_t PSS_RY;
		uint8_t PSS_LX;
		uint8_t PSS_LY;
	}DATE;
}_u_PS2_Data;

/* 扩展变量 ------------------------------------------------------------------*/
extern _u_PS2_Data PS2_Data;
/* 函数声明 ------------------------------------------------------------------*/
void PS2_SetInit(void);
void PS2_ClearData(void);
void PS2_ReadData(void);
void PS2_Vibration(uint8_t motor1, uint8_t motor2);
uint8_t PS2_AnologData(_e_RockerSelect button,uint8_t *qdata);
uint8_t PS2_DataKey(void);
uint8_t PS2_RedLight(void);

#endif  // __BSP_HAL_PS2_H__


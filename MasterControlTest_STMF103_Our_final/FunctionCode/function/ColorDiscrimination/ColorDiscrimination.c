/* 包含头文件 ----------------------------------------------------------------*/
#include "ColorDiscrimination/ColorDiscrimination.h"
#include "uartudp_config.h"
/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/

uint16_t ColorTime;
uint16_t CommandNum3501 = 0;
/* 扩展变量 ------------------------------------------------------------------*/
_s_Color Color_S;
extern _s_uartudp_nametab_tx uartudp_nametab_tx[NR_OF_HOST_NAMETABE];
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/


static float MIN(float a,float b) //求最大
{
	if(a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

static float MAX(float a,float b)//求最小
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
static void TCS3200overtime(_s_uartudp_nametab_tx *pQ)//串口通信超时
{
	if(pQ->PGState == UARTUDP_PG_TX_ERR)
	{
		switch(pQ->id[3])
		{
			case '1':
				Color_S.ComLinkflg = 1;
				ColorTime = 2000;
				break;
		}
	}
}
void TCS3200ComSucceedCallBack(struct _s_uartudp_nametab_tx *pQ)
{
	Color_S.ComLinkflg = 0;
	Color_S.rgb_s.R = Color_S.RGBamount[0];
	Color_S.rgb_s.G = Color_S.RGBamount[1];
	Color_S.rgb_s.B = Color_S.RGBamount[2];
}
void TCS3200InitialUsedPGs(void)
{
	Color_S.ComLinkflg = 0;
	CommandNum3501 = uartudp_dev.pnum_rx;
	uartudp_dev.CreateSendCommand(	UARTUDP_I,
									UARTUDP_ARRAY,
									UARTUDP_U16,
									6,
									"3501",
									(void*)Color_S.RGBamount,
									"RGBamount",
									UARTUDP_CYCLIC,
									UARTUDP_PG_TX_REQ,
									100,
									0,
									TCS3200ComSucceedCallBack,
									TCS3200overtime);
}
void TCS3200TimeScan(void)
{
	if(ColorTime > 0) //说明通讯断开
	{
		ColorTime--; //时间开始计时
		if(ColorTime == 0) 
		{//时间到重新向从机所取命令
			uartudp_nametab_tx[CommandNum3501].PGState = UARTUDP_PG_TX_REQ;
		}
	}
}
void RGB_TO_HSV(const COLOR_RGB* input,COLOR_HSV* output) //RGB转HSV
{
	float r,g,b,minRGB,maxRGB,deltaRGB;

	r = input->R/255.0f;
	g = input->G/255.0f;
	b = input->B/255.0f;
    minRGB = MIN(r,MIN(g,b));
    maxRGB = MAX(r,MAX(g,b));
	deltaRGB = maxRGB - minRGB;

	//V计算
	output->V = maxRGB*100.0f;
	//S计算
	if(maxRGB != 0.0f)
		output->S = deltaRGB / maxRGB;
	else
		output->S = 0.0f;
	output->S *= 100.0f;
	//H计算
	if(deltaRGB == 0.0f)
	{
		output->H = 0.0f;
	}
	else if(maxRGB == r)
	{
		output->H = (g-b)/deltaRGB;
	}
	else if(maxRGB == g)
	{
		output->H = 2.0f + ((b-r)/deltaRGB);
	}
	else if(maxRGB == b)
	{
		output->H = 4.0f + (r-g)/deltaRGB;
	}
	output->H = output->H * 60.0f;
	if (output->H < 0.0f)
	{
		output->H += 360.f;
	}
}

uint16_t ColorIdentification_HSV(_s_Color *p,
                                 uint16_t cs) //使用HSV值识别颜色
{
	uint8_t i;
	
	p->rgb_s.R = p->RGBamount[0];
	p->rgb_s.G = p->RGBamount[1];
	p->rgb_s.B = p->RGBamount[2];
	
	if(cs == 0) cs = 1;
	
	for(i=0; i<cs; i++)  //一阶滤波
	{
		if(cs != 1)
		{
			HAL_Delay(9);
		}
		p->rgb_s.R = ((p->rgb_s.R*5)+(p->RGBamount[0]*5))/10;
		p->rgb_s.G = ((p->rgb_s.G*5)+(p->RGBamount[1]*5))/10;
		p->rgb_s.B = ((p->rgb_s.B*5)+(p->RGBamount[2]*5))/10;
	}
	
	RGB_TO_HSV(&p->rgb_s,&p->hsv_s);
	if((p->hsv_s.S < 25.0f)&&(p->hsv_s.V < 15.0f))
	{
		return BLACK;
	}
	if(p->hsv_s.H >= 300.0f)
	{
		return RED;
	}
	else if(p->hsv_s.H >= 200.0f)
	{
		return BLUE;
	}
	else if(p->hsv_s.H >= 100.0f)
	{
		return GREEN;
	}
	else if(p->hsv_s.H <= 5.0f)
	{
		return RED;
	}
	else 
	{
		return WHITE;
	}
}

uint16_t ColorIdentification_RGB(_s_Color *p,
                                 uint16_t cs) //使用RGB值识别颜色
{
	uint16_t i;
	uint16_t ColorMAX;
	uint16_t ColorMIN;
	uint16_t ColorDiff;
	uint16_t color;
	
	p->rgb_s.R = p->RGBamount[0];
	p->rgb_s.G = p->RGBamount[1];
	p->rgb_s.B = p->RGBamount[2];
	
	if(cs == 0) cs = 1;
	
	for(i=0; i<cs; i++)  //一阶滤波
	{
		if(cs != 1)
		{
			HAL_Delay(9);
		}
		p->rgb_s.R = ((p->rgb_s.R*5)+(p->RGBamount[0]*5))/10;
		p->rgb_s.G = ((p->rgb_s.G*5)+(p->RGBamount[1]*5))/10;
		p->rgb_s.B = ((p->rgb_s.B*5)+(p->RGBamount[2]*5))/10;
	}
	
	if(p->rgb_s.R > p->rgb_s.G)
	{
		if(p->rgb_s.R > p->rgb_s.B)
		{
			ColorMAX = p->rgb_s.R;
			if(p->rgb_s.B > p->rgb_s.G)
			{
				ColorMIN = p->rgb_s.B;
			}
			else
			{
				ColorMIN = p->rgb_s.G;
			}
		}
		else
		{
			ColorMAX = p->rgb_s.B;
			ColorMIN = p->rgb_s.R;
		}
	}
	else 
	{
		if(p->rgb_s.G > p->rgb_s.B)
		{
			ColorMAX = p->rgb_s.G;
			if(p->rgb_s.B > p->rgb_s.R)
			{
				ColorMIN = p->rgb_s.B;
			}
			else
			{
				ColorMIN = p->rgb_s.R;
			}
		}
		else
		{
			ColorMAX = p->rgb_s.B;
			ColorMIN = p->rgb_s.G;
		}
	}
	ColorDiff = ColorMAX-ColorMIN;
	if(ColorDiff < 5)
	{
		color = BLACK;
	}
	else if(ColorDiff > 40)
	{
		if(ColorMAX == p->rgb_s.R)
		{
			color = RED;
		}
		else if(ColorMAX == p->rgb_s.G)
		{
			color = GREEN; 
		}
		else if(ColorMAX == p->rgb_s.B)
		{
			color = BLUE;
		}
	}
	else 
	{
		color = WHITE;
	}
	
	return color;
}


//uint16_t ColorIdentification1_RGB(uint16_t *RGBamount_p,
//                                 uint16_t cs) //使用RGB值识别颜色
//{
//	uint16_t color;
//	uint16_t i;
//	uint16_t RGBBUF[3];
//	uint16_t ColorMAX;
//	uint16_t MIN;
//	#define CoefficientColor 15
//	
//	color = WHITE;
//	RGBBUF[0] = RGBamount_p[0];
//	RGBBUF[1] = RGBamount_p[1];
//	RGBBUF[2] = RGBamount_p[2];
//	if(cs == 0) cs = 1;
//	
//	for(i=0; i<cs; i++)  //一阶滤波
//	{
//		if(cs != 1)
//		{
//			HAL_Delay(9);
//		}
//		RGBBUF[0] = ((RGBBUF[0]*5)+(RGBamount_p[0]*5))/10;
//		RGBBUF[1] = ((RGBBUF[1]*5)+(RGBamount_p[1]*5))/10;
//		RGBBUF[2] = ((RGBBUF[2]*5)+(RGBamount_p[2]*5))/10;
//	}
//	
//	ColorMAX = RGBBUF[0]+RGBBUF[1]+RGBBUF[2];   //取RGB总值
//	RGBBUF[0] = RGBBUF[0]*100/ColorMAX+CoefficientColor;  //提取R占总值的百分比，偏移CoefficientColor
//	RGBBUF[1] = RGBBUF[1]*100/ColorMAX+CoefficientColor;  //提取G占总值的百分比，偏移CoefficientColor
//	RGBBUF[2] = RGBBUF[2]*100/ColorMAX+CoefficientColor;  //提取B占总值的百分比，偏移CoefficientColor
//	
//	/**********************提取最大的百分比*****************************/
//	ColorMAX = RGBBUF[0];
//	MIN = RGBBUF[1];
//	if(ColorMAX < MIN)
//	{
//		ColorMAX = RGBBUF[1];
//		MIN = RGBBUF[0];
//	}
//	if(ColorMAX < RGBBUF[2])
//	{
//		ColorMAX = RGBBUF[2];
//	}
//	else if(MIN > RGBBUF[2])
//	{
//		MIN = RGBBUF[2];
//	}
//	if((ColorMAX-CoefficientColor) > MIN)
//	{
//		if(ColorMAX == RGBBUF[0])
//		{
//			color = RED;
//		}
//		else if(ColorMAX == RGBBUF[1])
//		{
//			color = GREEN; 
//		}
//		else if(ColorMAX == RGBBUF[2])
//		{
//			color = BLUE;
//		}
//	}
//	
//	return color;
//}



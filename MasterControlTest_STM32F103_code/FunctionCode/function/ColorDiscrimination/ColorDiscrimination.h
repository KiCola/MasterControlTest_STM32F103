#ifndef __ColorDiscrimination_H
#define __ColorDiscrimination_H

#include "data.h"






typedef struct{
 unsigned char R;
 unsigned char G;
 unsigned char B;
}COLOR_RGB;
typedef struct{
 float H;
 float S;
 float V;
}COLOR_HSV;


typedef struct
{
	uint8_t  ComLinkflg;//0为通讯中，1为断开
	uint16_t RGBamount[3];
	COLOR_RGB rgb_s;
	COLOR_HSV hsv_s;
}_s_Color;


extern _s_Color Color_S;



void TCS3200TimeScan(void);
void TCS3200InitialUsedPGs(void);
void RGB_TO_HSV(const COLOR_RGB* input,COLOR_HSV* output);
uint16_t ColorIdentification_HSV(_s_Color *p,
                                 uint16_t cs); //使用HSV值识别颜色
uint16_t ColorIdentification_RGB(_s_Color *p,
                                 uint16_t cs); //使用RGB值识别颜色



#endif

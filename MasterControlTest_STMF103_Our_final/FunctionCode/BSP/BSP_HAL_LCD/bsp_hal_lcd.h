#ifndef __BSP_LCD_H__
#define	__BSP_LCD_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "mytype.h"
#include "spi.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
//LCD��Ҫ������
typedef struct  
{										    
	u8 width;			//LCD ���
	u8 height;			//LCD �߶�
	u8  dir;			//���������������ƣ�0��������1��������	
}_lcd_dev; 	

/* �궨�� --------------------------------------------------------------------*/
#define LCD_SPI         hspi2
#define LCD_CS_HIGH()                      HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, GPIO_PIN_SET)
#define LCD_CS_LOW()                       HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_A0_HIGH()                      HAL_GPIO_WritePin(LCD_A0_GPIO_Port,LCD_A0_Pin, GPIO_PIN_SET)
#define LCD_A0_LOW()                       HAL_GPIO_WritePin(LCD_A0_GPIO_Port,LCD_A0_Pin, GPIO_PIN_RESET)
#define LCD_RESET_HIGH()                   HAL_GPIO_WritePin(LCD_RESET_GPIO_Port,LCD_RESET_Pin, GPIO_PIN_SET)
#define LCD_RESET_LOW()                    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port,LCD_RESET_Pin, GPIO_PIN_RESET)

#define LCD_DELAY(x)  HAL_Delay(x)   

#define Height 132
#define Width  132

#define RED    0xf800   //��5��6��5
#define GREEN  0x07e0
#define BLUE   0x001f
#define YELLOW 0xffe0
#define WHITE  0xffff
#define BLACK  0x0000
#define PURPLE 0xf81f
#define GOOD 0x073f //0x04D6

#define GRAY0   0xEF7D   	//��ɫ0 3165 00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1      00000 000000 00000
#define GRAY2   0x4208      	//��ɫ2  1111111111011111

/* ��չ���� ------------------------------------------------------------------*/
//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

/* �������� ------------------------------------------------------------------*/
void LCD_WriteCommand(u8 c);
void LCD_WriteData(u8 dat);
void LCD_Write_Data(u16 dat16);
void DISPLAY_COLOR(u16 color);
void DISP_WINDOWS(void);
void DISPLAY_image(u8 x,u8 y,u8 width,u8 height,u8 picnum);
void ST7735_LAIBAO177_INITIAL(void);//��ʼ��
void Frame(void);
void DISPLAY_RGB(void);
void LCD_Display_Dir(u8 dir);//���ú�����
void LCD_Clear(u16 color);//����
void LCD_DrawPoint(u8 x,u8 y);//����
void LCD_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2);//����
//void LCD_ShowChar(u8 x,u8 y,u8 num,u8 size,u8 mode);//��ʾ�ַ�
//void LCD_ShowString(u8 x,u8 y,u8 width,u8 height,u8 size,u8 *p);
u32 LCD_Pow(u8 m,u8 n);////m^n����������ֵ:m^n�η�.
void LCD_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//��ʾ����
void LCD_ShowxNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 mode,u16 fc, u16 bc);//��ʾ�������
void LCD_ShowxPMNum(u8 x,u8 y,s32 num,u8 len,u8 size,u8 mode,u16 fc, u16 bc);
void baocunjm1(void);
void LCD_ShowChar_BufMode(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode);
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);

void LCD_DrawRectage(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_Fill(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend ,uint16_t color);
void LCD_DrawCircle(uint8_t x0, uint16_t y0, uint8_t r);

#endif /* __BSP_LCD_H__ */


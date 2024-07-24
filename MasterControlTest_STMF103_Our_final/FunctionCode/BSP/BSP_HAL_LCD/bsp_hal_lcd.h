#ifndef __BSP_LCD_H__
#define	__BSP_LCD_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "mytype.h"
#include "spi.h"

/* 类型定义 ------------------------------------------------------------------*/
//LCD重要参数集
typedef struct  
{										    
	u8 width;			//LCD 宽度
	u8 height;			//LCD 高度
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
}_lcd_dev; 	

/* 宏定义 --------------------------------------------------------------------*/
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

#define RED    0xf800   //红5绿6蓝5
#define GREEN  0x07e0
#define BLUE   0x001f
#define YELLOW 0xffe0
#define WHITE  0xffff
#define BLACK  0x0000
#define PURPLE 0xf81f
#define GOOD 0x073f //0x04D6

#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111

/* 扩展变量 ------------------------------------------------------------------*/
//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

/* 函数声明 ------------------------------------------------------------------*/
void LCD_WriteCommand(u8 c);
void LCD_WriteData(u8 dat);
void LCD_Write_Data(u16 dat16);
void DISPLAY_COLOR(u16 color);
void DISP_WINDOWS(void);
void DISPLAY_image(u8 x,u8 y,u8 width,u8 height,u8 picnum);
void ST7735_LAIBAO177_INITIAL(void);//初始化
void Frame(void);
void DISPLAY_RGB(void);
void LCD_Display_Dir(u8 dir);//设置横竖屏
void LCD_Clear(u16 color);//清屏
void LCD_DrawPoint(u8 x,u8 y);//画点
void LCD_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2);//画线
//void LCD_ShowChar(u8 x,u8 y,u8 num,u8 size,u8 mode);//显示字符
//void LCD_ShowString(u8 x,u8 y,u8 width,u8 height,u8 size,u8 *p);
u32 LCD_Pow(u8 m,u8 n);////m^n函数，返回值:m^n次方.
void LCD_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//显示数字
void LCD_ShowxNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 mode,u16 fc, u16 bc);//显示多个数字
void LCD_ShowxPMNum(u8 x,u8 y,s32 num,u8 len,u8 size,u8 mode,u16 fc, u16 bc);
void baocunjm1(void);
void LCD_ShowChar_BufMode(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode);
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);

void LCD_DrawRectage(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_Fill(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend ,uint16_t color);
void LCD_DrawCircle(uint8_t x0, uint16_t y0, uint8_t r);

#endif /* __BSP_LCD_H__ */


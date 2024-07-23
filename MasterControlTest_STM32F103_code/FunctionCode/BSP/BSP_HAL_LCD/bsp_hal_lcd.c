/* 包含头文件 ----------------------------------------------------------------*/
#include "BSP_HAL_LCD/bsp_hal_lcd.h"
#include "BSP_HAL_LCD/font.h"
#include "string.h"	
/* 私有类型定义 --------------------------------------------------------------*/
uint16_t DotSite = 0;
uint16_t lcd_buf1[16*16]={0};
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
_lcd_dev lcddev;	//管理LCD重要参数
u16 POINT_COLOR=BLACK;	//画笔颜色
u16 BACK_COLOR=WHITE;  //背景色 
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 LCD_SPI_ReadWriteByte(u8 TxData)
{		
	
    HAL_SPI_Transmit(&LCD_SPI, &TxData, 1, 3);
    return 0 ; 
}
static void LCD_SPI_ReadWriteBuf(u8 *TxData,u16 len)
{		
    LCD_A0_HIGH();
    LCD_CS_LOW();
    HAL_SPI_Transmit(&LCD_SPI, TxData, len*2,1000);
    LCD_CS_HIGH();
    DotSite = 0;
}
static void LCD_WR_DATA(uint16_t data,uint16_t site)
{
    ((u8*)(lcd_buf1+site))[0] = data>>8;
    ((u8*)(lcd_buf1+site))[1] = data&0xff;
    DotSite++;
}
//========================================================
//向液晶屏写一个8位指令
void  LCD_WriteCommand(u8 c)
{
     LCD_A0_LOW();
     LCD_CS_LOW();
     LCD_SPI_ReadWriteByte(c);
     LCD_CS_HIGH();	
}
//========================================================
//向液晶屏写一个8位数据
void LCD_WriteData(u8 dat)
{
      LCD_A0_HIGH();
      LCD_CS_LOW();
      LCD_SPI_ReadWriteByte(dat);
      LCD_CS_HIGH();
}
//========================================================
//向液晶屏写一个16位数据
void LCD_Write_Data(u16 dat16)
{
  LCD_WriteData(dat16>>8);
  LCD_WriteData(dat16);
}

//========================================================
void DISPLAY_COLOR(u16 color)
{
	int i,j;
 	DISP_WINDOWS();
	for (i=Height;i>0;i--)
	for (j=Width; j>0;j--)
	LCD_Write_Data(color);
}
//========================================================
//void DISPLAY_RGB(void)
//{
//    int i,j,k;
//	for (i=53;i>0;i--)
//	for (j=Width;j>0;j--)
//	{
//        LCD_Write_Data(RED);
//	}
//    for (i=53;i>0;i--)
//	for (j=Width;j>0;j--)
//	{
//        LCD_Write_Data(GREEN);
//	}
//    for (k=54;k>0;k--)
//	for (j=Width;j>0;j--)
//	{
//        LCD_Write_Data(BLUE);
//	}
//}
void DISPLAY_RGB(void)
{
    int i,j,k;
	DISP_WINDOWS();
	for (i=Height/3;i>0;i--)
	for (j=Width;j>0;j--)
	{
		LCD_Write_Data(RED);
		//delay_us(30);
	}
    for (i=Height/3;i>0;i--)
	for (j=Width;j>0;j--)
	{
		LCD_Write_Data(GREEN);
		//delay_us(30);
	}
    for (k=Height/3;k>0;k--)
	for (j=Width;j>0;j--)
	{
		LCD_Write_Data(BLUE);
		//delay_us(30);
	}
}
//========================================================
void DISP_WINDOWS(void)
{
         LCD_WriteCommand(0x2A);
         LCD_WriteData(0x00);
         LCD_WriteData(0x00);
         LCD_WriteData(0x00);
         LCD_WriteData(0xaf);

         LCD_WriteCommand(0x2B);
         LCD_WriteData(0x00);
         LCD_WriteData(0x00);
         LCD_WriteData(0x00);
         LCD_WriteData(0x8F);
         LCD_WriteCommand(0x2C);
}

//========================================================
//设置LCD显示方向（6804不支持横屏显示）
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)//竖屏
	{
		lcddev.dir=0;//竖屏
		lcddev.width=Width;
		lcddev.height=Height;
		
	}else 
	{	  
		lcddev.dir=1;//横屏
		lcddev.width=Height;
		lcddev.height=Width;
		
	} 
	//LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}	 

//========================================================

void  ST7735_LAIBAO177_INITIAL (void)
{
	// VCI=2.8V
	//************* RES LCD Driver ****************//
	LCD_RESET_HIGH();
    LCD_DELAY(1);// Delay 1ms
	LCD_RESET_LOW();
    LCD_DELAY(10);// Delay 10ms // This delay time is necessary
	LCD_RESET_HIGH();
    LCD_DELAY(20);
	LCD_WriteCommand(0x11); //Sleep out 
    LCD_DELAY(50);//Delay 120ms             
	//------------------------------------ST7735S Frame Rate-----------------------------------------// 
	LCD_WriteCommand(0xB1); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteCommand(0xB2); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteCommand(0xB3); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	//------------------------------------End ST7735S Frame Rate---------------------------------// 
	LCD_WriteCommand(0xB4); //Dot inversion 
	LCD_WriteData(0x03); 
	//------------------------------------ST7735S Power Sequence---------------------------------// 
	LCD_WriteCommand(0xC0); 
	LCD_WriteData(0x28); 
	LCD_WriteData(0x08); 
	LCD_WriteData(0x04); 
	LCD_WriteCommand(0xC1); 
	LCD_WriteData(0XC0); 
	LCD_WriteCommand(0xC2); 
	LCD_WriteData(0x0D); 
	LCD_WriteData(0x00); 
	LCD_WriteCommand(0xC3); 
	LCD_WriteData(0x8D); 
	LCD_WriteData(0x2A); 
	LCD_WriteCommand(0xC4); 
	LCD_WriteData(0x8D); 
	LCD_WriteData(0xEE); 
	//---------------------------------End ST7735S Power Sequence-------------------------------------// 
	LCD_WriteCommand(0xC5); //VCOM 
	LCD_WriteData(0x1A); 
	LCD_WriteCommand(0x36); //MX, MY, RGB mode 
	LCD_WriteData(0x79); //显示屏幕方向设置 和RPG颜色设置
	//------------------------------------ST7735S Gamma Sequence---------------------------------// 
	LCD_WriteCommand(0xE0); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x22); 
	LCD_WriteData(0x07); 
	LCD_WriteData(0x0A); 
	LCD_WriteData(0x2E); 
	LCD_WriteData(0x30); 
	LCD_WriteData(0x25); 
	LCD_WriteData(0x2A); 
	LCD_WriteData(0x28); 
	LCD_WriteData(0x26); 
	LCD_WriteData(0x2E); 
	LCD_WriteData(0x3A); 
	LCD_WriteData(0x00); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x03); 
	LCD_WriteData(0x13); 
	LCD_WriteCommand(0xE1); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x16); 
	LCD_WriteData(0x06); 
	LCD_WriteData(0x0D); 
	LCD_WriteData(0x2D); 
	LCD_WriteData(0x26); 
	LCD_WriteData(0x23); 
	LCD_WriteData(0x27); 
	LCD_WriteData(0x27); 
	LCD_WriteData(0x25); 
	LCD_WriteData(0x2D); 
	LCD_WriteData(0x3B); 
	LCD_WriteData(0x00); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x13); 
	//------------------------------------End ST7735S Gamma Sequence-----------------------------// 
	LCD_WriteCommand(0x3A); //65k mode 
	LCD_WriteData(0x05); 
		//===================================
	LCD_WriteCommand(0x29); //Display on 
	LCD_Display_Dir(0);	
}
//清屏函数
//color:要清屏的填充色
//void LCD_Clear(u16 color)
//{
//	u8 i,j;
//    DISP_WINDOWS();
//	for (i=Height;i>0;i--)
//	for (j=Width;j>0;j--)
//	{
//        LCD_Write_Data(color);
//	}
//}  

void LCD_Clear(u16 color)
{
	u16 i,j;
    uint16_t lcd_buf[2*Width]={0};
    DISP_WINDOWS();
    for (j=0;j<66;j++)
    {
        for (i=0;i<(2*Width);i++)
        {
           // LCD_WR_DATA(color,i);
            ((u8*)(lcd_buf+i))[0] = color>>8;
            ((u8*)(lcd_buf+i))[1] = color&0xff;
        } 
        LCD_SPI_ReadWriteBuf((u8*)lcd_buf,i);
    }
}
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u8 x,u8 y)
{
	
 	LCD_WriteCommand(0x2A);
     LCD_WriteData(0x00);
     LCD_WriteData(x);
     LCD_WriteData(0x00);
     LCD_WriteData(x);

     LCD_WriteCommand(0x2B);
     LCD_WriteData(0x00);
     LCD_WriteData(y);
     LCD_WriteData(0x00);
     LCD_WriteData(y);
     LCD_WriteCommand(0x2C);

	LCD_Write_Data(POINT_COLOR);
	
}
void LCD_SetWindows(u8 x,u8 y,u8 xx,u8 yy)
{
	
 	LCD_WriteCommand(0x2A);
         LCD_WriteData(0x00);
         LCD_WriteData(x);
         LCD_WriteData(0x00);
         LCD_WriteData(xx);

         LCD_WriteCommand(0x2B);
         LCD_WriteData(0x00);
         LCD_WriteData(y);
         LCD_WriteData(0x00);
         LCD_WriteData(yy);
         LCD_WriteCommand(0x2C);

//	LCD_Write_Data(POINT_COLOR);
	
}
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol;  
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   


//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
//void LCD_ShowChar(u8 x,u8 y,u8 num,u8 size,u8 mode)
//{  							  
//    u8 temp,t1,t;
//	u8 y0=y;
//	u16 colortemp=POINT_COLOR;   
//    u8 x_buf,y_buf;	 
//    x_buf = x;
//    y_buf = y;      
//	//设置窗口		   
//	num=num-' ';//得到偏移后的值
//	if(!mode) //非叠加方式
//	{
//	    for(t=0;t<size;t++)
//	    {   
//			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
//			else temp=asc2_1608[num][t];		 //调用1608字体 
//			for(t1=0;t1<8;t1++)
//			{			    
//		        if(temp&0x80)POINT_COLOR=colortemp;
//				else POINT_COLOR=BACK_COLOR;
//                
//                LCD_WR_DATA(POINT_COLOR,DotSite);
//				//LCD_DrawPoint(x,y);	
//				temp<<=1;
//				y++;
//				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
//					break;
//				}
//			}  	 
//	    }    
//	}else//叠加方式
//	{
//	    for(t=0;t<size;t++)
//	    {   
//			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
//			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
//	        for(t1=0;t1<8;t1++)
//			{			    
//		        if(temp&0x80)
//                {
//                    LCD_WR_DATA(POINT_COLOR,DotSite);
//                    //LCD_DrawPoint(x,y); 
//                }   
//				temp<<=1;
//				y++;
//				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
//					break;
//				}
//			}  	 
//	    }     
//	}
//	POINT_COLOR=colortemp;	

//    LCD_WriteCommand(0x2A);
//    LCD_WriteData(0x00);
//    LCD_WriteData(x_buf);
//    LCD_WriteData(0x00);
//    LCD_WriteData(x_buf+size/2-1);
//    
//    LCD_WriteCommand(0x2B);
//    LCD_WriteData(0x00);
//    LCD_WriteData(y_buf);
//    LCD_WriteData(0x00);
//    LCD_WriteData(y_buf+size-1);
//    
//    LCD_WriteCommand(0x2C);//
//    
//    LCD_SPI_ReadWriteBuf((u8*)lcd_buf1,DotSite);     
//} 
//============================================

//============================================================
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar_BufMode(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
		LCD_ShowChar_BufMode(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',size,0);
  }
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 mode,u16 fc, u16 bc)
{  
	u8 t,temp;
	u8 enshow=0;
    POINT_COLOR=fc;	//画笔颜色
    BACK_COLOR=bc;  //背景色  
    if(size != 12)
	{
		size = 16;
	}		
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)
				{
					LCD_ShowChar_BufMode(x+(size/2)*t,y,fc,bc,'0',size,mode&0X01);
				} 
				else
				{	
                    LCD_ShowChar_BufMode(x+(size/2)*t,y,fc,bc,' ',size,mode&0X01);					
				} 
 				continue;
			}else enshow=1; 
		 	 
		}
		LCD_ShowChar_BufMode(x+(size/2)*t,y,fc,bc,temp+'0',size,mode&0X01);
	}
    POINT_COLOR=BLACK;	//画笔颜色
    BACK_COLOR=WHITE;  //背景色 
} 




void LCD_ShowxPMNum(u8 x,u8 y,s32 num,u8 len,u8 size,u8 mode,u16 fc, u16 bc)
{  
	u8 t,temp;
	u8 enshow=0;
	u8 flg = 0;
    POINT_COLOR=fc;	//画笔颜色
    BACK_COLOR=bc;  //背景色  
    if(size != 12)
	{
		size = 16;
	}	
    if(num >= 0)
	{
		flg = 1;
	}	
    else
	{
		num = -num;
	}		
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)
				{
					if((t==(len-1))&&(flg == 0))
					{
						flg = 1;
						LCD_ShowChar_BufMode(x+(size/2)*t,y,fc,bc,'-',size,mode&0X01);	
					}
					else
					{
						LCD_ShowChar_BufMode(x+(size/2)*t,y,fc,bc,'0',size,mode&0X01);
					}
				} 
				else
				{	
					if(flg == 0)
					{
						flg = 1;
						LCD_ShowChar_BufMode(x+(size/2)*t,y,fc,bc,'-',size,mode&0X01);	
					}
					else
					{
						LCD_ShowChar_BufMode(x+(size/2)*t,y,fc,bc,' ',size,mode&0X01);	
					}						
				} 
 				continue;
			}else enshow=1; 
		 	 
		}
		LCD_ShowChar_BufMode(x+(size/2)*t,y,fc,bc,temp+'0',size,mode&0X01);
	}
    POINT_COLOR=BLACK;	//画笔颜色
    BACK_COLOR=WHITE;  //背景色 
} 




//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
//void LCD_ShowString(u8 x,u8 y,u8 width,u8 height,u8 size,u8 *p)
//{         
//	u8 x0=x;
//	width+=x;
//	height+=y;
//    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
//    {       
//        if(x>=width){x=x0;y+=size;}
//        if(y>=height)break;//退出
//		
//        LCD_ShowChar(x,y,*p,size,0);
//        x+=size/2;
//        p++;
//    }  
//}	 		   
void DISP_WINDOWS1(u8 x)
{
         LCD_WriteCommand(0x2A);
         LCD_WriteData(0x00);
         LCD_WriteData(x);
         LCD_WriteData(0x00);
         LCD_WriteData(x+1);

         LCD_WriteCommand(0x2B);
         LCD_WriteData(0x00);
         LCD_WriteData(0x00);
         LCD_WriteData(0x00);
         LCD_WriteData(0x7F);
         LCD_WriteCommand(0x2C);
}
//======================================================
//******************************************************************
//函数名：  GUI_DrawFont16
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    显示单个16X16中文字体
//输入参数：x,y :起点坐标
//			fc:前置画笔颜色
//			bc:背景颜色	 
//			s:字符串地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 x0=x;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//自动统计汉字数目
	
			
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	
		  LCD_SetWindows(x,y,x+16-1,y+16-1);
		    for(i=0;i<16*2;i++)
		    {
				for(j=0;j<8;j++)
		    	{	
					if(!mode) //非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x80>>j))	
							LCD_Write_Data(fc);
						else
							LCD_Write_Data(bc);
					}
					else
					{
						POINT_COLOR=fc;
						if(tfont16[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//画一个点
						x++;
						if((x-x0)==16)
						{
							x=x0;
							y++;
							break;
						}
					}

				}
				
			}
			
			
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
	POINT_COLOR=BLACK;	//画笔颜色
	BACK_COLOR=WHITE;  //背景色 
	//LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar1(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
{  							  
    u8 temp;
    u8 pos,t1;
//	u8 y0=y;
    u8 x0=x;
	u16 colortemp=fc;//POINT_COLOR;      
		   
	num=num-' ';//得到偏移后的值
	LCD_SetWindows(x,y,x+size/2-1,y+size-1);//设置单个文字显示窗口
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp<<=1;
                x++;
                if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
                if((x-x0)==8)
				{
					x=x0;
					y++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
//				y++;
//				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
//					break;
//				}
			}  
			
		}	
	}else//叠加方式
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y); 
				temp<<=1;
                x++;
                if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
                if((x-x0)==8)
				{
					x=x0;
					y++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
//				y++;
//				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
//					break;
//				}
			}  
		}
	}
	POINT_COLOR=colortemp;	
	POINT_COLOR=BLACK;	//画笔颜色
	BACK_COLOR=WHITE;  //背景色   	   	 	  
}   
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString1(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{         
	//u16 y0=y;
	u16 x0=x;							  	  
  	u8 bHz=0;     //字符或者中文 
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
			if(x>(lcddev.width-size/2)||y>(lcddev.height-size)) 
			return; 
	        if(*str>0x80)
						bHz=1;//中文 
	        else              //字符
	        {          
		        if(*str==0x0D)//换行符号
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else
				{
					if(size>16)//字库中没有集成12X24 16X32的英文字体,用8X16代替
					{  
					LCD_ShowChar1(x,y,fc,bc,*str,16,mode);
					x+=8; //字符,为全字的一半 
					}
					else
					{
					LCD_ShowChar1(x,y,fc,bc,*str,size,mode);
					x+=size/2; //字符,为全字的一半 
					}
				} 
				str++; 
		        
	        }
        }else//中文 
        {   
			if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
			return;  
            bHz=0;//有汉字库    
			GUI_DrawFont16(x,y,fc,bc,str,mode);
				
	        str+=2; 
	        x+=size;//下一个汉字偏移	    
        }						 
    }	
}
void GUI_DrawFont16_BufMode(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 x0=x;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//自动统计汉字数目		
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	
		    for(i=0;i<16*2;i++)
		    {
				for(j=0;j<8;j++)
		    	{	
					if(!mode) //非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x80>>j))	
                            LCD_WR_DATA(fc,DotSite);
						else
                            LCD_WR_DATA(bc,DotSite);
					}
					else
					{
						POINT_COLOR=fc;
						if(tfont16[k].Msk[i]&(0x80>>j))	
                            LCD_WR_DATA(POINT_COLOR,DotSite);
						x++;
						if((x-x0)==16)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
            LCD_SetWindows(x,y,x+16-1,y+16-1);
            LCD_SPI_ReadWriteBuf((u8*)lcd_buf1,DotSite);  
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
	POINT_COLOR=BLACK;	//画笔颜色
	BACK_COLOR=WHITE;  //背景色 
}

void LCD_ShowChar_BufMode(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
{  							  
    u8 temp;
    u8 pos,t1;
	u16 x0=x;
	u16 colortemp=fc;//POINT_COLOR;      
	u16 x_buf,y_buf;	 
    x_buf = x;
    y_buf = y;  
	num=num-' ';//得到偏移后的值
	
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			x=x0;
			for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)
                {
                    POINT_COLOR=fc;
                }
				else
                {
//                    if(num == 0)
//                    {
//                        POINT_COLOR=WHITE;
//                    }
//                    else
//                    {
                        POINT_COLOR=bc;
//                    }
                    
                }
                LCD_WR_DATA(POINT_COLOR,DotSite);
				//LCD_DrawPoint(x,y);	
				temp<<=1;
				x++;
				if((x-x0)==size)
				{
					x=x0;
					y++;
					if(y>=lcddev.width)
                    {
                        POINT_COLOR=colortemp;return;
                    }//超区域了
					break;
				}
				if(y>=lcddev.width)
                {
                    POINT_COLOR=colortemp;return;
                }//超区域了
			}  
			
		}	
	}else//叠加方式
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			x=x0;
			for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)
                {
                   // LCD_DrawPoint(x,y); 
                    LCD_WR_DATA(POINT_COLOR,DotSite);
                }  
				temp<<=1;
				x++;
				if((x-x0)==size)
				{
					x=x0;
					y++;
					if(y>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
				if(y>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
			}  
		}
	}
	POINT_COLOR=colortemp;	
	POINT_COLOR=BLACK;	//画笔颜色
	BACK_COLOR=WHITE;  //背景色    
    
    if(size==12)
	{
		LCD_WriteCommand(0x2A);
		LCD_WriteData(0x00);
		LCD_WriteData(x_buf);
		LCD_WriteData(0x00);
		LCD_WriteData(x_buf+7);
		
		LCD_WriteCommand(0x2B);
		LCD_WriteData(0x00);
		LCD_WriteData(y_buf);
		LCD_WriteData(0x00);
		LCD_WriteData(y_buf+12-1);
	}
	else
	{
		LCD_WriteCommand(0x2A);
		LCD_WriteData(0x00);
		LCD_WriteData(x_buf);
		LCD_WriteData(0x00);
		LCD_WriteData(x_buf+size/2-1);
		
		LCD_WriteCommand(0x2B);
		LCD_WriteData(0x00);
		LCD_WriteData(y_buf);
		LCD_WriteData(0x00);
		LCD_WriteData(y_buf+size-1);
	}
    
    LCD_WriteCommand(0x2C);//
    
    LCD_SPI_ReadWriteBuf((u8*)lcd_buf1,DotSite);  
}
void LCD_ShowString_BufMode(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{         
	u16 x0=x;							  	  
  	u8 bHz=0;     //字符或者中文 
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
			if(x>(lcddev.width-size/2)) 
			{
				y+=size;
				x=x0;
			}
			if(y>(lcddev.height-size))
			{
				return;
			}
	        if(*str>0x80){
				bHz=1;//中文 
			}
	        else              //字符
	        {          
		        if(*str==0x0D)//换行符号
		        {         
		            y+=size;
					x=x0;
		        }  
		        else
				{
					LCD_ShowChar_BufMode(x,y,fc,bc,*str,size,mode);
					x+=size/2; //字符,为全字的一半 
				} 
				str++; 
	        }
        }else//中文 
        {   
			if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
			return;  
            bHz=0;//有汉字库    
			GUI_DrawFont16_BufMode(x,y,fc,bc,str,mode);	
	        str+=2; 
	        x+=16;//size;//下一个汉字偏移	    
        }						 
    }	
}
//******************************************************************
//函数名：  Gui_StrCenter
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    居中显示一个字符串,包含中英文显示
//输入参数：x,y :起点坐标
// 			fc:前置画笔颜色
//			bc:背景颜色
//			str :字符串	 
//			size:字体大小
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************   
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{
    LCD_ShowString_BufMode(x,y,fc,bc,str,size,mode);
//	LCD_ShowString1(x/*+x1*/,y,fc,bc,str,size,mode);
} 


/**********************************************/
/* 函数功能；画矩形                           */
/**********************************************/
void LCD_DrawRectage(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend,uint16_t color)
{
	POINT_COLOR=color;
	LCD_DrawLine(xstart, ystart, xend, ystart);
	LCD_DrawLine(xstart, yend, xend, yend);
	LCD_DrawLine(xstart, ystart, xstart, yend);
	LCD_DrawLine(xend, ystart, xend, yend);
}
/**********************************************/
/* 函数功能；矩形填充                         */
/**********************************************/
void LCD_Fill(uint16_t xstart ,uint16_t ystart ,uint16_t xend ,uint16_t yend ,uint16_t color)
{              
	u8 xs=xstart;
	POINT_COLOR=color;	
	if(xstart<xend)
  for(;xs<xend;xs++)
	{
	 LCD_DrawLine(xs, ystart, xs, yend);
	}
	else 	if(xstart>=xend)
  for(;xs>xend;xs--)
	{
	 LCD_DrawLine(xs, ystart, xs, yend);
	}
	POINT_COLOR=WHITE;
}
/*******************************************/
/* 函数功能：画圆                          */
/* 入口参数：x0,y0  圆心坐标               */
/*           r      半径(单位：像素)       */
/*******************************************/
void LCD_DrawCircle(uint8_t x0, uint16_t y0, uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 


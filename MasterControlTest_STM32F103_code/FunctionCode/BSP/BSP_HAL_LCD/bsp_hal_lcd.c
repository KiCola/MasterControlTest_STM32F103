/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "BSP_HAL_LCD/bsp_hal_lcd.h"
#include "BSP_HAL_LCD/font.h"
#include "string.h"	
/* ˽�����Ͷ��� --------------------------------------------------------------*/
uint16_t DotSite = 0;
uint16_t lcd_buf1[16*16]={0};
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
_lcd_dev lcddev;	//����LCD��Ҫ����
u16 POINT_COLOR=BLACK;	//������ɫ
u16 BACK_COLOR=WHITE;  //����ɫ 
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
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
//��Һ����дһ��8λָ��
void  LCD_WriteCommand(u8 c)
{
     LCD_A0_LOW();
     LCD_CS_LOW();
     LCD_SPI_ReadWriteByte(c);
     LCD_CS_HIGH();	
}
//========================================================
//��Һ����дһ��8λ����
void LCD_WriteData(u8 dat)
{
      LCD_A0_HIGH();
      LCD_CS_LOW();
      LCD_SPI_ReadWriteByte(dat);
      LCD_CS_HIGH();
}
//========================================================
//��Һ����дһ��16λ����
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
//����LCD��ʾ����6804��֧�ֺ�����ʾ��
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)//����
	{
		lcddev.dir=0;//����
		lcddev.width=Width;
		lcddev.height=Height;
		
	}else 
	{	  
		lcddev.dir=1;//����
		lcddev.width=Height;
		lcddev.height=Width;
		
	} 
	//LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
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
	LCD_WriteData(0x79); //��ʾ��Ļ�������� ��RPG��ɫ����
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
//��������
//color:Ҫ���������ɫ
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
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
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
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol;  
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
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


//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//void LCD_ShowChar(u8 x,u8 y,u8 num,u8 size,u8 mode)
//{  							  
//    u8 temp,t1,t;
//	u8 y0=y;
//	u16 colortemp=POINT_COLOR;   
//    u8 x_buf,y_buf;	 
//    x_buf = x;
//    y_buf = y;      
//	//���ô���		   
//	num=num-' ';//�õ�ƫ�ƺ��ֵ
//	if(!mode) //�ǵ��ӷ�ʽ
//	{
//	    for(t=0;t<size;t++)
//	    {   
//			if(size==12)temp=asc2_1206[num][t];  //����1206����
//			else temp=asc2_1608[num][t];		 //����1608���� 
//			for(t1=0;t1<8;t1++)
//			{			    
//		        if(temp&0x80)POINT_COLOR=colortemp;
//				else POINT_COLOR=BACK_COLOR;
//                
//                LCD_WR_DATA(POINT_COLOR,DotSite);
//				//LCD_DrawPoint(x,y);	
//				temp<<=1;
//				y++;
//				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
//					break;
//				}
//			}  	 
//	    }    
//	}else//���ӷ�ʽ
//	{
//	    for(t=0;t<size;t++)
//	    {   
//			if(size==12)temp=asc2_1206[num][t];  //����1206����
//			else temp=asc2_1608[num][t];		 //����1608���� 	                          
//	        for(t1=0;t1<8;t1++)
//			{			    
//		        if(temp&0x80)
//                {
//                    LCD_WR_DATA(POINT_COLOR,DotSite);
//                    //LCD_DrawPoint(x,y); 
//                }   
//				temp<<=1;
//				y++;
//				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//��������
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
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
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 mode,u16 fc, u16 bc)
{  
	u8 t,temp;
	u8 enshow=0;
    POINT_COLOR=fc;	//������ɫ
    BACK_COLOR=bc;  //����ɫ  
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
    POINT_COLOR=BLACK;	//������ɫ
    BACK_COLOR=WHITE;  //����ɫ 
} 




void LCD_ShowxPMNum(u8 x,u8 y,s32 num,u8 len,u8 size,u8 mode,u16 fc, u16 bc)
{  
	u8 t,temp;
	u8 enshow=0;
	u8 flg = 0;
    POINT_COLOR=fc;	//������ɫ
    BACK_COLOR=bc;  //����ɫ  
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
    POINT_COLOR=BLACK;	//������ɫ
    BACK_COLOR=WHITE;  //����ɫ 
} 




//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
//void LCD_ShowString(u8 x,u8 y,u8 width,u8 height,u8 size,u8 *p)
//{         
//	u8 x0=x;
//	width+=x;
//	height+=y;
//    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
//    {       
//        if(x>=width){x=x0;y+=size;}
//        if(y>=height)break;//�˳�
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
//��������  GUI_DrawFont16
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ʾ����16X16��������
//���������x,y :�������
//			fc:ǰ�û�����ɫ
//			bc:������ɫ	 
//			s:�ַ�����ַ
//			mode:ģʽ	0,���ģʽ;1,����ģʽ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 x0=x;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//�Զ�ͳ�ƺ�����Ŀ
	
			
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	
		  LCD_SetWindows(x,y,x+16-1,y+16-1);
		    for(i=0;i<16*2;i++)
		    {
				for(j=0;j<8;j++)
		    	{	
					if(!mode) //�ǵ��ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x80>>j))	
							LCD_Write_Data(fc);
						else
							LCD_Write_Data(bc);
					}
					else
					{
						POINT_COLOR=fc;
						if(tfont16[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//��һ����
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
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
	POINT_COLOR=BLACK;	//������ɫ
	BACK_COLOR=WHITE;  //����ɫ 
	//LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��  
} 
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar1(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
{  							  
    u8 temp;
    u8 pos,t1;
//	u8 y0=y;
    u8 x0=x;
	u16 colortemp=fc;//POINT_COLOR;      
		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	LCD_SetWindows(x,y,x+size/2-1,y+size-1);//���õ���������ʾ����
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp<<=1;
                x++;
                if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
                if((x-x0)==8)
				{
					x=x0;
					y++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
//				y++;
//				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
//					break;
//				}
			}  
			
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y); 
				temp<<=1;
                x++;
                if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
                if((x-x0)==8)
				{
					x=x0;
					y++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
//				y++;
//				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//��������
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
//					break;
//				}
			}  
		}
	}
	POINT_COLOR=colortemp;	
	POINT_COLOR=BLACK;	//������ɫ
	BACK_COLOR=WHITE;  //����ɫ   	   	 	  
}   
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString1(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{         
	//u16 y0=y;
	u16 x0=x;							  	  
  	u8 bHz=0;     //�ַ��������� 
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
			if(x>(lcddev.width-size/2)||y>(lcddev.height-size)) 
			return; 
	        if(*str>0x80)
						bHz=1;//���� 
	        else              //�ַ�
	        {          
		        if(*str==0x0D)//���з���
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else
				{
					if(size>16)//�ֿ���û�м���12X24 16X32��Ӣ������,��8X16����
					{  
					LCD_ShowChar1(x,y,fc,bc,*str,16,mode);
					x+=8; //�ַ�,Ϊȫ�ֵ�һ�� 
					}
					else
					{
					LCD_ShowChar1(x,y,fc,bc,*str,size,mode);
					x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
					}
				} 
				str++; 
		        
	        }
        }else//���� 
        {   
			if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
			return;  
            bHz=0;//�к��ֿ�    
			GUI_DrawFont16(x,y,fc,bc,str,mode);
				
	        str+=2; 
	        x+=size;//��һ������ƫ��	    
        }						 
    }	
}
void GUI_DrawFont16_BufMode(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 x0=x;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//�Զ�ͳ�ƺ�����Ŀ		
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	
		    for(i=0;i<16*2;i++)
		    {
				for(j=0;j<8;j++)
		    	{	
					if(!mode) //�ǵ��ӷ�ʽ
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
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
	POINT_COLOR=BLACK;	//������ɫ
	BACK_COLOR=WHITE;  //����ɫ 
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
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
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
                    }//��������
					break;
				}
				if(y>=lcddev.width)
                {
                    POINT_COLOR=colortemp;return;
                }//��������
			}  
			
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
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
					if(y>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
				if(y>=lcddev.height){POINT_COLOR=colortemp;return;}//��������
			}  
		}
	}
	POINT_COLOR=colortemp;	
	POINT_COLOR=BLACK;	//������ɫ
	BACK_COLOR=WHITE;  //����ɫ    
    
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
  	u8 bHz=0;     //�ַ��������� 
    while(*str!=0)//����δ����
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
				bHz=1;//���� 
			}
	        else              //�ַ�
	        {          
		        if(*str==0x0D)//���з���
		        {         
		            y+=size;
					x=x0;
		        }  
		        else
				{
					LCD_ShowChar_BufMode(x,y,fc,bc,*str,size,mode);
					x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
				} 
				str++; 
	        }
        }else//���� 
        {   
			if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
			return;  
            bHz=0;//�к��ֿ�    
			GUI_DrawFont16_BufMode(x,y,fc,bc,str,mode);	
	        str+=2; 
	        x+=16;//size;//��һ������ƫ��	    
        }						 
    }	
}
//******************************************************************
//��������  Gui_StrCenter
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ������ʾһ���ַ���,������Ӣ����ʾ
//���������x,y :�������
// 			fc:ǰ�û�����ɫ
//			bc:������ɫ
//			str :�ַ���	 
//			size:�����С
//			mode:ģʽ	0,���ģʽ;1,����ģʽ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************   
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{
    LCD_ShowString_BufMode(x,y,fc,bc,str,size,mode);
//	LCD_ShowString1(x/*+x1*/,y,fc,bc,str,size,mode);
} 


/**********************************************/
/* �������ܣ�������                           */
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
/* �������ܣ��������                         */
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
/* �������ܣ���Բ                          */
/* ��ڲ�����x0,y0  Բ������               */
/*           r      �뾶(��λ������)       */
/*******************************************/
void LCD_DrawCircle(uint8_t x0, uint16_t y0, uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 


/* 包含头文件 ----------------------------------------------------------------*/
#include "uartudp_str.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define abs(x) ((x)>0? (x):(-(x)))
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/** @brief  ASCII转HEX(2个字符串转一个字节)
  * @param  *p:字符串指针
  * @retval 返回值:HEX值
  */
uint8_t ASCII_HEX(uint8_t *p)
{
	uint8_t i;
	uint16_t Hex_buf;
	Hex_buf = 0;
	for(i=0; i<2; i++)
	{
		if(p[i] >= 'a')
		{
			Hex_buf = (p[i]-'a'+10)|(Hex_buf<<(i*4));
		}
		else if(p[i] >= 'A')
		{
			Hex_buf = (p[i]-'A'+10)|(Hex_buf<<(i*4));
		}
		else
		{
			Hex_buf = (p[i]-'0')|(Hex_buf<<(i*4));
		}
	}
	return Hex_buf;
}


/** @brief  HEX(一个字节)转ASCII
  * @param  *p:HEX码指针
            *str：转换完ASCII嘛存放位置
  * @retval 返回值:无
  */
void HEX_ASCII(uint8_t *p,uint8_t *str)
{
	uint8_t ascii_buf;
	ascii_buf = *p>>4;
    if(ascii_buf>9)
    {
        str[0] = (ascii_buf-10)+'A';
    }
    else
    {
        str[0] = ascii_buf+'0';
    }
   
    ascii_buf = *p&0x0f;
    if(ascii_buf>9)
    {
        str[1] = (ascii_buf-10)+'A';
    }
    else
    {
        str[1] = ascii_buf+'0';
    }
 
}
/** @brief  和效验
  * @param  *uBuff:字符串指针
          uBuffLen:字符串长度
  * @retval 返回值:和效验计算出的值，为HEX值
  */
unsigned char  CheckSum(unsigned char *uBuff, uint16_t uBuffLen)
{
	unsigned char uSum=0;
    uint16_t i;
	for(i=0; i<uBuffLen; i++)
	{
		uSum = uSum + uBuff[i];
	}

	uSum = 0x100-uSum;
    	
	return uSum;
}
/** @brief  对比字符串str1和str2
  * @param  *str1:字符串1指针
            *str2:字符串2指针
  * @retval 返回值:0，不相等;1，相等;
  */
uint8_t uartudp_strcmp(uint8_t *str1,uint8_t *str2)
{
	while(*str1)
	{
		if(*str1 != *str2)//不相等
        { 
            return 0;
        }
		str1++;
		str2++;
	}
	return 1;//两个字符串相等
}
/** @brief  复制字符串str2到str1
  * @param  *str1:字符串1指针
            *str2:字符串2指针
* @retval   返回复制的长度
  */
uint8_t uartudp_strcopy(uint8_t *str1,uint8_t *str2)
{
    uint8_t len = 0;
	while(*str2)
    {
        *str1 = *str2;
        str1++;
        str2++;
        len++;
    }
    return len;
}
uint8_t uartudp_typecopy(uint8_t *str1,_e_class classbuff,_e_type typebuff)
{
	uint8_t len = 0;

	if(classbuff == UARTUDP_PARA) str1[len++] = '(';
	else str1[len++] = '[';
	
	switch(typebuff)
	{
		case UARTUDP_F:
			str1[len++] = 'f';
			break;
		case UARTUDP_U8:
			str1[len++] = 'u';
			str1[len++] = '8';
			break;
		case UARTUDP_U16:
			str1[len++] = 'u';
			str1[len++] = '1';
			str1[len++] = '6';
			break;
		case UARTUDP_U32:
			str1[len++] = 'u';
			str1[len++] = '3';
			str1[len++] = '2';
			break;
		case UARTUDP_S8:
			str1[len++] = 's';
			str1[len++] = '8';
			break;
		case UARTUDP_S16:
			str1[len++] = 's';
			str1[len++] = '1';
			str1[len++] = '6';
			break;
		case UARTUDP_S32:
			str1[len++] = 's';
			str1[len++] = '3';
			str1[len++] = '2';
			break;
		default:
			break;
	}
	
	if(classbuff == UARTUDP_PARA) str1[len++] = ')';
	else str1[len++] = ']';
	
	 return len;
}
/** @brief  按str1中关键字进行str2移位
  * @param  *str1:字符串1指针
            *str2:字符串2指针
* @retval   返回值:1，有关键字;0，没有关键字;
  */
uint8_t uartudp_shifting(uint8_t *str1,uint8_t **str2)
{
	uint8_t i;
	uint8_t *str2buff;
	i = 0;
	str2buff = *str2;
	while(1)
	{
		if(str1[i] == *str2buff)
		{
			*str2 = str2buff;
			if(*str2buff != '\0')
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			if(*str2buff == '\0')
			{
				i++;
				str2buff = *str2;
			}
			else
			{
				str2buff++;
			}
		}
		
	}
}
uint8_t uartudp_same(uint8_t *str1,uint8_t s)
{
	uint8_t num;
    num =1;
    while((*str1)&&(*str1 != '\r'))
    {
        if(*str1 == s)
        {
            num++;
        }
        str1++;
    }
    return num;
}
/** @brief  计算str1长度
  * @param  *str1:字符串1指针
* @retval   返回值:str1长度
  */
uint8_t uartudp_len(uint8_t *str1)
{
	uint8_t len =0;
    while(*str1)
    {
        len++;
        str1++;
    }
    return len;
}
/** @briefm m^n函数

    @retval 返回值:m^n次方
**/
static uint32_t uartudp_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}
static void uartudp_sprintf_f(uint8_t *str1,void *data,_e_type typehc)
{
    uint8_t num,i,sat,j;
    uint32_t val,val1;
    float  datahcf;
    
    datahcf =  *((float*)data);
    sat = 0;      
    i = 0;
    
    if(datahcf < 0)
    {
        str1[i++] = '-';
        datahcf = abs(datahcf);
    }
    num=6;    
    for(j=0;j<2;j++)   
    { 
        val1 = datahcf;        
        for(; num>0; num--)
        {
            val = val1/uartudp_pow(10,num-1);
            val %= 10;
            val += '0';
            if((val >'0')||(sat)||(j)||(num==1))
            {
                sat = 1; 
                str1[i++] = val;
            }
        }
        if(j == 0)
        {
            str1[i++] = '.';
            val = datahcf;
            datahcf = (datahcf-val)*1000;
            num=3; 
        }
    }
    str1[i++] = '\0'; 
}

/** @brief  复制数据data到str1
  * @param  *str1:字符串1指针
            *str2:字符串2指针
* @retval   返回复制的长度
  */
uint8_t uartudp_datacopy(uint8_t *str1,void *data,_e_type typehc)
{
    uint8_t len = 0;
	switch(typehc)
	{
        case UARTUDP_F:
            uartudp_sprintf_f(str1,data,typehc);
			break;
		case UARTUDP_U8:
            sprintf((char*)str1,"%u",*((uint8_t*)data));
			break;
		case UARTUDP_U16:
            sprintf((char*)str1,"%u",*((uint16_t*)data));
			break;
		case UARTUDP_U32:
            sprintf((char*)str1,"%lu",*((uint32_t*)data));
			break;
		case UARTUDP_S8:
            sprintf((char*)str1,"%d",*((int8_t*)data));
			break;
		case UARTUDP_S16:
            sprintf((char*)str1,"%d",*((int16_t*)data));
			break;
		case UARTUDP_S32:
            sprintf((char*)str1,"%ld",*((int32_t*)data));
			break;
		default:
			break;
	}
    len = uartudp_len(str1);
	return len;
}
static void uartudp_atof(uint8_t *str1,void *data)
{
    uint8_t i;
    uint8_t sta;
    uint8_t symbol;
    float  datahcf;
    
    i = 0;
    sta = 0;
    symbol = '+';
    datahcf = 0.0f;
    if(*str1 == '-')
    {   
        symbol = '-';
        str1++;
    }
    while(((*str1>='0') && (*str1<='9')) || (*str1=='.'))
    {
        if(*str1 == '.')
        {
            sta = 1;
        }
        else
        {
            datahcf *= 10;
            datahcf += (*str1-'0');
            if(1 == sta) i++;
        }
        str1++;
    }
    for(;i>0;i--)
    {
        datahcf *= 0.1f;
    }
    if(symbol == '-')
    {
        *((float*)data) = -datahcf;
    }
    else
    {
        *((float*)data) = datahcf;
    }
}
uint8_t uartudp_dataset(uint8_t *str1,void *data,_e_type typehc)
{
    uint8_t len = 0;
    uint8_t *strch = str1;
    switch(typehc)
	{
        case UARTUDP_F:
            uartudp_atof(str1,data);
			break;
		case UARTUDP_U8:
            *((uint8_t*)data) = atoi((char*)strch);
			break;
		case UARTUDP_U16:
            *((uint16_t*)data) = atoi((char*)strch);
			break;
		case UARTUDP_U32:
            *((uint32_t*)data) = atol((char*)strch);//此处值太大还是有问题
			break;
		case UARTUDP_S8:
            *((int8_t*)data) = atoi((char*)strch);
			break;
		case UARTUDP_S16:
            *((int16_t*)data) = atoi((char*)strch);
			break;
		case UARTUDP_S32:
            *((int32_t*)data) = atoi((char*)strch);
			break;
		default:
			break;
	}
    return len;
}

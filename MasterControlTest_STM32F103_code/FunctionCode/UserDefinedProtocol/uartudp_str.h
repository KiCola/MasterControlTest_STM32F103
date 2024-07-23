#ifndef __UARTUDP_STR_H__
#define __UARTUDP_STR_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include <stdio.h> 
#include <stdlib.h>
/* 类型定义 ------------------------------------------------------------------*/
typedef enum {     
	UARTUDP_NOT = 0,
	UARTUDP_PARA,
	UARTUDP_ARRAY,
}_e_class;//PDU数据类别
typedef enum {
	UARTUDP_VOID = 0,
	UARTUDP_F,
	UARTUDP_U8,
	UARTUDP_U16,
	UARTUDP_U32,
	UARTUDP_S8,
	UARTUDP_S16,
	UARTUDP_S32,
	UARTUDP_STRING,
} _e_type;//PDU命令类型
/* 宏定义 --------------------------------------------------------------------*/


/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
uint8_t ASCII_HEX(uint8_t *p);
void HEX_ASCII(uint8_t *p,uint8_t *str);
unsigned char  CheckSum(unsigned char *uBuff, uint16_t uBuffLen);
uint8_t uartudp_strcmp(uint8_t *str1,uint8_t *str2);
uint8_t uartudp_strcopy(uint8_t *str1,uint8_t *str2);
uint8_t uartudp_typecopy(uint8_t *str1,_e_class classbuff,_e_type typebuff);
uint8_t uartudp_shifting(uint8_t *str1,uint8_t **str2);
uint8_t uartudp_same(uint8_t *str1,uint8_t s);
uint8_t uartudp_len(uint8_t *str1);
uint8_t uartudp_datacopy(uint8_t *str1,void *data,_e_type typehc);
uint8_t uartudp_dataset(uint8_t *str1,void *data,_e_type typehc);










#endif  // __UARTUDP_STR_H__



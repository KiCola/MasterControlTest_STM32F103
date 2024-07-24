#ifndef __UARTUDP_CONFIG_H__
#define __UARTUDP_CONFIG_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "uartudp.h"
/* 宏定义 --------------------------------------------------------------------*/



/* 用户宏定义 --------------------------------------------------------------------*/
    


/* 类型定义 ------------------------------------------------------------------*/
#define UARTUDP_MODE_SLAVE 0

#define UARTUDP_RX_LEN 100
#define UARTUDP_TX_LEN 200

#define NR_OF_SLAVE_NAMETABE 1  //不能为0
#define NR_OF_HOST_NAMETABE  3  //不能为0
/* 扩展变量 ------------------------------------------------------------------*/
extern uint8_t  UARTUDP_RX_BUF[UARTUDP_RX_LEN]; 
extern uint16_t UARTUDP_RX_STA; 
extern uint8_t  UARTUDP_TX_BUF[UARTUDP_TX_LEN]; 
extern uint16_t UARTUDP_TX_STA;
extern	_s_uartudp_dev uartudp_dev;	 
/* 函数声明 ------------------------------------------------------------------*/
void UartudpInitialUsedPGs(void);

 













#endif  // __UARTUDP_CONFIG_H__

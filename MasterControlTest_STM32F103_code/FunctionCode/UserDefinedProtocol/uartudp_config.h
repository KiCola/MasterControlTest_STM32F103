#ifndef __UARTUDP_CONFIG_H__
#define __UARTUDP_CONFIG_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "uartudp.h"
/* �궨�� --------------------------------------------------------------------*/



/* �û��궨�� --------------------------------------------------------------------*/
    


/* ���Ͷ��� ------------------------------------------------------------------*/
#define UARTUDP_MODE_SLAVE 0

#define UARTUDP_RX_LEN 100
#define UARTUDP_TX_LEN 200

#define NR_OF_SLAVE_NAMETABE 1  //����Ϊ0
#define NR_OF_HOST_NAMETABE  3  //����Ϊ0
/* ��չ���� ------------------------------------------------------------------*/
extern uint8_t  UARTUDP_RX_BUF[UARTUDP_RX_LEN]; 
extern uint16_t UARTUDP_RX_STA; 
extern uint8_t  UARTUDP_TX_BUF[UARTUDP_TX_LEN]; 
extern uint16_t UARTUDP_TX_STA;
extern	_s_uartudp_dev uartudp_dev;	 
/* �������� ------------------------------------------------------------------*/
void UartudpInitialUsedPGs(void);

 













#endif  // __UARTUDP_CONFIG_H__

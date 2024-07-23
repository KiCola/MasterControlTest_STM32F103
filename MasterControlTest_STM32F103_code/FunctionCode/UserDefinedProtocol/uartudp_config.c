/* 包含头文件 ----------------------------------------------------------------*/
#include "uartudp_config.h"
/* 用户包含头文件 ------------------------------------------------------------*/


/* 私有类型定义 --------------------------------------------------------------*/
 
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
_s_uartudp_nametab uartudp_nametab_rx[NR_OF_SLAVE_NAMETABE];
_s_uartudp_nametab_tx uartudp_nametab_tx[NR_OF_HOST_NAMETABE];
/* 扩展变量 ------------------------------------------------------------------*/
_s_uartudp_dev uartudp_dev = {
	"V_1_0_0",         //版本号
	"0100",            //id号
	UARTUDP_TX_FREE,   //发送状态
	UARTUDP_NOT,       //数据类别
	UARTUDP_RETAIN,    //命令类别
	UARTUDP_VOID,      //参数类型
	UARTUDP_HOST,     //模式,(保留，主模式，从模式)
	0,                 //主模式发送命令个数
	0,                 //从模式命令个数
	0,				   //运行时间
	uartudp_nametab_tx,//指向发送命令的指针
	uartudp_nametab_rx,//指向接收命令的指针
	/*************指向运行的函数****************/
	uartudp_init,      //数据初始化函数
	uartudp_scan,      //任务处理函数
    uartudp_receive,   //数据接收函数
    uartudp_transfer,  //数据发送函数
	uartudp_CreateSlaveCommand, //声明一个从机命令
	uartudp_CreateSendCommand,  //声明一个主机命令
};
uint8_t UARTUDP_RX_BUF[UARTUDP_RX_LEN];   
uint16_t UARTUDP_RX_STA = 0;   
uint8_t  UARTUDP_TX_BUF[UARTUDP_TX_LEN]; 
uint16_t UARTUDP_TX_STA = 0; 
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
__weak void UartudpInitialUsedPGs(void)
{
	
	
}





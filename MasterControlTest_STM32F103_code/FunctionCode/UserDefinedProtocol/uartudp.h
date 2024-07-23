#ifndef __UARTUDP_H__
#define __UARTUDP_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "uartudp_str.h"


/* 类型定义 ------------------------------------------------------------------*/
typedef enum {
	UARTUDP_PG_TX_FREE = 0,
	UARTUDP_PG_TX_REQ,
	UARTUDP_PG_TX_TxING,
	UARTUDP_PG_TX_SUC,
	UARTUDP_PG_TX_ERR,
}_e_PGState;//PDU运行状态
typedef enum {
	UARTUDP_ASYNC = 0,
	UARTUDP_CYCLIC,
}_e_TransType;//PDU传输类型
typedef enum {
	UARTUDP_I = 0,
	UARTUDP_IO,
	UARTUDP_O,
}_e_IOstate;//PDU读写状态
     
typedef enum {
	UARTUDP_RETAIN = 0,
	UARTUDP_HELP,
	UARTUDP_ID,
	UARTUDP_GET_,
	UARTUDP_SET_,
} _e_order;//PDU命令类型
typedef enum {
	UARTUDP_UNDEFINED = 0,
	UARTUDP_SLAVE,
	UARTUDP_HOST,
	UARTUDP_INSTRUMENT
} _e_mode;//协议模式
typedef enum {
	UARTUDP_TX_FREE = 0,
	UARTUDP_TX_TxING,
}_e_TxSta;

typedef struct _s_uartudp_nametab {
	_e_IOstate   IOstate;   //PDU读写状态
	_e_class     Class;     //PDU数据类别
	_e_type      Parmtype;  //PDU参数类型
	uint8_t plen;
	uint8_t typelen;
	uint8_t namelen;
	void* func;	
	uint8_t* name;
	void (*ComSucceedCallBack)(struct _s_uartudp_nametab *pQ,_e_order Order);
}_s_uartudp_nametab;
typedef struct _s_uartudp_nametab_tx{
	_e_IOstate   IOstate;   //PDU读写状态
	_e_class     Class;     //PDU数据类别
	_e_type      Parmtype;  //PDU参数类型
	_e_PGState   PGState;   //PDU运行状态
	_e_TransType TransType; //PDU传输类型
	uint8_t id[5];          //id号
	uint8_t      txerrnum;  //发送失败次数
	uint8_t plen;
	uint8_t typelen;
	uint8_t namelen;
	uint16_t TransRate;  //Trans. rate in ms, only for cyclic 
	uint16_t Timer;      //Timer in ms
	void* func;	
	uint8_t* name;
	void (*ComSucceedCallBack)(struct _s_uartudp_nametab_tx *pQ);
	void (*OvertimeCallBack)(struct _s_uartudp_nametab_tx *pQ);
}_s_uartudp_nametab_tx;

typedef struct {
	const uint8_t versions[8];  //版本号
	uint8_t  id[5];      //id号
	_e_TxSta TxSta;      //发送状态（主机模式有用） 
	_e_class Class;      //数据类别，参数或数组
	_e_order Order;      //命令类别
	_e_type  Parmtype;   //参数类型
	_e_mode  mode_;      //模式,保留，主模式，从模式
	uint16_t pnum_tx;    //命令发送个数
	uint16_t pnum_rx;    //命令接收个数
	uint16_t Timer;      //运行时间
	_s_uartudp_nametab_tx *para_tx; //指向发送命令的指针
	_s_uartudp_nametab    *para_rx; //指向接收命令的指针
	/*************指向运行的函数****************/
	void (*init)(void);
    void (*scan)(void);
    void (*receive)(uint8_t *str,uint16_t len);
    uint8_t (*transfer)(uint8_t *str,uint16_t *len);  
	uint8_t (*CreateSlaveCommand)(	_e_IOstate   IOstate,
									_e_class     Class,     
									_e_type      Parmtype,
									uint8_t plen,
									void* func,
                                    uint8_t* name,
									void (*ComSucceedCallBack)(struct _s_uartudp_nametab *pQ,_e_order Order));
	uint8_t (*CreateSendCommand)(	_e_IOstate   IOstate,
									_e_class     Class,    
									_e_type      Parmtype,
									uint8_t  plen,
									uint8_t* id,
									void*    func,
									uint8_t* name,
									_e_TransType TransType,
									_e_PGState   PGState,
									uint16_t TransRate,
									uint16_t Timer,
									void (*ComSucceedCallBack)(struct _s_uartudp_nametab_tx *pQ),
									void (*OvertimeCallBack)(struct _s_uartudp_nametab_tx *pQ));	       								
}_s_uartudp_dev;
/* 宏定义 --------------------------------------------------------------------*/


/* 扩展变量 ------------------------------------------------------------------*/

/* 函数声明 ------------------------------------------------------------------*/
void uartudp_init(void);
void uartudp_scan(void);
void uartudp_receive(uint8_t *str,uint16_t len);
uint8_t uartudp_transfer(uint8_t *str,uint16_t *len);
uint8_t uartudp_CreateSlaveCommand(_e_IOstate   IOstate,
								   _e_class     Class,     
								   _e_type      Parmtype,
                                   uint8_t plen,
                                   void* func,
                                   uint8_t* name,
								   void (*ComSucceedCallBack)(struct _s_uartudp_nametab *pQ,_e_order Order));
								   
uint8_t uartudp_CreateSendCommand(	_e_IOstate   IOstate,
									_e_class     Class,    
									_e_type      Parmtype,
									uint8_t  plen,
									uint8_t* id,
									void*    func,
									uint8_t* name,
									_e_TransType TransType,
									_e_PGState   PGState,
									uint16_t TransRate,
									uint16_t Timer,
									void (*ComSucceedCallBack)(struct _s_uartudp_nametab_tx *pQ),
									void (*OvertimeCallBack)(struct _s_uartudp_nametab_tx *pQ));	
                                 						   


#endif  // __UARTUDP_H__








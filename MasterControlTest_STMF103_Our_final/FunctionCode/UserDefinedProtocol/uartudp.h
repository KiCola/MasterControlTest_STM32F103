#ifndef __UARTUDP_H__
#define __UARTUDP_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "uartudp_str.h"


/* ���Ͷ��� ------------------------------------------------------------------*/
typedef enum {
	UARTUDP_PG_TX_FREE = 0,
	UARTUDP_PG_TX_REQ,
	UARTUDP_PG_TX_TxING,
	UARTUDP_PG_TX_SUC,
	UARTUDP_PG_TX_ERR,
}_e_PGState;//PDU����״̬
typedef enum {
	UARTUDP_ASYNC = 0,
	UARTUDP_CYCLIC,
}_e_TransType;//PDU��������
typedef enum {
	UARTUDP_I = 0,
	UARTUDP_IO,
	UARTUDP_O,
}_e_IOstate;//PDU��д״̬
     
typedef enum {
	UARTUDP_RETAIN = 0,
	UARTUDP_HELP,
	UARTUDP_ID,
	UARTUDP_GET_,
	UARTUDP_SET_,
} _e_order;//PDU��������
typedef enum {
	UARTUDP_UNDEFINED = 0,
	UARTUDP_SLAVE,
	UARTUDP_HOST,
	UARTUDP_INSTRUMENT
} _e_mode;//Э��ģʽ
typedef enum {
	UARTUDP_TX_FREE = 0,
	UARTUDP_TX_TxING,
}_e_TxSta;

typedef struct _s_uartudp_nametab {
	_e_IOstate   IOstate;   //PDU��д״̬
	_e_class     Class;     //PDU�������
	_e_type      Parmtype;  //PDU��������
	uint8_t plen;
	uint8_t typelen;
	uint8_t namelen;
	void* func;	
	uint8_t* name;
	void (*ComSucceedCallBack)(struct _s_uartudp_nametab *pQ,_e_order Order);
}_s_uartudp_nametab;
typedef struct _s_uartudp_nametab_tx{
	_e_IOstate   IOstate;   //PDU��д״̬
	_e_class     Class;     //PDU�������
	_e_type      Parmtype;  //PDU��������
	_e_PGState   PGState;   //PDU����״̬
	_e_TransType TransType; //PDU��������
	uint8_t id[5];          //id��
	uint8_t      txerrnum;  //����ʧ�ܴ���
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
	const uint8_t versions[8];  //�汾��
	uint8_t  id[5];      //id��
	_e_TxSta TxSta;      //����״̬������ģʽ���ã� 
	_e_class Class;      //������𣬲���������
	_e_order Order;      //�������
	_e_type  Parmtype;   //��������
	_e_mode  mode_;      //ģʽ,��������ģʽ����ģʽ
	uint16_t pnum_tx;    //����͸���
	uint16_t pnum_rx;    //������ո���
	uint16_t Timer;      //����ʱ��
	_s_uartudp_nametab_tx *para_tx; //ָ���������ָ��
	_s_uartudp_nametab    *para_rx; //ָ����������ָ��
	/*************ָ�����еĺ���****************/
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
/* �궨�� --------------------------------------------------------------------*/


/* ��չ���� ------------------------------------------------------------------*/

/* �������� ------------------------------------------------------------------*/
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








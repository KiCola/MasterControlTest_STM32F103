/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "uartudp_config.h"
/* �û�����ͷ�ļ� ------------------------------------------------------------*/


/* ˽�����Ͷ��� --------------------------------------------------------------*/
 
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
_s_uartudp_nametab uartudp_nametab_rx[NR_OF_SLAVE_NAMETABE];
_s_uartudp_nametab_tx uartudp_nametab_tx[NR_OF_HOST_NAMETABE];
/* ��չ���� ------------------------------------------------------------------*/
_s_uartudp_dev uartudp_dev = {
	"V_1_0_0",         //�汾��
	"0100",            //id��
	UARTUDP_TX_FREE,   //����״̬
	UARTUDP_NOT,       //�������
	UARTUDP_RETAIN,    //�������
	UARTUDP_VOID,      //��������
	UARTUDP_HOST,     //ģʽ,(��������ģʽ����ģʽ)
	0,                 //��ģʽ�����������
	0,                 //��ģʽ�������
	0,				   //����ʱ��
	uartudp_nametab_tx,//ָ���������ָ��
	uartudp_nametab_rx,//ָ����������ָ��
	/*************ָ�����еĺ���****************/
	uartudp_init,      //���ݳ�ʼ������
	uartudp_scan,      //��������
    uartudp_receive,   //���ݽ��պ���
    uartudp_transfer,  //���ݷ��ͺ���
	uartudp_CreateSlaveCommand, //����һ���ӻ�����
	uartudp_CreateSendCommand,  //����һ����������
};
uint8_t UARTUDP_RX_BUF[UARTUDP_RX_LEN];   
uint16_t UARTUDP_RX_STA = 0;   
uint8_t  UARTUDP_TX_BUF[UARTUDP_TX_LEN]; 
uint16_t UARTUDP_TX_STA = 0; 
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
__weak void UartudpInitialUsedPGs(void)
{
	
	
}





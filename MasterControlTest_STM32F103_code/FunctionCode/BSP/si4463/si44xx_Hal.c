/* Includes ------------------------------------------------------------------*/
#include	"platform.h"
#include	"stdint.h"
//#include "stm32f0xx_hal.h"
#include "spi.h"
/* Private typedef -----------------------------------------------------------*/
/* Private constants----------------------------------------------------------*/
/* ���ڿ���SPI�������ʣ����ݲ�ͬ��MCU�ٶ�����������ǰΪMCU��ƵΪ48MHz    */
#define TIME   20 /* TIME   5   ��Ӧ��400Kbps*/

#define RADIO_CTS_TIMEOUT 10000

/* Private macro -------------------------------------------------------------*/
//*  ��ֵʱ��Ҫ�޸ĵĺ궨��    */	
/*   SPI-SEL   :  PA4             */
/*   SPI-MOSI  :  PA7             */
/*   SPI-MISO  :  PA6             */
/*   SPI-CLK   :  PA5             */
/*   RST	   :  PA1            */
/*   IRQ	   :  PA0             */
/*   DIO1	   :               */
/*   DIO2      :               */
#define SEL_L()      HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET)                 //                        
#define SEL_H()      HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET)  


#define RST_L()      HAL_GPIO_WritePin(SDN_GPIO_Port, SDN_Pin, GPIO_PIN_RESET)                 //                        
#define RST_H()      HAL_GPIO_WritePin(SDN_GPIO_Port, SDN_Pin, GPIO_PIN_SET)

#define IRQ_Level()   HAL_GPIO_ReadPin(IRQ_GPIO_Port, IRQ_Pin)           /* ��ȡģ���ж��������״̬ */


#define HSPI hspi1
//#define DIO1_Level()  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)           
//#define DIO2_Level()  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)           
/* Public  variables ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ctsWentHigh=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/************************������ʱ***************************/
void delay(uint32_t T)
{
	uint32_t j,k;
	for (j = 0; j <2; j++){
	   for (k = 0; k < T; k++);
	}
}

/************************SPI��дһ���ֽ�************************/
//uint8_t SPI_ReadWriteByte(uint8_t TxData)
//{
//	u16 i=0;
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){i++;if(i>=10000)break;}//�ȴ���������  
//	
//	SPI_SendData8(SPI2, TxData); //ͨ������SPIx����һ��byte  ����
//	i=0;	
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_SR_BSY) == RESET){i++;if(i>=10000)break;} //�ȴ�������һ��byte  
// 
//	return SPI_ReceiveData8(SPI2); //����ͨ��SPIx������յ�����	
//}

/************************��ʼ��IO��**************************/
void si44xx_init_io(void)
{
	uint8_t txdata=0;
    HAL_SPI_Transmit(&HSPI,&txdata, 1,10000);
//	SPI_ReadWriteByte(0x00);//��������	
	
	SEL_H();  

}
/************************��λSi44xx**************************/
void si44xx_SetReset(bool bit)
{
	if(bit==false)
		RST_H();
	else
		RST_L();
}


/************************SPI��һ������************************/
void SpiReadData(uint8_t size,uint8_t *Rbuff)
{
    SEL_L();
    HAL_SPI_Receive(&HSPI, Rbuff, size, 10000);
    SEL_H();
}
/************************SPIдһ������************************/
void SpiWriteData(uint8_t size,uint8_t *Sbuff)
{
//	uint8_t i;
	SEL_L();
	HAL_SPI_Transmit(&HSPI, Sbuff, size,10000);
   SEL_H();
}
/*************************Si44xx��ȡ�Ĵ�����ֵ*******************/
uint8_t radio_comm_GetResp(uint8_t Rcnt,uint8_t *Rbuff)
{
	uint8_t ctsVal=0;
	uint8_t txdata=0x44;
	uint16_t errCnt=RADIO_CTS_TIMEOUT;//��ʱ
	while(errCnt!=0)
	{
		SEL_L();
		HAL_SPI_Transmit(&HSPI,&txdata, 1,10000);
//		SPI_ReadWriteByte(0x44);
		HAL_SPI_Receive(&HSPI, &ctsVal, 1, 10000);
//		ctsVal = SPI_ReadWriteByte(0x00);
		if(ctsVal==0xff)
		{
			if(Rcnt)
			{
				SpiReadData(Rcnt,Rbuff);
			}
			SEL_H();
			break;
		}
		SEL_H();
		errCnt--;
	}
	if(errCnt==0)
	{}
	if(ctsVal==0xff)ctsWentHigh=1;
	return ctsVal;
}
/*************************�ȴ�Si44xx����*************************/
uint8_t radio_comm_PollCTS(void)
{
	return radio_comm_GetResp(0 , 0);
}
/*************************Si44xx��������*************************/
void radio_comm_SendCmd(uint8_t byteCount, uint8_t* pData)
{
	while(!ctsWentHigh){radio_comm_PollCTS();}
	SEL_L();
	HAL_SPI_Transmit(&HSPI,pData, byteCount,10000);
//	SpiWriteData(byteCount,pData);
	SEL_H();
	ctsWentHigh = 0;
}
/********************Si44xx���������ȡ����ֵ********************/
uint8_t radio_comm_SendCmdGetResp(uint8_t cmdByteCount, uint8_t* pCmdData, uint8_t respByteCount, uint8_t* pRespData)
{
	radio_comm_SendCmd(cmdByteCount, pCmdData);
    return radio_comm_GetResp(respByteCount, pRespData);
}
/************************Si44xx��һ��Ĵ���************************/
void radio_comm_ReadData(uint8_t cmd, bool pollCts, uint8_t byteCount, uint8_t* pData)
{
	if(pollCts==1){while(!ctsWentHigh){radio_comm_PollCTS();}}
    SEL_L();
	HAL_SPI_Transmit(&HSPI, &cmd, 1,10000);
//    SPI_ReadWriteByte(cmd);
    SpiReadData(byteCount, pData);
    SEL_H();
	ctsWentHigh = 0;
}
/************************Si44xxдһ��Ĵ���************************/
void radio_comm_WriteData(uint8_t cmd, bool pollCts, uint8_t byteCount, uint8_t* pData)
{
    if(pollCts==1){while(!ctsWentHigh){radio_comm_PollCTS();}}
    SEL_L();
	HAL_SPI_Transmit(&HSPI, &cmd, 1,10000);
//    SPI_ReadWriteByte(cmd);
    SpiWriteData(byteCount, pData);
    SEL_H();
	ctsWentHigh = 0;
}
/****************************��ȡIrq�ĵ�ƽ*************************/
uint8_t radio_irqLevel(void)
{
    return IRQ_Level();
}
/****************************���CTS��־***************************/
void radio_comm_ClearCTS(void)
{
  ctsWentHigh = 0;
}

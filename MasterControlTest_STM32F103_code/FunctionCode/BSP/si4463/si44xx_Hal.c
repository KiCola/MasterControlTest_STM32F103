/* Includes ------------------------------------------------------------------*/
#include	"platform.h"
#include	"stdint.h"
//#include "stm32f0xx_hal.h"
#include "spi.h"
/* Private typedef -----------------------------------------------------------*/
/* Private constants----------------------------------------------------------*/
/* 用于控制SPI总线速率，根据不同的MCU速度来调整；当前为MCU主频为48MHz    */
#define TIME   20 /* TIME   5   对应着400Kbps*/

#define RADIO_CTS_TIMEOUT 10000

/* Private macro -------------------------------------------------------------*/
//*  移值时需要修改的宏定义    */	
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

#define IRQ_Level()   HAL_GPIO_ReadPin(IRQ_GPIO_Port, IRQ_Pin)           /* 读取模块中断输出引脚状态 */


#define HSPI hspi1
//#define DIO1_Level()  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)           
//#define DIO2_Level()  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)           
/* Public  variables ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ctsWentHigh=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/************************稍作延时***************************/
void delay(uint32_t T)
{
	uint32_t j,k;
	for (j = 0; j <2; j++){
	   for (k = 0; k < T; k++);
	}
}

/************************SPI读写一个字节************************/
//uint8_t SPI_ReadWriteByte(uint8_t TxData)
//{
//	u16 i=0;
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){i++;if(i>=10000)break;}//等待发送区空  
//	
//	SPI_SendData8(SPI2, TxData); //通过外设SPIx发送一个byte  数据
//	i=0;	
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_SR_BSY) == RESET){i++;if(i>=10000)break;} //等待接收完一个byte  
// 
//	return SPI_ReceiveData8(SPI2); //返回通过SPIx最近接收的数据	
//}

/************************初始化IO口**************************/
void si44xx_init_io(void)
{
	uint8_t txdata=0;
    HAL_SPI_Transmit(&HSPI,&txdata, 1,10000);
//	SPI_ReadWriteByte(0x00);//启动传输	
	
	SEL_H();  

}
/************************复位Si44xx**************************/
void si44xx_SetReset(bool bit)
{
	if(bit==false)
		RST_H();
	else
		RST_L();
}


/************************SPI读一个数组************************/
void SpiReadData(uint8_t size,uint8_t *Rbuff)
{
    SEL_L();
    HAL_SPI_Receive(&HSPI, Rbuff, size, 10000);
    SEL_H();
}
/************************SPI写一个数组************************/
void SpiWriteData(uint8_t size,uint8_t *Sbuff)
{
//	uint8_t i;
	SEL_L();
	HAL_SPI_Transmit(&HSPI, Sbuff, size,10000);
   SEL_H();
}
/*************************Si44xx获取寄存器的值*******************/
uint8_t radio_comm_GetResp(uint8_t Rcnt,uint8_t *Rbuff)
{
	uint8_t ctsVal=0;
	uint8_t txdata=0x44;
	uint16_t errCnt=RADIO_CTS_TIMEOUT;//超时
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
/*************************等待Si44xx就绪*************************/
uint8_t radio_comm_PollCTS(void)
{
	return radio_comm_GetResp(0 , 0);
}
/*************************Si44xx发送命令*************************/
void radio_comm_SendCmd(uint8_t byteCount, uint8_t* pData)
{
	while(!ctsWentHigh){radio_comm_PollCTS();}
	SEL_L();
	HAL_SPI_Transmit(&HSPI,pData, byteCount,10000);
//	SpiWriteData(byteCount,pData);
	SEL_H();
	ctsWentHigh = 0;
}
/********************Si44xx发送命令并获取返回值********************/
uint8_t radio_comm_SendCmdGetResp(uint8_t cmdByteCount, uint8_t* pCmdData, uint8_t respByteCount, uint8_t* pRespData)
{
	radio_comm_SendCmd(cmdByteCount, pCmdData);
    return radio_comm_GetResp(respByteCount, pRespData);
}
/************************Si44xx读一组寄存器************************/
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
/************************Si44xx写一组寄存器************************/
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
/****************************读取Irq的电平*************************/
uint8_t radio_irqLevel(void)
{
    return IRQ_Level();
}
/****************************清除CTS标志***************************/
void radio_comm_ClearCTS(void)
{
  ctsWentHigh = 0;
}

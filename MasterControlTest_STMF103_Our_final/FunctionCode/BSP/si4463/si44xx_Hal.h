/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SI44XX_HAL_H__
#define __SI44XX_HAL_H__

/* Includes ------------------------------------------------------------------*/
#include	"platform.h"
#include	"stdint.h"
//#include "stm32f0xx_hal.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void si44xx_init_io(void);
void si44xx_SetReset(bool bit);
uint8_t SPI_ReadWriteByte(uint8_t TxData);
void SpiReadData(uint8_t size,uint8_t *Rbuff);
void SpiWriteData(uint8_t size,uint8_t *Sbuff);
void radio_comm_ReadData(uint8_t cmd, bool pollCts, uint8_t byteCount, uint8_t* pData);
void radio_comm_WriteData(uint8_t cmd, bool pollCts, uint8_t byteCount, uint8_t* pData);
uint8_t WaitForCTS(void);
void radio_comm_SendCmd(uint8_t byteCount, uint8_t* pData);
uint8_t radio_comm_GetResp(uint8_t Rcnt,uint8_t *Rbuff);
uint8_t radio_comm_SendCmdGetResp(uint8_t cmdByteCount, uint8_t* pCmdData, uint8_t respByteCount, uint8_t* pRespData);
uint8_t radio_irqLevel(void);
void radio_comm_ClearCTS(void);

#endif	/* __SI44XX_HAL_H__ */

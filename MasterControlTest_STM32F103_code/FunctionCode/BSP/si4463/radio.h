/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RADIO_H__
#define __RADIO_H__

/* Includes ------------------------------------------------------------------*/
#include	"stdint.h"
#include	"si44xx.h"

/* Exported types ------------------------------------------------------------*/
//RF process function return codes
//Radio driver structure defining the different function pointers
typedef struct sRadioDriver
{
	uint8_t (*Init)				(void);
    void 	(*Reset)			(void);
    void 	(*RFRxStateEnter)	(uint8_t channel, uint8_t packetLenght);
	void 	(*RFGetIntStatus)	(uint8_t PH_CLR_PEND, uint8_t MODEM_CLR_PEND, uint8_t CHIP_CLR_PEND);
	void 	(*RFChangeState)	(uint8_t NEXT_STATE1);
	void 	(*RFTxPacket )		(uint8_t channel, uint8_t *pioRadioPacket, uint8_t length);
	uint8_t (*RFEvent)			(void);
	uint8_t (*GetRssi)			(void);
}tRadioDriver;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
tRadioDriver* RadioDriverInit( void );

#endif	/* __RADIO_H__ */

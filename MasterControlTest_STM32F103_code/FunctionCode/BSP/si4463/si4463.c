/* Includes ------------------------------------------------------------------*/
#include	"si4463.h"
#include	"radio_config_Si4463.h"
#include	"si44xx_Hal.h"
#include	"si44xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private constants----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Public  variables ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t Si4463RegTbl[] = RADIO_CONFIGURATION_DATA_ARRAY;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*****************************ÅäÖÃSI4463**************************/
uint8_t Si4463_Config_Init(void)
{
	uint8_t	i,iData=0xff;
	uint32_t iCnt=0x00;
	iData=Si4463RegTbl[iCnt];
	while(iData!=0x00)
	{
		if(iData>16)return SI446X_COMMAND_ERROR;
		iCnt++;
		i++;
		if(radio_comm_SendCmdGetResp(iData, (uint8_t*)&Si4463RegTbl[iCnt], 0, 0)!=0xff) 
			return SI446X_CTS_TIMEOUT;
		if(radio_irqLevel()==0)
		{
			si446x_get_int_status(0, 0, 0);
			if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CHIP_PEND_CMD_ERROR_PEND_MASK)
			{
				return SI446X_COMMAND_ERROR;
			}
		}
		iCnt += iData;
		iData=Si4463RegTbl[iCnt];
	}
	return SI446X_SUCCESS;
}

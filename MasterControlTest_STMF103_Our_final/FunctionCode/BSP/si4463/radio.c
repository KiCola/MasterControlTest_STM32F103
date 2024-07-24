/* Includes ------------------------------------------------------------------*/
#include	"platform.h"
#include	"radio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private constants----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Public  variables ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
tRadioDriver RadioDriver;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

tRadioDriver* RadioDriverInit( void )
{
    RadioDriver.Init 			= 	si44xx_init;
	RadioDriver.Reset			= 	si446x_reset;
	RadioDriver.RFChangeState	=	si446x_change_state;
	RadioDriver.RFEvent			=	si446x_check_event;
	RadioDriver.RFGetIntStatus	=	si446x_get_int_status;
	RadioDriver.RFRxStateEnter	=	si446x_enter_rx;
	RadioDriver.RFTxPacket		=	si446x_send_packet;
	RadioDriver.GetRssi			=	si446x_get_rssi;

    return &RadioDriver;
}

/* Includes ------------------------------------------------------------------*/
#include	"platform.h"
#include	"si44xx.h"
#include	"si44xx_Hal.h"
#include	"si4463.h"



/* Private typedef -----------------------------------------------------------*/
/* Private constants----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Public  variables ---------------------------------------------------------*/
union si446x_cmd_reply_union Si446xCmd;
uint8_t	Packet[64];
/* Private variables ---------------------------------------------------------*/
uint8_t	Pro2Cmd[16];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/***************************重启SI44xx**************************/
void si446x_reset(void)
{
	si44xx_SetReset(false);//拉高SDN电平
	HAL_Delay(5);
	si44xx_SetReset(true); //拉低SDN电平
	HAL_Delay(5);
	radio_comm_ClearCTS();
}
/************************初始化SI44xx**************************/
uint8_t si44xx_init(void)
{
	uint8_t i; 
	si44xx_init_io();
	si446x_reset();
	si446x_part_info();
  if((Si446xCmd.PART_INFO.CHIPREV==0x11)&&(Si446xCmd.PART_INFO.PART==0x4463))
	{
		i=Si4463_Config_Init();
		if(i==SI446X_SUCCESS) 
		{
//			printf("SI4463_INIT_SUCCESS\r\n");
			return true;
		}
//		else if(i==SI446X_COMMAND_ERROR)
//		{
////			printf("SI4463_COMMAND_ERROR\r\n");
//		}
//		else 
//		{
////			printf("SI4463_CTS_TIMEOUT\r\n");
//		}
	}
//	else
//	{
//		printf("检测不到芯片，请检查芯片是否正确链接\r\n");
//	}
	return false;
}
/******************************获取Si44xx硬件信息***************************/
void si446x_part_info(void)
{
    Pro2Cmd[0] = SI446X_CMD_ID_PART_INFO;
    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_PART_INFO,Pro2Cmd,SI446X_CMD_REPLY_COUNT_PART_INFO,Pro2Cmd );
    Si446xCmd.PART_INFO.CHIPREV         = Pro2Cmd[0];
    Si446xCmd.PART_INFO.PART            = ((U16)Pro2Cmd[1] << 8) & 0xFF00;
    Si446xCmd.PART_INFO.PART           |= (U16)Pro2Cmd[2] & 0x00FF;
    Si446xCmd.PART_INFO.PBUILD          = Pro2Cmd[3];
    Si446xCmd.PART_INFO.ID              = ((U16)Pro2Cmd[4] << 8) & 0xFF00;
    Si446xCmd.PART_INFO.ID             |= (U16)Pro2Cmd[5] & 0x00FF;
    Si446xCmd.PART_INFO.CUSTOMER        = Pro2Cmd[6];
    Si446xCmd.PART_INFO.ROMID           = Pro2Cmd[7];
}
//************************开始发送*****************************/
void si446x_start_tx(uint8_t CHANNEL, uint8_t CONDITION, U16 TX_LEN)
{
    Pro2Cmd[0] = SI446X_CMD_ID_START_TX;
    Pro2Cmd[1] = CHANNEL;
    Pro2Cmd[2] = CONDITION;
    Pro2Cmd[3] = (uint8_t)(TX_LEN >> 8);
    Pro2Cmd[4] = (uint8_t)(TX_LEN);
    Pro2Cmd[5] = 0x00;

    // Don't repeat the packet, 
    // ie. transmit the packet only once
    Pro2Cmd[6] = 0x00;
    radio_comm_SendCmd(SI446X_CMD_ARG_COUNT_START_TX, Pro2Cmd);
}
//************************开始接收*****************************/
void si446x_start_rx(uint8_t CHANNEL, uint8_t CONDITION, U16 RX_LEN, uint8_t NEXT_STATE1, uint8_t NEXT_STATE2, uint8_t NEXT_STATE3)
{
    Pro2Cmd[0] = SI446X_CMD_ID_START_RX;
    Pro2Cmd[1] = CHANNEL;
    Pro2Cmd[2] = CONDITION;
    Pro2Cmd[3] = (uint8_t)(RX_LEN >> 8);
    Pro2Cmd[4] = (uint8_t)(RX_LEN);
    Pro2Cmd[5] = NEXT_STATE1;
    Pro2Cmd[6] = NEXT_STATE2;
    Pro2Cmd[7] = NEXT_STATE3;
    radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_RX, Pro2Cmd );
}
//***************************获取中断信息**************************/
void si446x_get_int_status(uint8_t PH_CLR_PEND, uint8_t MODEM_CLR_PEND, uint8_t CHIP_CLR_PEND)
{
    Pro2Cmd[0] = SI446X_CMD_ID_GET_INT_STATUS;
    Pro2Cmd[1] = PH_CLR_PEND;
    Pro2Cmd[2] = MODEM_CLR_PEND;
    Pro2Cmd[3] = CHIP_CLR_PEND;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_INT_STATUS,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GET_INT_STATUS,
                              Pro2Cmd);

    Si446xCmd.GET_INT_STATUS.INT_PEND       = Pro2Cmd[0];
    Si446xCmd.GET_INT_STATUS.INT_STATUS     = Pro2Cmd[1];
    Si446xCmd.GET_INT_STATUS.PH_PEND        = Pro2Cmd[2];
    Si446xCmd.GET_INT_STATUS.PH_STATUS      = Pro2Cmd[3];
    Si446xCmd.GET_INT_STATUS.MODEM_PEND     = Pro2Cmd[4];
    Si446xCmd.GET_INT_STATUS.MODEM_STATUS   = Pro2Cmd[5];
    Si446xCmd.GET_INT_STATUS.CHIP_PEND      = Pro2Cmd[6];
    Si446xCmd.GET_INT_STATUS.CHIP_STATUS    = Pro2Cmd[7];
}
//*******************************配置引脚****************************/
void si446x_gpio_pin_cfg(uint8_t GPIO0, uint8_t GPIO1, uint8_t GPIO2, uint8_t GPIO3, uint8_t NIRQ, uint8_t SDO, uint8_t GEN_CONFIG)
{
    Pro2Cmd[0] = SI446X_CMD_ID_GPIO_PIN_CFG;
    Pro2Cmd[1] = GPIO0;
    Pro2Cmd[2] = GPIO1;
    Pro2Cmd[3] = GPIO2;
    Pro2Cmd[4] = GPIO3;
    Pro2Cmd[5] = NIRQ;
    Pro2Cmd[6] = SDO;
    Pro2Cmd[7] = GEN_CONFIG;
    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GPIO_PIN_CFG,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GPIO_PIN_CFG,
                              Pro2Cmd );
    Si446xCmd.GPIO_PIN_CFG.GPIO[0]        = Pro2Cmd[0];
    Si446xCmd.GPIO_PIN_CFG.GPIO[1]        = Pro2Cmd[1];
    Si446xCmd.GPIO_PIN_CFG.GPIO[2]        = Pro2Cmd[2];
    Si446xCmd.GPIO_PIN_CFG.GPIO[3]        = Pro2Cmd[3];
    Si446xCmd.GPIO_PIN_CFG.NIRQ         = Pro2Cmd[4];
    Si446xCmd.GPIO_PIN_CFG.SDO          = Pro2Cmd[5];
    Si446xCmd.GPIO_PIN_CFG.GEN_CONFIG   = Pro2Cmd[6];
}
//***************************切换模块状态**************************/
void si446x_change_state(uint8_t NEXT_STATE1)
{
    Pro2Cmd[0] = SI446X_CMD_ID_CHANGE_STATE;
    Pro2Cmd[1] = NEXT_STATE1;
    radio_comm_SendCmd(SI446X_CMD_ARG_COUNT_CHANGE_STATE, Pro2Cmd);
}
/*********************************Si44xx清空FIFO***************************/
void si446x_fifo_info(uint8_t FIFO)
{
    Pro2Cmd[0] = SI446X_CMD_ID_FIFO_INFO;
    Pro2Cmd[1] = FIFO;
    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_FIFO_INFO,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_FIFO_INFO,
                              Pro2Cmd );
    Si446xCmd.FIFO_INFO.RX_FIFO_COUNT   = Pro2Cmd[0];
    Si446xCmd.FIFO_INFO.TX_FIFO_SPACE   = Pro2Cmd[1];
}
/*********************************Si44xx写TxFIFO*****************************/
void si446x_write_tx_fifo(uint8_t numBytes, uint8_t* pTxData)
{
  radio_comm_WriteData(SI446X_CMD_ID_WRITE_TX_FIFO, 0, numBytes, pTxData );
}
/*********************************Si44xx读RxFIFO*****************************/
void si446x_read_rx_fifo(uint8_t numBytes, uint8_t* pRxData)
{
  radio_comm_ReadData( SI446X_CMD_ID_READ_RX_FIFO, 0, numBytes, pRxData );
}
/****************************获取Si44xx寄存器组的值*************************/
void si446x_get_property(uint8_t GROUP, uint8_t NUM_PROPS, uint8_t START_PROP)
{
    Pro2Cmd[0] = SI446X_CMD_ID_GET_PROPERTY;
    Pro2Cmd[1] = GROUP;
    Pro2Cmd[2] = NUM_PROPS;
    Pro2Cmd[3] = START_PROP;
    radio_comm_SendCmdGetResp(SI446X_CMD_ARG_COUNT_GET_PROPERTY,Pro2Cmd,Pro2Cmd[2],Pro2Cmd );
	
    Si446xCmd.GET_PROPERTY.DATA[0 ]   = Pro2Cmd[0];
    Si446xCmd.GET_PROPERTY.DATA[1 ]   = Pro2Cmd[1];
    Si446xCmd.GET_PROPERTY.DATA[2 ]   = Pro2Cmd[2];
    Si446xCmd.GET_PROPERTY.DATA[3 ]   = Pro2Cmd[3];
    Si446xCmd.GET_PROPERTY.DATA[4 ]   = Pro2Cmd[4];
    Si446xCmd.GET_PROPERTY.DATA[5 ]   = Pro2Cmd[5];
    Si446xCmd.GET_PROPERTY.DATA[6 ]   = Pro2Cmd[6];
    Si446xCmd.GET_PROPERTY.DATA[7 ]   = Pro2Cmd[7];
    Si446xCmd.GET_PROPERTY.DATA[8 ]   = Pro2Cmd[8];
    Si446xCmd.GET_PROPERTY.DATA[9 ]   = Pro2Cmd[9];
    Si446xCmd.GET_PROPERTY.DATA[10]   = Pro2Cmd[10];
    Si446xCmd.GET_PROPERTY.DATA[11]   = Pro2Cmd[11];
    Si446xCmd.GET_PROPERTY.DATA[12]   = Pro2Cmd[12];
    Si446xCmd.GET_PROPERTY.DATA[13]   = Pro2Cmd[13];
    Si446xCmd.GET_PROPERTY.DATA[14]   = Pro2Cmd[14];
    Si446xCmd.GET_PROPERTY.DATA[15]   = Pro2Cmd[15];
}
/*************************获取Si44xx快速响应寄存器A*************************/
void si446x_frr_a_read(uint8_t respByteCount)
{
    radio_comm_ReadData(SI446X_CMD_ID_FRR_A_READ,0,respByteCount,Pro2Cmd);
    Si446xCmd.FRR_A_READ.FRR_A_VALUE = Pro2Cmd[0];
    Si446xCmd.FRR_A_READ.FRR_B_VALUE = Pro2Cmd[1];
    Si446xCmd.FRR_A_READ.FRR_C_VALUE = Pro2Cmd[2];
    Si446xCmd.FRR_A_READ.FRR_D_VALUE = Pro2Cmd[3];
}
/*************************获取Si44xx快速响应寄存器B*************************/
void si446x_frr_b_read(uint8_t respByteCount)
{
    radio_comm_ReadData(SI446X_CMD_ID_FRR_B_READ,0,respByteCount,Pro2Cmd);
    Si446xCmd.FRR_B_READ.FRR_B_VALUE = Pro2Cmd[0];
    Si446xCmd.FRR_B_READ.FRR_C_VALUE = Pro2Cmd[1];
    Si446xCmd.FRR_B_READ.FRR_D_VALUE = Pro2Cmd[2];
    Si446xCmd.FRR_B_READ.FRR_A_VALUE = Pro2Cmd[3];
}
/*************************获取Si44xx快速响应寄存器C*************************/
void si446x_frr_c_read(uint8_t respByteCount)
{
    radio_comm_ReadData(SI446X_CMD_ID_FRR_C_READ,0,respByteCount,Pro2Cmd);
    Si446xCmd.FRR_C_READ.FRR_C_VALUE = Pro2Cmd[0];
    Si446xCmd.FRR_C_READ.FRR_D_VALUE = Pro2Cmd[1];
    Si446xCmd.FRR_C_READ.FRR_A_VALUE = Pro2Cmd[2];
    Si446xCmd.FRR_C_READ.FRR_B_VALUE = Pro2Cmd[3];
}
/*************************获取Si44xx快速响应寄存器D*************************/
void si446x_frr_d_read(uint8_t respByteCount)
{
    radio_comm_ReadData(SI446X_CMD_ID_FRR_D_READ,0,respByteCount,Pro2Cmd);
    Si446xCmd.FRR_D_READ.FRR_D_VALUE = Pro2Cmd[0];
    Si446xCmd.FRR_D_READ.FRR_A_VALUE = Pro2Cmd[1];
    Si446xCmd.FRR_D_READ.FRR_B_VALUE = Pro2Cmd[2];
    Si446xCmd.FRR_D_READ.FRR_C_VALUE = Pro2Cmd[3];
}
/*!
 * Reads the ADC values from the radio into @Si446xCmd union.
 * @param ADC_EN  ADC enable parameter.
 */
void si446x_get_adc_reading(uint8_t ADC_EN)
{
    Pro2Cmd[0] = SI446X_CMD_ID_GET_ADC_READING;
    Pro2Cmd[1] = ADC_EN;
    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_ADC_READING,Pro2Cmd,SI446X_CMD_REPLY_COUNT_GET_ADC_READING,Pro2Cmd );

    Si446xCmd.GET_ADC_READING.GPIO_ADC         = ((U16)Pro2Cmd[0] << 8) & 0xFF00;
    Si446xCmd.GET_ADC_READING.GPIO_ADC        |=  (U16)Pro2Cmd[1] & 0x00FF;
    Si446xCmd.GET_ADC_READING.BATTERY_ADC      = ((U16)Pro2Cmd[2] << 8) & 0xFF00;
    Si446xCmd.GET_ADC_READING.BATTERY_ADC     |=  (U16)Pro2Cmd[3] & 0x00FF;
    Si446xCmd.GET_ADC_READING.TEMP_ADC         = ((U16)Pro2Cmd[4] << 8) & 0xFF00;
    Si446xCmd.GET_ADC_READING.TEMP_ADC        |=  (U16)Pro2Cmd[5] & 0x00FF;
}
/*!
 * Gets the Packet Handler status flags. Optionally clears them.
 * @param PH_CLR_PEND Flags to clear.
 */
void si446x_get_ph_status(uint8_t PH_CLR_PEND)
{
    Pro2Cmd[0] = SI446X_CMD_ID_GET_PH_STATUS;
    Pro2Cmd[1] = PH_CLR_PEND;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_PH_STATUS,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GET_PH_STATUS,
                              Pro2Cmd );

    Si446xCmd.GET_PH_STATUS.PH_PEND        = Pro2Cmd[0];
    Si446xCmd.GET_PH_STATUS.PH_STATUS      = Pro2Cmd[1];
}
/*!
 * Gets the Modem status flags. Optionally clears them.
 * @param MODEM_CLR_PEND Flags to clear.
 */
void si446x_get_modem_status( uint8_t MODEM_CLR_PEND )
{
    Pro2Cmd[0] = SI446X_CMD_ID_GET_MODEM_STATUS;
    Pro2Cmd[1] = MODEM_CLR_PEND;
	
    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_MODEM_STATUS,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GET_MODEM_STATUS,
                              Pro2Cmd );
	
    Si446xCmd.GET_MODEM_STATUS.MODEM_PEND   = Pro2Cmd[0];
    Si446xCmd.GET_MODEM_STATUS.MODEM_STATUS = Pro2Cmd[1];
    Si446xCmd.GET_MODEM_STATUS.CURR_RSSI    = Pro2Cmd[2];
    Si446xCmd.GET_MODEM_STATUS.LATCH_RSSI   = Pro2Cmd[3];
    Si446xCmd.GET_MODEM_STATUS.ANT1_RSSI    = Pro2Cmd[4];
    Si446xCmd.GET_MODEM_STATUS.ANT2_RSSI    = Pro2Cmd[5];
    Si446xCmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET =  ((U16)Pro2Cmd[6] << 8) & 0xFF00;
    Si446xCmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET |= (U16)Pro2Cmd[7] & 0x00FF;
}
/*!
 * Gets the Chip status flags. Optionally clears them.
 * @param CHIP_CLR_PEND Flags to clear.
 */
void si446x_get_chip_status( uint8_t CHIP_CLR_PEND )
{
    Pro2Cmd[0] = SI446X_CMD_ID_GET_CHIP_STATUS;
    Pro2Cmd[1] = CHIP_CLR_PEND;
	
    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_CHIP_STATUS,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GET_CHIP_STATUS,
                              Pro2Cmd );
	
    Si446xCmd.GET_CHIP_STATUS.CHIP_PEND         = Pro2Cmd[0];
    Si446xCmd.GET_CHIP_STATUS.CHIP_STATUS       = Pro2Cmd[1];
    Si446xCmd.GET_CHIP_STATUS.CMD_ERR_STATUS    = Pro2Cmd[2];
}

/**************************获取中断并处理************************/
uint8_t si446x_check_event(void)
{
	if (radio_irqLevel() == 0)
	{
	  /* Read ITs, clear pending ones */
	  si446x_get_int_status(0u, 0u, 0u);
	  if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CHIP_PEND_CMD_ERROR_PEND_BIT)
	  {
		/* State change to */
		si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_SLEEP);
		/* Reset FIFO */
		si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);
		/* State change to */
		si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_RX);
	  }
	  if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT)
	  {
		return SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT;
	  }
	  if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_RX_PEND_BIT)
	  {
		/* Packet RX */
		/* Get payload length */
		si446x_fifo_info(0x00);
		si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &Packet[0]);
		return SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_RX_PEND_BIT;
	  }
	  if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_STATUS_CRC_ERROR_BIT)
	  {
		/* Reset FIFO */
		si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);
	  }
	}
	return 0;
}
//************************开始接收*****************************/
void si446x_enter_rx(uint8_t channel, uint8_t packetLenght)
{
	// Read ITs, clear pending ones
	si446x_get_int_status(0u, 0u, 0u);
	// Reset the Rx Fifo
	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);
	/* Start Receiving packet, channel 0, START immediately, Packet length used or not according to packetLength */
	si446x_start_rx(channel, 0u, packetLenght,
				  SI446X_CMD_START_RX_ARG_NEXT_STATE1_RXTIMEOUT_STATE_ENUM_NOCHANGE,
				  SI446X_CMD_START_RX_ARG_NEXT_STATE2_RXVALID_STATE_ENUM_READY,
				  SI446X_CMD_START_RX_ARG_NEXT_STATE3_RXINVALID_STATE_ENUM_RX);
}
//************************发送一个包****************************/
void si446x_send_packet(uint8_t channel, uint8_t *pioRadioPacket, uint8_t length)
{
  /* Leave RX state */
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);

  /* Read ITs, clear pending ones */
  si446x_get_int_status(0u, 0u, 0u);

  /* Reset the Tx Fifo */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_TX_BIT);

  /* Fill the TX fifo with datas */
  si446x_write_tx_fifo(length, pioRadioPacket);

  /* Start sending packet, channel 0, START immediately */
  si446x_start_tx(channel, 0x80, length);
}
//************************获取Si44xxRssi****************************/
uint8_t si446x_get_rssi(void)
{
	si446x_frr_a_read(1);
	return Si446xCmd.FRR_A_READ.FRR_A_VALUE;
}

uint8_t si446x_set_power(uint8_t power)//0x7F  是最大功率输出
{
	//0x11, 0x22, 0x04, 0x00, 0x08, 0x7F, 0x00, 0x3D
	uint8_t	iData=5;
	uint8_t	Data[5]={0x11, 0x22, 0x01, 0x01, 0x7F};
	if(power<0x80)Data[4]=power;
	if(radio_comm_SendCmdGetResp(iData, Data, 0, 0)!=0xff)
	return false;
	return true;
}

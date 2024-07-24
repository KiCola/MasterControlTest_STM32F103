/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp_hal_can.h"
#include "can.h"
#include "string.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
typedef struct
{
	uint8_t front;
    uint8_t rear;
	CanRxMemberType data[CAN_QUEUE_NUM];
}CanRxQueueType;
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
CanRxQueueType CanRxQueueStruct;//��������
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/** @brief  �ж϶����Ƿ�����
  * @param  
  * @retval QUEUE_TRUE;  ����
            QUEUE_FALSE; δ��
  */
static uint8_t CAN_FullQueue(CanRxQueueType *pQ)
{
	if(((pQ->rear + 1)%CAN_QUEUE_NUM) == pQ->front  )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/** @brief  �ж϶����Ƿ�Ϊ��
  * @param  
  * @retval QUEUE_TRUE; ��
            QUEUE_FALSE;û��
  */
static uint8_t CAN_EmputQueue(CanRxQueueType *pQ)
{
	if ( pQ->front == pQ->rear )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/** @brief  ���
  * @param  val:���ֵ
  * @retval QUEUE_FALSE;  ʧ��
            QUEUE_TRUE;   �ɹ�
  */
static uint8_t CAN_EnterQueue(CanRxQueueType *pQ,CanRxMemberType val)
{
	if ( CAN_FullQueue(pQ) )
	{
		return FALSE;
	}
	else
	{
		pQ->data[pQ->rear] = val;
		pQ->rear = (pQ->rear+1) % CAN_QUEUE_NUM;

		return TRUE;
	}
}
/** @brief  ����
  * @param  *val:�������ݴ�ŵ�ַ
  * @retval QUEUE_FALSE;  ʧ��
            QUEUE_TRUE;   �ɹ�
  */
uint8_t CAN_OutQueue(CanRxMemberType * pVal)
{
	if ( CAN_EmputQueue(&CanRxQueueStruct) )
	{
		return FALSE;
	}
	else
	{
		*pVal = CanRxQueueStruct.data[CanRxQueueStruct.front];
		CanRxQueueStruct.front = (CanRxQueueStruct.front+1) % CAN_QUEUE_NUM;

		return TRUE;
	}
}
static void HAL_USER_CANFilterConfig(void)
{
    CAN_FilterTypeDef  sFilterConfig;
    
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
    
	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
	}
	
	/* Start the CAN peripheral */
	if (HAL_CAN_Start(&hcan) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}
	
	/* Activate CAN RX notification */
	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		/* Notification Error */
		Error_Handler();
	}

}
void Bsp_Cancom_ConfigurationInit(void)
{
	MX_CAN_Init();
}
void Bsp_Cancom_FilterInit(void)
{
    HAL_USER_CANFilterConfig();
}

uint32_t Bsp_Cancom_ReadERR(void)
{
	return hcan.ErrorCode;
}

///** @brief  ����һ֡����
//  * @param  
//  * @retval 1������ʧ��   0�����ͳɹ�
//  */
uint8_t CANSend(uint32_t Source,
	               uint32_t IDE,
                   uint32_t RTR,
                   uint8_t* Datum,
                   uint8_t DataLength)       
{
	uint8_t err;
	CAN_TxHeaderTypeDef   TxHeader;
	uint32_t              TxMailbox;
	
	if(DataLength > 8)
	{
		DataLength = 8;
	}
	TxHeader.ExtId = Source; 
	TxHeader.StdId = Source; 
	TxHeader.IDE = IDE; 
	TxHeader.RTR = RTR;    
	TxHeader.DLC = DataLength;
	
	err = HAL_CAN_AddTxMessage(&hcan, &TxHeader, Datum, &TxMailbox);
	
	return err;
}
__weak void QueueOverflow_Dispose(CanRxMemberType *pQ)
{
	UNUSED(pQ);
}
///** @brief  CAN�����жϻص�����
//  * @param  
//  * @retval 
//  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
	CAN_RxHeaderTypeDef   RxHeader;
	uint8_t               RxData[8];
	CanRxMemberType       val_buf;
	uint8_t i;
	/* Get RX message */
	if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
	{
		//����һ֡���ݽ������
	    val_buf.IDE = RxHeader.IDE;//��ʶ��
	    val_buf.RTR = RxHeader.RTR;//��Ϣ����
	    val_buf.DLC = RxHeader.DLC;//���ݳ���
		if(val_buf.IDE == CAN_ID_EXT) //ID
		{
			val_buf.Id = RxHeader.ExtId;//��չID
		}
		else
		{
			val_buf.Id = RxHeader.StdId;//��׼ID
		}
		for(i=0; i<8; i++) //����
		{
		   if(i<val_buf.DLC)
		   {
				val_buf.Data[i] = RxData[i];
		   }
		   else
		   {
			   val_buf.Data[i] = 0;
		   }
		}
		i = CAN_EnterQueue(&CanRxQueueStruct,val_buf);
		//�������
        if(FALSE == i)
        {
           //...
			QueueOverflow_Dispose(&val_buf);
        }
	}
}

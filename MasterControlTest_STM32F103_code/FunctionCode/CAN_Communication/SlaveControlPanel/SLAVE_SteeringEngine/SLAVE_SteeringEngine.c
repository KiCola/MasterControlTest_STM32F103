/* 包含头文件 ----------------------------------------------------------------*/
#include "SLAVE_SteeringEngine/SLAVE_SteeringEngine.h"
#include "string.h"
#include "CAN_Communication.h"
/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
static SteeringEngineDef_t* head_handle = NULL;
/* 扩展变量 ------------------------------------------------------------------*/

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/
static uint16_t SLAVE_SteeringEngine_ProtocolDefinitions(SteeringEngineDef_t* handle,
                                                   uint16_t cannum,
                                                   PgnCanTxDataType *head)
{
    uint16_t cannum_buff;
    cannum_buff = cannum;
    
    handle->CAN_Number = cannum_buff; //第一个帧的CAN编号，后面以次累加计算
    
    head[cannum_buff++] = xCreate( DEVICE_ID,DEVICE_LIST,
								   SteeringEnginePlate_Addr,
								   handle->SteeringEngine_SerialNumber.DATE.Serial_Number,
								   1,1,
                                   FALSE,PG_TX_FREE,MODE_REQ,TT_ASYNC,10,0,  
                                   handle->SteeringEngine_SerialNumber.Datum,PG_RXSTEERINGENGINE_SERIAL_NUMBER_LEN,
								   handle->SteeringEngine_SerialNumber.Datum,PG_RXSTEERINGENGINE_SERIAL_NUMBER_LEN); //序号修改    
                                  
    
    head[cannum_buff++] = xCreate( ALL_Addr,0,
								   SteeringEnginePlate_Addr,
								   handle->SteeringEngine_SerialNumber.DATE.Serial_Number,
								   0,0,   
                                   FALSE,PG_TX_FREE,MODE_BRO,TT_CYCLIC,10,0,    
                                   0,0, 
	                               handle->SteeringEngine_UploadData.Datum,PG_TXSTEERINGENGINEDATA_LEN); //数据反馈     
    
    head[cannum_buff++] = xCreate( DEVICE_ID,DEVICE_LIST,
	                               SteeringEnginePlate_Addr,
								   handle->SteeringEngine_SerialNumber.DATE.Serial_Number,
								   0,0,   
                                   TRUE,PG_TX_FREE,MODE_REQ,TT_ASYNC,10,0,    
                                   handle->SteeringEngine_ControlData.Datum,PG_RXSTEERINGENGINEDATA_LEN,
								   handle->SteeringEngine_ControlData.Datum,PG_RXSTEERINGENGINEDATA_LEN); //控制数据帧初始化     
                                  
    
    return cannum_buff;
}

uint16_t SLAVE_SteeringEngine_CANinit(uint16_t cannum,void *head)
{
    SteeringEngineDef_t* target;
    uint16_t cannum_buff;
    cannum_buff = cannum;
    
	for(target=head_handle; target; target=target->next) 
    {
        cannum_buff = SLAVE_SteeringEngine_ProtocolDefinitions(target,cannum_buff,((PgnCanTxDataType*)head));
	}
   
    return cannum_buff;
}

uint8_t SLAVE_SteeringEngine_Init(SteeringEngineDef_t* handle,uint8_t list)
{
	SteeringEngineDef_t* target = head_handle;
    
    memset(handle, 0, sizeof(SteeringEngineDef_t));
    
    handle->SteeringEngine_UploadData.DATE.SpeedFac = 20;
    handle->SteeringEngine_SerialNumber.DATE.Serial_Number = list;
    
	while(target) 
    {
		if(target == handle) 
            return 0;	//already exist.
		target = target->next;
	}
	handle->next = head_handle;
	head_handle = handle;
    
	return 1;
}
void SLAVE_SteeringEngine_NumberSet(SteeringEngineDef_t* handle,uint8_t number)
{
    if(number > 15)
    {
        number = 15;
    }
    
    handle->SteeringEngine_SerialNumber.DATE.Serial_Number = number;
    
    CAN_TT_ASYNC_Enable(handle->CAN_Number);
}
void SLAVE_SteeringEngine_MotorControl(SteeringEngineDef_t* handle,uint8_t motor,uint8_t mode,int8_t val) 
{
    if(mode > SteeringEngine_MODE_Speed)
    {
        mode = SteeringEngine_MODE_Speed;
    }
    if(mode == SteeringEngine_MODE_Angle)
    {
        if(val > 90)
        {
            val = 90;
        }
        else if(val < -90)
        {
            val = -90;
        }
    }
    else 
    {
        if(val > handle->SteeringEngine_UploadData.DATE.SpeedFac)
        {
            val = handle->SteeringEngine_UploadData.DATE.SpeedFac;
        }
        else if(val < -handle->SteeringEngine_UploadData.DATE.SpeedFac)
        {
            val = -handle->SteeringEngine_UploadData.DATE.SpeedFac;
        }
    }
    switch(motor)
    {
        case SteeringEngine_MOTOR_1:
            handle->SteeringEngine_ControlData.DATE.mode1 = mode;
            handle->SteeringEngine_ControlData.DATE.SpeedAndAngle1 = val;
            break;
        case SteeringEngine_MOTOR_2:
            handle->SteeringEngine_ControlData.DATE.mode2 = mode;
            handle->SteeringEngine_ControlData.DATE.SpeedAndAngle2 = val;
            break;
        case SteeringEngine_MOTOR_3:
            handle->SteeringEngine_ControlData.DATE.mode3 = mode;
            handle->SteeringEngine_ControlData.DATE.SpeedAndAngle3 = val;
            break;
        default:
            break;
    }
    if(motor <= SteeringEngine_MOTOR_3)
    {
        handle->SteeringEngine_ControlData.DATE.MemoryFlg = 0;
        CAN_TT_ASYNC_Enable(handle->CAN_Number+2);
    }
}

void SLAVE_SteeringEngine_MoreMotorControl(SteeringEngineDef_t* handle,
                                           uint8_t mode1,int8_t val1,
                                           uint8_t mode2,int8_t val2,
                                           uint8_t mode3,int8_t val3
                                          )
{
    if(mode1 > SteeringEngine_MODE_Speed)
    {
        mode1 = SteeringEngine_MODE_Speed;
    }
    if(mode1 == SteeringEngine_MODE_Angle)
    {
        if(val1 > 90)
        {
            val1 = 90;
        }
        else if(val1 < -90)
        {
            val1 = -90;
        }
    }
    else 
    {
        if(val1 > handle->SteeringEngine_UploadData.DATE.SpeedFac)
        {
            val1 = handle->SteeringEngine_UploadData.DATE.SpeedFac;
        }
        else if(val1 < -handle->SteeringEngine_UploadData.DATE.SpeedFac)
        {
            val1 = -handle->SteeringEngine_UploadData.DATE.SpeedFac;
        }
    }
    handle->SteeringEngine_ControlData.DATE.mode1 = mode1;
    handle->SteeringEngine_ControlData.DATE.SpeedAndAngle1 = val1;
    
    if(mode2 > SteeringEngine_MODE_Speed)
    {
        mode2 = SteeringEngine_MODE_Speed;
    }
    if(mode2 == SteeringEngine_MODE_Angle)
    {
        if(val2 > 90)
        {
            val2 = 90;
        }
        else if(val2 < -90)
        {
            val2 = -90;
        }
    }
    else 
    {
        if(val2 > handle->SteeringEngine_UploadData.DATE.SpeedFac)
        {
            val2 = handle->SteeringEngine_UploadData.DATE.SpeedFac;
        }
        else if(val2 < -handle->SteeringEngine_UploadData.DATE.SpeedFac)
        {
            val2 = -handle->SteeringEngine_UploadData.DATE.SpeedFac;
        }
    }
    handle->SteeringEngine_ControlData.DATE.mode2 = mode2;
    handle->SteeringEngine_ControlData.DATE.SpeedAndAngle2 = val2;
    
    if(mode3 > SteeringEngine_MODE_Speed)
    {
        mode3 = SteeringEngine_MODE_Speed;
    }
    if(mode3 == SteeringEngine_MODE_Angle)
    {
        if(val3 > 90)
        {
            val3 = 90;
        }
        else if(val3 < -90)
        {
            val3 = -90;
        }
    }
    else 
    {
        if(val3 > handle->SteeringEngine_UploadData.DATE.SpeedFac)
        {
            val3 = handle->SteeringEngine_UploadData.DATE.SpeedFac;
        }
        else if(val3 < -handle->SteeringEngine_UploadData.DATE.SpeedFac)
        {
            val3 = -handle->SteeringEngine_UploadData.DATE.SpeedFac;
        }
    }
    handle->SteeringEngine_ControlData.DATE.mode3 = mode3;
    handle->SteeringEngine_ControlData.DATE.SpeedAndAngle3 = val3;
    
    handle->SteeringEngine_ControlData.DATE.MemoryFlg = 0;
    CAN_TT_ASYNC_Enable(handle->CAN_Number+2);
}

void SLAVE_SteeringEngine_MovementMemory(SteeringEngineDef_t* handle)
{
    handle->SteeringEngine_ControlData.DATE.MemoryFlg = 1;
    CAN_TT_ASYNC_Enable(handle->CAN_Number+2);
}





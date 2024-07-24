/* 包含头文件 ----------------------------------------------------------------*/
#include "SLAVE_BLHDCMotorMiniwatt/SLAVE_BLHDCMotorMiniwatt.h"
#include "string.h"
#include "CAN_Communication.h"
/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
static BLHDCMotorMiniwattDef_t* head_handle = NULL;
/* 扩展变量 ------------------------------------------------------------------*/

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/

static uint16_t SLAVE_BLHDCMotorMiniwatt_ProtocolDefinitions(BLHDCMotorMiniwattDef_t* handle,
                                       uint16_t cannum,
                                       PgnCanTxDataType *head)
{
    uint16_t cannum_buff;
    cannum_buff = cannum;
    
    handle->CAN_Number = cannum_buff; //第一个帧的CAN编号，后面以次累加计算
    
    head[cannum_buff++] = xCreate(  DEVICE_ID,DEVICE_LIST,
	                                BLHDCMotorPlate_Addr,handle->Motor_SerialNumber.DATE.Serial_Number,
	                                1,1,   
                                    TRUE,PG_TX_FREE,MODE_REQ,TT_ASYNC,50,0,  
                                    handle->Motor_SerialNumber.Datum,PG_RXBLHMOTOR_SERIAL_NUMBER_LEN,
	                                handle->Motor_SerialNumber.Datum,PG_RXBLHMOTOR_SERIAL_NUMBER_LEN); //序号修改      
                                  
    
    head[cannum_buff++] = xCreate(  ALL_Addr,0,
	                                BLHDCMotorPlate_Addr,handle->Motor_SerialNumber.DATE.Serial_Number,
	                                0,0,   
                                    FALSE,PG_TX_FREE,MODE_BRO,TT_CYCLIC,10,0,   
                                    0,0,
	                                handle->Motor_UploadData.Datum,PG_TXBLHMOTORDATA_LEN); //反馈数据帧初始化     
                                  
    
    head[cannum_buff++] = xCreate(  DEVICE_ID,DEVICE_LIST,
	                                BLHDCMotorPlate_Addr,handle->Motor_SerialNumber.DATE.Serial_Number,
									0,0,   
                                    TRUE,PG_TX_REQ,MODE_BRO,TT_CYCLIC,10,0,   
                                    handle->Motor_ControlData.Datum,PG_RXBLHMOTORDATA_LEN,
									0,0); //控制数据帧初始化
                                  
    
    return cannum_buff;
}

uint16_t SLAVE_BLHDCMotorMiniwatt_CANinit(uint16_t cannum,void *head)
{
    BLHDCMotorMiniwattDef_t* target;
    uint16_t cannum_buff;
    cannum_buff = cannum;
    
	for(target=head_handle; target; target=target->next) 
    {
        cannum_buff = SLAVE_BLHDCMotorMiniwatt_ProtocolDefinitions(target,cannum_buff,((PgnCanTxDataType*)head));
	}
    
    return cannum_buff;
}

uint8_t SLAVE_BLHDCMotorMiniwatt_Init(BLHDCMotorMiniwattDef_t* handle,uint8_t list)
{
	BLHDCMotorMiniwattDef_t* target = head_handle;
    
    memset(handle, 0, sizeof(BLHDCMotorMiniwattDef_t));
    
    handle->Motor_SerialNumber.DATE.Serial_Number = list;
    
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

void SLAVE_BLHDCMotorMiniwatt_NumberSet(BLHDCMotorMiniwattDef_t* handle,uint8_t number)
{
    if(number > 15)
    {
        number = 15;
    }
    
    handle->Motor_SerialNumber.DATE.Serial_Number = number;
    
    CAN_TT_ASYNC_Enable(handle->CAN_Number);
}

uint16_t SLAVE_BLHDCMotorMiniwatt_PlaceRead(BLHDCMotorMiniwattDef_t* handle)
{
    return handle->Motor_UploadData.DATE.Coded_Value;
}

int16_t SLAVE_BLHDCMotorMiniwatt_SpeedRead(BLHDCMotorMiniwattDef_t* handle)
{
    int16_t val;
	
    val = handle->Motor_UploadData.DATE.Speed;
    
    return val;
}

void SLAVE_BLHDCMotorMiniwatt_SpeedSet(BLHDCMotorMiniwattDef_t* handle,int16_t val)
{
        handle->Motor_ControlData.DATE.PwmSet = 0;
        handle->Motor_ControlData.DATE.SetSpeed = val;
}
    
void SLAVE_BLHDCMotorMiniwatt_PWMSet(BLHDCMotorMiniwattDef_t* handle,int16_t val)
{
        handle->Motor_ControlData.DATE.PwmSet = val;
        handle->Motor_ControlData.DATE.SetSpeed = 0;
}










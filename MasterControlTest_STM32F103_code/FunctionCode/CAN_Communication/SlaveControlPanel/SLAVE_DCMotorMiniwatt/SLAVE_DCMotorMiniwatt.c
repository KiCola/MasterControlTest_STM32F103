/* 包含头文件 ----------------------------------------------------------------*/
#include "SLAVE_DCMotorMiniwatt/SLAVE_DCMotorMiniwatt.h"
#include "string.h"
#include "CAN_Communication.h"
/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
static DCMotorMiniwattDef_t* head_handle = NULL;
/* 扩展变量 ------------------------------------------------------------------*/

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/

static uint16_t SLAVE_DCMotorMiniwatt_ProtocolDefinitions(DCMotorMiniwattDef_t* handle,
                                       uint16_t cannum,
                                       PgnCanTxDataType *head)
{
    uint16_t cannum_buff;
    cannum_buff = cannum;
    
    handle->CAN_Number = cannum_buff; //第一个帧的CAN编号，后面以次累加计算
    
    head[cannum_buff++] = xCreate(  DEVICE_ID,DEVICE_LIST,
									MotorPlate_Addr,
									handle->Motor_SerialNumber.DATE.Serial_Number,
									2,2,   
                                    FALSE,PG_TX_FREE,MODE_REQ,TT_ASYNC,50,0, 
                                    handle->Motor_SerialNumber.Datum,PG_RXMOTOR_SERIAL_NUMBER_LEN,
									handle->Motor_SerialNumber.Datum,PG_RXMOTOR_SERIAL_NUMBER_LEN); //序号修改      
                                  
    
    head[cannum_buff++] = xCreate(  ALL_Addr,0,
									MotorPlate_Addr,
									handle->Motor_SerialNumber.DATE.Serial_Number,
									0,0,   
                                    FALSE,PG_TX_FREE,MODE_BRO,TT_CYCLIC,10,0,   
                                    0,0,
									handle->Motor_UploadData.Datum,PG_TXMOTORDATA_LEN); //反馈数据帧初始化     
                                  
    
    head[cannum_buff++] = xCreate(  DEVICE_ID,DEVICE_LIST,
									MotorPlate_Addr,
									handle->Motor_SerialNumber.DATE.Serial_Number,
									0,0,   
                                    TRUE,PG_TX_REQ,MODE_BRO,TT_CYCLIC,10,0,   
                                    handle->Motor_ControlData.Datum,PG_RXMOTORDATA_LEN,
									0,0); //控制数据帧初始化     
                                  
    
    return cannum_buff;
}

uint16_t SLAVE_DCMotorMiniwatt_CANinit(uint16_t cannum,void *head)
{
    DCMotorMiniwattDef_t* target;
    uint16_t cannum_buff;
    cannum_buff = cannum;
    
	for(target=head_handle; target; target=target->next) 
    {
        cannum_buff = SLAVE_DCMotorMiniwatt_ProtocolDefinitions(target,cannum_buff,((PgnCanTxDataType*)head));
	}
    
    return cannum_buff;
}

uint8_t SLAVE_DCMotorMiniwatt_Init(DCMotorMiniwattDef_t* handle,uint8_t list)
{
	DCMotorMiniwattDef_t* target = head_handle;
    
    memset(handle, 0, sizeof(DCMotorMiniwattDef_t));
    
    handle->Motor_SerialNumber.DATE.Serial_Number = list;
    //电机速度： 0.5 rpm/位递增，从-16000 rpm开始计算，范围-16000 rpm到16000 rpm),0rpm计算得出32000;
    handle->Motor_ControlData.DATE.SetSpeed = 32000;
    handle->Motor_ControlData.DATE.PwmSet = 0;
    handle->Motor_UploadData.DATE.Speed = 32000;
    
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

void SLAVE_DCMotorMiniwatt_NumberSet(DCMotorMiniwattDef_t* handle,uint8_t number)
{
    if(number > 15)
    {
        number = 15;
    }
    
    handle->Motor_SerialNumber.DATE.Serial_Number = number;
    
    CAN_TT_ASYNC_Enable(handle->CAN_Number);
}

uint16_t SLAVE_DCMotorMiniwatt_PlaceRead(DCMotorMiniwattDef_t* handle)
{
    return handle->Motor_UploadData.DATE.Coded_Value;
}

int16_t SLAVE_DCMotorMiniwatt_SpeedRead(DCMotorMiniwattDef_t* handle)
{
    int16_t val;
    val = (handle->Motor_UploadData.DATE.Speed/2)-16000;
    
    return val;
}

void SLAVE_DCMotorMiniwatt_SpeedSet(DCMotorMiniwattDef_t* handle,int16_t val)
{
        handle->Motor_ControlData.DATE.PwmSet = 0;
        handle->Motor_ControlData.DATE.SetSpeed = (val+16000)*2;
}
    
void SLAVE_DCMotorMiniwatt_PWMSet(DCMotorMiniwattDef_t* handle,int16_t val)
{
        handle->Motor_ControlData.DATE.PwmSet = val;
        handle->Motor_ControlData.DATE.SetSpeed = 32000;
}










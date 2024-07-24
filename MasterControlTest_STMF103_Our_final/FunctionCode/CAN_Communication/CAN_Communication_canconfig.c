/* 包含头文件 ----------------------------------------------------------------*/
#include "CAN_Communication_canconfig.h"

/* USER自定义 ----------------------------------------------------------------*/

/* 扩展变量 ------------------------------------------------------------------*/
PgnCanTxDataType TxPGArray[NR_OF_TX_PG]; //协议帧缓存数组
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体声明 ----------------------------------------------------------------*/
#ifdef SLAVE_DCMotorMiniwatt_ENABLED//小功率直流电机
    extern uint16_t SLAVE_DCMotorMiniwatt_CANinit(uint16_t cannum,void *head);
#endif 

#ifdef SLAVE_Tracking_ENABLED       //循迹板
    extern uint16_t SLAVE_Tracking_CANinit(uint16_t cannum,void *head);
#endif

#ifdef SLAVE_SteeringEngine_ENABLED //舵机控制板
    extern uint16_t SLAVE_SteeringEngine_CANinit(uint16_t cannum,void *head);
#endif

#ifdef SLAVE_ExpansionBoardInput_ENABLED //扩展板（输入）
    extern uint16_t SLAVE_ExpansionBoardInput_CANinit(uint16_t cannum,void *head);
    extern void SLAVE_SteeringEngine_CANRXDispose(void *head,uint32_t Id);
#endif

#ifdef SLAVE_SteppingMotor_ENABLED  //步进电机驱动板
    extern uint16_t SLAVE_SteppingMotorg_CANinit(uint16_t cannum,void *head);
#endif

#ifdef SLAVE_ExpansionBoardOut_ENABLED  //扩展板（输出）
    extern uint16_t SLAVE_ExpansionBoardOut_CANinit(uint16_t cannum,void *head);
#endif
#ifdef SLAVE_SteeringEngine6CH_ENABLED //6通道舵机控制板
    extern uint16_t SLAVE_SteeringEngine6CH_CANinit(uint16_t cannum,void *head);
#endif
#ifdef SLAVE_BLHDCMotorMiniwatt_ENABLED  //无刷电机
	extern uint16_t SLAVE_BLHDCMotorMiniwatt_CANinit(uint16_t cannum,void *head);
#endif

#ifdef SLAVE_BLDCMotorMiniwatt_ENABLED  //无刷电机
	extern uint16_t SLAVE_BLDCMotorMiniwatt_CANinit(uint16_t cannum,void *head);
#endif
/* 函数体 --------------------------------------------------------------------*/


void InitialUsedPGs(void)//这里的定义USER自编写(目前包模式还未写，不可用)
{
    uint16_t cannum;
    cannum = 0;
	memset(TxPGArray, 0, sizeof(PgnCanTxDataType)*NR_OF_TX_PG);
#ifdef SLAVE_DCMotorMiniwatt_ENABLED    //小功率直流电机
    cannum = SLAVE_DCMotorMiniwatt_CANinit(cannum,TxPGArray);
#endif 

#ifdef SLAVE_Tracking_ENABLED           //循迹板
    cannum = SLAVE_Tracking_CANinit(cannum,TxPGArray);
#endif  

#ifdef SLAVE_SteeringEngine_ENABLED     //舵机控制板
     cannum = SLAVE_SteeringEngine_CANinit(cannum,TxPGArray);
#endif
   
#ifdef SLAVE_ExpansionBoardInput_ENABLED//扩展板
     cannum = SLAVE_ExpansionBoardInput_CANinit(cannum,TxPGArray);     
#endif 

#ifdef SLAVE_SteppingMotor_ENABLED      //步进电机驱动板
    cannum = SLAVE_SteppingMotorg_CANinit(cannum,TxPGArray);
#endif    

#ifdef SLAVE_ExpansionBoardOut_ENABLED  //扩展板（输出）
    cannum = SLAVE_ExpansionBoardOut_CANinit(cannum,TxPGArray);
#endif

#ifdef SLAVE_SteeringEngine6CH_ENABLED     //舵机控制板
     cannum = SLAVE_SteeringEngine6CH_CANinit(cannum,TxPGArray);
#endif

#ifdef SLAVE_BLHDCMotorMiniwatt_ENABLED  //直流有感无刷电机
	cannum = SLAVE_BLHDCMotorMiniwatt_CANinit(cannum,TxPGArray);
#endif

#ifdef SLAVE_BLDCMotorMiniwatt_ENABLED  //直流无感无刷电机
	cannum = SLAVE_BLDCMotorMiniwatt_CANinit(cannum,TxPGArray);
#endif

    if(cannum > NR_OF_TX_PG)  
    {
        while(1);//TxPGArray的内存申请不够
    }        
}

void GetReferenceFromDevice(void)//装置数据转换到协议中
{
    
}

void GetReferenceToDevice(uint32_t id)//协议数据解析到装置
{
#ifdef SLAVE_ExpansionBoardInput_ENABLED     //扩展板
    SLAVE_ExpansionBoardInput_CANRXDispose(TxPGArray,id);
#endif
}






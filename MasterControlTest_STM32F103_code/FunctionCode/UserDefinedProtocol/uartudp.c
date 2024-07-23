/* 包含头文件 ----------------------------------------------------------------*/
#include "uartudp.h"
#include "uartudp_config.h"
/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/
#define RECEIVE_OK   0x8000
#define STR_SHIFTING_ADDR_ID 6
#define STR_SHIFTING_ADDR_LRC 2
#define STR_ADDR_ORDER  4
/* 私有变量 ------------------------------------------------------------------*/
static uint8_t sys_cmd_tab[7][17]=
{
	{':','(',')','[',']'},
	{'\r','\n',},
	"help",
	"get_",
	"set_",
    " OrderErr ",
    "sequence",
};
uint8_t  uartudp_sequence[2];


uint8_t OrderSerialNumber = 0xff;//liu: sequence功能
uint8_t  sequence_flg = 0;//liu: sequence功能
uint8_t  modification_sequence_buff[20] = {":1102sequence[u8]OK"};
/* 扩展变量 ------------------------------------------------------------------*/

/* 私有函数原形 --------------------------------------------------------------*/
#if (UARTUDP_MODE_SLAVE == 1)
	static uint8_t uartudp_SLAVEScan(_s_uartudp_dev *handle);
#else 
    static void uartudp_HOSTScan(_s_uartudp_dev *handle);
#endif
/* 函数体 --------------------------------------------------------------------*/
/** @brief  uartudp initialize
  * @param  
  * @retval 
  */
void uartudp_init(void) //数据初始化
{
	uartudp_sequence[0] = uartudp_dev.id[2];
	uartudp_sequence[1] = uartudp_dev.id[3];
	memset(UARTUDP_TX_BUF, 0, sizeof(uint8_t)*UARTUDP_TX_LEN);
	memset(UARTUDP_RX_BUF, 0, sizeof(uint8_t)*UARTUDP_RX_LEN);
	memset(uartudp_dev.para_rx, 0, sizeof(struct _s_uartudp_nametab)*NR_OF_SLAVE_NAMETABE);   //数据初始化
	memset(uartudp_dev.para_tx, 0, sizeof(struct _s_uartudp_nametab_tx)*NR_OF_HOST_NAMETABE); //数据初始化
	UARTUDP_TX_STA = 0;
	UARTUDP_RX_STA = 0;
#if (UARTUDP_MODE_SLAVE == 1)
    uartudp_dev.mode_ = UARTUDP_SLAVE;
	uartudp_dev.CreateSlaveCommand(UARTUDP_IO,
	                               UARTUDP_ARRAY,
	                               UARTUDP_U8,
	                               2,
	                               (void*)uartudp_sequence,
								   sys_cmd_tab[6],0);
#endif
	UartudpInitialUsedPGs();
}
/** @brief  uartudp one millisecond a scan
  * @param  
  * @retval 
  */
void uartudp_scan(void)   //数据分析
{
	switch(uartudp_dev.mode_)
	{
	#if (UARTUDP_MODE_SLAVE == 1)
		case UARTUDP_SLAVE:    //从模式
			uartudp_SLAVEScan(&uartudp_dev);
			break;
	#else
		case UARTUDP_HOST:     //主模式
			uartudp_HOSTScan(&uartudp_dev);
			break;
	#endif
		default:break;
	}
    uartudp_dev.Timer++;
}
/** @brief  uartudp receive
  * @param  
  * @retval 
  */
void uartudp_receive(uint8_t *str,uint16_t len) //数据读取
{
    uint16_t i,j;
    if(len > 3)
    {
        j = 0;
        for(i=0; i<len; i++)
        {
            if(str[i] == sys_cmd_tab[0][0])
            {
                UARTUDP_RX_BUF[j++] = str[i++]; 
                break;
            }
        }
        if(i<len)
        {
            for(; i<len; i++)
            {
                UARTUDP_RX_BUF[j++] = str[i]; 
                if(str[i] == sys_cmd_tab[1][1])
                {
                    break;
                }
            }
        }
        
        UARTUDP_RX_STA = j;
        UARTUDP_RX_STA |= RECEIVE_OK;
    }
}
/** @brief  uartudp transfer
  * @param  
  * @retval 1：发送成功 0：忙取消发送
  */
uint8_t uartudp_transfer(uint8_t *str,uint16_t *len) //数据发送
{
    uint16_t i;
    if(UARTUDP_TX_STA&RECEIVE_OK)
    {
        *len = UARTUDP_TX_STA&(~RECEIVE_OK);
        for(i=0; i<*len; i++)
        {
            str[i] = UARTUDP_TX_BUF[i]; 
        }
        UARTUDP_TX_STA = 0;
        
        return 1;
    }
    return 0;
}
uint8_t uartudp_CreateSlaveCommand(  //声明一个从机命令
									_e_IOstate   IOstate,
									_e_class     Class,    
									_e_type      Parmtype,
									uint8_t plen,
									void* func,
									uint8_t* name,
									void (*ComSucceedCallBack)(struct _s_uartudp_nametab *pQ,_e_order Order)
									)
{
	uint8_t err;
	uint8_t i;
	
	for(i=0; i<uartudp_dev.pnum_rx; i++)
	{
		if(uartudp_strcmp(uartudp_dev.para_rx[i].name,name))
		{
			return 1;
		}
	}
	if(uartudp_dev.pnum_rx < NR_OF_SLAVE_NAMETABE)
	{
		err = 0;
		if(IOstate == UARTUDP_O) IOstate = UARTUDP_IO;
		uartudp_dev.para_rx[uartudp_dev.pnum_rx].func = func;
		uartudp_dev.para_rx[uartudp_dev.pnum_rx].name = name;
		uartudp_dev.para_rx[uartudp_dev.pnum_rx].plen = plen;
		uartudp_dev.para_rx[uartudp_dev.pnum_rx].IOstate = IOstate;
		uartudp_dev.para_rx[uartudp_dev.pnum_rx].Class = Class;
		uartudp_dev.para_rx[uartudp_dev.pnum_rx].Parmtype = Parmtype;
		if((UARTUDP_U8 == uartudp_dev.para_rx[uartudp_dev.pnum_rx].Parmtype)||(UARTUDP_S8 == uartudp_dev.para_rx[uartudp_dev.pnum_rx].Parmtype)){
			uartudp_dev.para_rx[uartudp_dev.pnum_rx].typelen = sizeof(uint8_t);
		}
		else if((UARTUDP_U16 == uartudp_dev.para_rx[uartudp_dev.pnum_rx].Parmtype)||(UARTUDP_S16 == uartudp_dev.para_rx[uartudp_dev.pnum_rx].Parmtype)){
			uartudp_dev.para_rx[uartudp_dev.pnum_rx].typelen = sizeof(uint16_t);
		}
		else if((UARTUDP_U32 == uartudp_dev.para_rx[uartudp_dev.pnum_rx].Parmtype)||(UARTUDP_S32 == uartudp_dev.para_rx[uartudp_dev.pnum_rx].Parmtype)){
			uartudp_dev.para_rx[uartudp_dev.pnum_rx].typelen = sizeof(uint32_t);
		}
		else if(UARTUDP_F == uartudp_dev.para_rx[uartudp_dev.pnum_rx].Parmtype){
			uartudp_dev.para_rx[uartudp_dev.pnum_rx].typelen = sizeof(float);
		}
		uartudp_dev.para_rx[uartudp_dev.pnum_rx].namelen = uartudp_len(uartudp_dev.para_rx[uartudp_dev.pnum_rx].name);
		uartudp_dev.para_rx[uartudp_dev.pnum_rx].ComSucceedCallBack = ComSucceedCallBack;
		uartudp_dev.pnum_rx++;
	}
	else
	{
		err =1;
	}
	
	return err;
}
uint8_t uartudp_CreateSendCommand(	//声明一个主机命令
									_e_IOstate   IOstate,
									_e_class     Class,    
									_e_type      Parmtype,
									uint8_t  plen,
									uint8_t* id,
									void*    func,
									uint8_t* name,
									_e_TransType TransType,
									_e_PGState   PGState,
									uint16_t TransRate,
									uint16_t Timer,
									void (*ComSucceedCallBack)(struct _s_uartudp_nametab_tx *pQ),
									void (*OvertimeCallBack)(struct _s_uartudp_nametab_tx *pQ)
                                 )
{
    uint8_t err;
	uint8_t i;

	for(i=0; i<uartudp_dev.pnum_tx; i++)
	{
		if((uartudp_strcmp(uartudp_dev.para_tx[i].name,name))&&(uartudp_dev.para_tx[i].IOstate == IOstate))
		{
			if(uartudp_strcmp(uartudp_dev.para_tx[i].id,id))
			{
				break;
			}
		}
	}
	if((IOstate == UARTUDP_IO)
	 ||(uartudp_dev.pnum_tx >= NR_OF_HOST_NAMETABE)
	 ||(i<uartudp_dev.pnum_tx))
	{
		err = 1;
	}
	else 
    {
        err = 0;
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].id[0]   = id[0];
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].id[1]   = id[1];
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].id[2]   = id[2];
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].id[3]   = id[3];
        uartudp_dev.para_tx[uartudp_dev.pnum_tx].func = func;
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].name = name;
        uartudp_dev.para_tx[uartudp_dev.pnum_tx].plen = plen;
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].IOstate = IOstate;
        uartudp_dev.para_tx[uartudp_dev.pnum_tx].Class = Class;
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].Parmtype = Parmtype;
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].TransType = TransType;
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].PGState = PGState;
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].TransRate = TransRate;
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].Timer = Timer;
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].txerrnum = 0;
		if((UARTUDP_U8 == uartudp_dev.para_tx[uartudp_dev.pnum_tx].Parmtype)||(UARTUDP_S8 == uartudp_dev.para_tx[uartudp_dev.pnum_tx].Parmtype)){
			uartudp_dev.para_tx[uartudp_dev.pnum_tx].typelen = sizeof(uint8_t);
		}
		else if((UARTUDP_U16 == uartudp_dev.para_tx[uartudp_dev.pnum_tx].Parmtype)||(UARTUDP_S16 == uartudp_dev.para_tx[uartudp_dev.pnum_tx].Parmtype)){
			uartudp_dev.para_tx[uartudp_dev.pnum_tx].typelen = sizeof(uint16_t);
		}
		else if((UARTUDP_U32 == uartudp_dev.para_tx[uartudp_dev.pnum_tx].Parmtype)||(UARTUDP_S32 == uartudp_dev.para_tx[uartudp_dev.pnum_tx].Parmtype)){
			uartudp_dev.para_tx[uartudp_dev.pnum_tx].typelen = sizeof(uint32_t);
		}
		else if(UARTUDP_F == uartudp_dev.para_tx[uartudp_dev.pnum_tx].Parmtype){
			uartudp_dev.para_tx[uartudp_dev.pnum_tx].typelen = sizeof(float);
		}
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].namelen = uartudp_len(uartudp_dev.para_tx[uartudp_dev.pnum_tx].name);
        uartudp_dev.para_tx[uartudp_dev.pnum_tx].ComSucceedCallBack = ComSucceedCallBack;
		uartudp_dev.para_tx[uartudp_dev.pnum_tx].OvertimeCallBack = OvertimeCallBack;
		uartudp_dev.pnum_tx++;
    }
    
    return err;
}
/* 函数原形 --------------------------------------------------------------*/
#if (UARTUDP_MODE_SLAVE == 1)
static uint16_t uartudp_help_send(_s_uartudp_dev *pQ) //发送help
{
    uint8_t j;
    uint16_t i;
    
    i=1;
    i += uartudp_strcopy(UARTUDP_TX_BUF+i,pQ->id);
    i += uartudp_strcopy(UARTUDP_TX_BUF+i,sys_cmd_tab[2]);
    UARTUDP_TX_BUF[i++] = ';';
    UARTUDP_TX_BUF[i++] = ' ';
    i += uartudp_strcopy(UARTUDP_TX_BUF+i,(uint8_t*)pQ->versions); 
    UARTUDP_TX_BUF[i++] = ';'; 
    UARTUDP_TX_BUF[i++] = ' ';  
    for(j=0; j<pQ->pnum_rx; j++)
    {
		i += uartudp_typecopy(UARTUDP_TX_BUF+i,pQ->para_rx[j].Class,pQ->para_rx[j].Parmtype);
		i += uartudp_strcopy(UARTUDP_TX_BUF+i,sys_cmd_tab[3]);
        i += uartudp_strcopy(UARTUDP_TX_BUF+i,pQ->para_rx[j].name); 
        UARTUDP_TX_BUF[i++] = ';'; 
        UARTUDP_TX_BUF[i++] = ' ';
		if(pQ->para_rx[j].IOstate == UARTUDP_IO)
		{
			i += uartudp_strcopy(UARTUDP_TX_BUF+i,sys_cmd_tab[4]);
			i += uartudp_strcopy(UARTUDP_TX_BUF+i,pQ->para_rx[j].name); 
			i += uartudp_typecopy(UARTUDP_TX_BUF+i,pQ->para_rx[j].Class,pQ->para_rx[j].Parmtype);
			UARTUDP_TX_BUF[i++] = ';'; 
			UARTUDP_TX_BUF[i++] = ' ';
		}
    } 
	
	return i;
}
static uint16_t uartudp_err_send(_s_uartudp_dev *pQ)  //发送错误
{
    uint16_t i;
    
    i=1;
    i += uartudp_strcopy(UARTUDP_TX_BUF+i,pQ->id);
    i += uartudp_strcopy(UARTUDP_TX_BUF+i,sys_cmd_tab[5]);
    
	return i;
}
static uint16_t uartudp_get_send(_s_uartudp_dev *pQ,uint8_t *str) //发送get
{
    uint16_t i;
	uint8_t num;
	uint8_t addr;
	uint8_t addr1;
    uint8_t j;
    
    for(addr=0; addr<pQ->pnum_rx; addr++)
    {
        if(uartudp_strcmp(pQ->para_rx[addr].name,str))
		{
			if((pQ->Parmtype == pQ->para_rx[addr].Parmtype)
			 &&(pQ->Class == pQ->para_rx[addr].Class)
			 &&(uartudp_strcmp(uartudp_dev.id,str+pQ->para_rx[addr].namelen)))
			{
				num = pQ->para_rx[addr].plen / pQ->para_rx[addr].typelen; 
				break;
			}
		}
    }
    if((addr < pQ->pnum_rx)&&(pQ->Parmtype != UARTUDP_VOID))
    {
        i=1;
        i += uartudp_strcopy(UARTUDP_TX_BUF+i,pQ->id);
        i += uartudp_strcopy(UARTUDP_TX_BUF+i,pQ->para_rx[addr].name);
        if(1 == num) UARTUDP_TX_BUF[i++] = sys_cmd_tab[0][1];
        else UARTUDP_TX_BUF[i++] = sys_cmd_tab[0][3];
        addr1 = 0;
        for(j=num; j>0; j--)
        {
            i += uartudp_datacopy(UARTUDP_TX_BUF+i,(uint8_t*)pQ->para_rx[addr].func+addr1,pQ->Parmtype);
            if(j > 1)
            {
                UARTUDP_TX_BUF[i++] = ',';
                addr1 += pQ->para_rx[addr].typelen;
            }
        }
        if(1 == num) UARTUDP_TX_BUF[i++] = sys_cmd_tab[0][2];
        else UARTUDP_TX_BUF[i++] = sys_cmd_tab[0][4];
		if(pQ->para_rx[addr].ComSucceedCallBack != 0)
		{
			pQ->para_rx[addr].ComSucceedCallBack(&pQ->para_rx[addr],pQ->Order);
		}
    }
    else
    {
        i =  uartudp_err_send(pQ);
    }
    return i;
}
static uint16_t uartudp_set_send(_s_uartudp_dev *pQ,uint8_t *str) //发送set
{
    uint16_t i;
    uint8_t j;
    uint8_t *k;
    uint8_t addr;
    int8_t addr1;
    uint8_t err;
	
    err = 1;
	for(addr=0; addr<pQ->pnum_rx; addr++)
    {
        if(uartudp_strcmp(pQ->para_rx[addr].name,str))
		{
			if((pQ->Class == pQ->para_rx[addr].Class)
			 &&(pQ->para_rx[addr].IOstate == UARTUDP_IO))
			{
				pQ->Parmtype = pQ->para_rx[addr].Parmtype;
				
				break;
			}
		}
	}
    if((addr < pQ->pnum_rx)&&(pQ->Parmtype != UARTUDP_VOID))//命令是有效的
    {
        err = 0;
        /************解析字符串**************/
        k = str;
		uartudp_shifting("([",&k);
		if(*k != '\0')
		{
			k++;
		}
		else
		{
			err = 1;
		}
        //=============================================================
        addr1 = 0;
        j = uartudp_same(str,',');
        if(pQ->Class == UARTUDP_ARRAY)
        {
            addr1 = (atoi((char*)k)*pQ->para_rx[addr].typelen);
            if((((addr1/pQ->para_rx[addr].typelen)+j) > pQ->para_rx[addr].plen)||(addr1 < 0))
            {
                err = 1;
            }
			uartudp_shifting("-",&k);
			if(*k != '\0')
			{
				k++;
			}
			else
			{
				err = 1;
			}
        }
        if(0 == err)
        {
            for(; j>0; j--)
            {
                uartudp_dataset(k,(uint8_t*)pQ->para_rx[addr].func+addr1,pQ->Parmtype);
                addr1 += pQ->para_rx[addr].typelen;
                if(pQ->Class == UARTUDP_ARRAY)
                {
					uartudp_shifting(",",&k);
                    k++;
                }
            }
            /************发送字符串**************/
			if(addr == 0)//修改ID（sequence）命令
			{
				uartudp_dev.id[2] = uartudp_sequence[0];
				uartudp_dev.id[3] = uartudp_sequence[1];
			}
            i=1;
            i += uartudp_strcopy(UARTUDP_TX_BUF+i,pQ->id);
            i += uartudp_strcopy(UARTUDP_TX_BUF+i,pQ->para_rx[addr].name);
			i += uartudp_typecopy(UARTUDP_TX_BUF+i,pQ->para_rx[addr].Class,pQ->para_rx[addr].Parmtype);
            UARTUDP_TX_BUF[i++] = 'O';
            UARTUDP_TX_BUF[i++] = 'K';
			if(pQ->para_rx[addr].ComSucceedCallBack != 0)
			{
				pQ->para_rx[addr].ComSucceedCallBack(&pQ->para_rx[addr],pQ->Order);
			}
        }
    }
    if(1 == err)
    {
        i = uartudp_err_send(pQ);
    }
    return i;
}
static void uartudp_readorder(uint8_t *str,uint8_t len,_s_uartudp_dev *pQ)
{
    switch(str[STR_ADDR_ORDER])
    {
        case 'h': {  
            if( (uartudp_strcmp(sys_cmd_tab[2],str+STR_ADDR_ORDER))
			  &&(len == 14))
            {
                pQ->Order = UARTUDP_HELP;
            }  
            else
            {
                pQ->Order = UARTUDP_RETAIN;
            }                
            pQ->Class = UARTUDP_NOT;
            pQ->Parmtype = UARTUDP_VOID;
            break;
		}
        case '(': {
            pQ->Order = UARTUDP_GET_;
            pQ->Class = UARTUDP_PARA;
            if(uartudp_strcmp("f)",str+STR_ADDR_ORDER+1))
            {
                pQ->Parmtype = UARTUDP_F;
            }
            else if(str[STR_ADDR_ORDER+1] == 'u')
            {
                if(uartudp_strcmp("8)",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_U8;
                }
                else if(uartudp_strcmp("16)",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_U16;
                }
                else if(uartudp_strcmp("32)",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_U32;
                }
                else
                {
                    pQ->Parmtype = UARTUDP_VOID;
                }
            }
            else if(str[STR_ADDR_ORDER+1] == 's')
            {
                if(uartudp_strcmp("8)",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_S8;
                }
                else if(uartudp_strcmp("16)",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_S16;
                }
                else if(uartudp_strcmp("32)",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_S32;
                }
                else
                {
                    pQ->Parmtype = UARTUDP_VOID;
                }
            }
            else
            {
                pQ->Parmtype = UARTUDP_VOID;
            }
            break;
		}
        case '[': {
            pQ->Order = UARTUDP_GET_;
            pQ->Class = UARTUDP_ARRAY;
            if(uartudp_strcmp("f]",str+STR_ADDR_ORDER+1))
            {
                pQ->Parmtype = UARTUDP_F;
            }
            else if(str[STR_ADDR_ORDER+1] == 'u')
            {
                if(uartudp_strcmp("8]",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_U8;
                }
                else if(uartudp_strcmp("16]",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_U16;
                }
                else if(uartudp_strcmp("32]",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_U32;
                }
                else
                {
                    pQ->Parmtype = UARTUDP_VOID;
                }
            }
            else if(str[STR_ADDR_ORDER+1] == 's')
            {
                if(uartudp_strcmp("8]",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_S8;
                }
                else if(uartudp_strcmp("16]",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_S16;
                }
                else if(uartudp_strcmp("32]",str+STR_ADDR_ORDER+2))
                {
                    pQ->Parmtype = UARTUDP_S32;
                }
                else
                {
                    pQ->Parmtype = UARTUDP_VOID;
                }
            }
            else
            {
                pQ->Parmtype = UARTUDP_VOID;
            }
            break;
		}
        case 's': {
            pQ->Order = UARTUDP_SET_;
            if(str[len-STR_SHIFTING_ADDR_ID-1] == ')')
            {
               pQ->Class = UARTUDP_PARA;
            }
            else if(str[len-STR_SHIFTING_ADDR_ID-1] == ']')
            {
                pQ->Class = UARTUDP_ARRAY;
            }
            else
            {
                pQ->Class = UARTUDP_NOT;
            }
            break;
		}
        default:{
            pQ->Order = UARTUDP_RETAIN;
            pQ->Class = UARTUDP_NOT;
            pQ->Parmtype = UARTUDP_VOID;
            break;
		}
    }
}
static uint8_t uartudp_exe(uint8_t *str, uint16_t len,_s_uartudp_dev *pQ)
{
    uint8_t lrc;
	uint16_t i;
	uint16_t hostid;
	uint16_t slaveid;
	uint8_t *strbuff;
    /*********效验本地ID*************/
    if(!uartudp_strcmp(uartudp_dev.id,&str[len-STR_SHIFTING_ADDR_ID]))
    {
        return 1;
    }
    /***********效验LRC************/
    lrc = CheckSum(str,len-2);
    if(lrc != ASCII_HEX(&str[len-STR_SHIFTING_ADDR_LRC]))
    {
        return 1;
    }
	/*********效验主ID是否有效*********/
	pQ->Order = UARTUDP_RETAIN;
	hostid = atoi((char*)str);
	slaveid = atoi((char*)&str[len-STR_SHIFTING_ADDR_ID]);
	i = 0;
	if(slaveid > hostid)
	{
		/***********读取命令*************/
		uartudp_readorder(str,len,pQ);
		if((pQ->Order != UARTUDP_RETAIN)&&(pQ->Class != UARTUDP_NOT))
		{
			strbuff = str;
			if (uartudp_shifting("_",&strbuff))
			{
				if((uartudp_strcmp(sys_cmd_tab[3],strbuff-3) == 0)
				 &&(uartudp_strcmp(sys_cmd_tab[4],strbuff-3) == 0))
				{
					pQ->Order = UARTUDP_RETAIN;
				}
			}
			else
			{
				pQ->Order = UARTUDP_RETAIN;
			}
		}
	}
	i = 0;
    UARTUDP_TX_BUF[i++] = sys_cmd_tab[0][0];
    switch(pQ->Order)
    {
        case UARTUDP_HELP:
            i = uartudp_help_send(pQ);
            break;
        case UARTUDP_GET_:
            i = uartudp_get_send(pQ,strbuff+1);
            break;
        case UARTUDP_SET_:
            i = uartudp_set_send(pQ,strbuff+1);
            break;
        default:
            i = uartudp_err_send(pQ);
            break;
    }
    /************写入效验码字符串**************/
    lrc = CheckSum(UARTUDP_TX_BUF+1,i-1);
    
    HEX_ASCII(&lrc,UARTUDP_TX_BUF+i); 

    i += 2;
    /************写入结束码字符串**************/
    UARTUDP_TX_BUF[i++] = sys_cmd_tab[1][0];
    UARTUDP_TX_BUF[i++] = sys_cmd_tab[1][1];
    
    UARTUDP_TX_STA = i;
    UARTUDP_TX_STA |= RECEIVE_OK;
    
    return 0;
}
static uint8_t uartudp_SLAVEScan(_s_uartudp_dev *handle)
{
	uint8_t err;
	uint16_t len;
	
	err = 1;
	if((UARTUDP_RX_STA&RECEIVE_OK)&&(0 == UARTUDP_TX_STA))
    {
		len = UARTUDP_RX_STA&(~RECEIVE_OK);
		if(
			(sys_cmd_tab[0][0] == UARTUDP_RX_BUF[0])
		  &&(sys_cmd_tab[1][0] == UARTUDP_RX_BUF[len-2])
		  &&(sys_cmd_tab[1][1] == UARTUDP_RX_BUF[len-1])
		  )
		{
			err = uartudp_exe(&UARTUDP_RX_BUF[1],len-3,handle);
		}
		
        UARTUDP_RX_STA = 0;
    }
	
	return err;
}
#else
static uint8_t Tim_CheckTimeOut(uint16_t TimeStart, uint16_t TimeNow, uint16_t TimeOut)
{
  #define ActivateCounter ((uint16_t)(TimeOut+TimeStart))
  if (ActivateCounter >= TimeStart)
  {
    if ((TimeNow >= ActivateCounter) ||
        (TimeNow < TimeStart))
    {
      return 1;
    }
  }
  else if ((TimeNow >= ActivateCounter) &&
           (TimeNow < TimeStart))
  {
    return 1;
  }
  return 0;
}
static uint8_t uartudp_HOSTAnalysisUsedPG(_s_uartudp_dev *handle,uint8_t *num)
{
	uint8_t err;
	uint16_t len;
	uint8_t lrc;
	uint8_t *str1;
	uint8_t i;
	uint8_t j;
	uint8_t *k;
	uint16_t addr;
	uint8_t *databuff;
	
	err = 1;
	if((UARTUDP_RX_STA&RECEIVE_OK)&&(0 == UARTUDP_TX_STA))//是否有一帧数据
    {
		len = UARTUDP_RX_STA&(~RECEIVE_OK);
		if(
            (sys_cmd_tab[0][0] == UARTUDP_RX_BUF[0])
          &&(sys_cmd_tab[1][0] == UARTUDP_RX_BUF[len-2])
          &&(sys_cmd_tab[1][1] == UARTUDP_RX_BUF[len-1])
          )
        {
			/***********效验LRC************/
			str1 = UARTUDP_RX_BUF+1;
            len -= 3;
            lrc = CheckSum(str1,len-2);
            if(lrc != ASCII_HEX(&str1[len-STR_SHIFTING_ADDR_LRC]))
            {
                return 1;
            }
			/***********sequence************///liu: sequence功能
			if(sequence_flg == 1)
			{
				if(uartudp_strcmp(modification_sequence_buff,UARTUDP_RX_BUF))
				{
					*num = OrderSerialNumber;
					return 0;
				}
			}
			/***********读取命令*************/
            str1 = UARTUDP_RX_BUF+STR_ADDR_ORDER+1;
			for(i=0; i<handle->pnum_tx; i++)
			{
				if(
                    (UARTUDP_RX_BUF[1] == handle->para_tx[i].id[0])
                    &&(UARTUDP_RX_BUF[2] == handle->para_tx[i].id[1])
                    &&(UARTUDP_RX_BUF[3] == handle->para_tx[i].id[2])
                    &&(UARTUDP_RX_BUF[4] == handle->para_tx[i].id[3])
                  )
				{
					if(uartudp_strcmp(handle->para_tx[i].name,str1))
					{
						handle->Class = handle->para_tx[i].Class;
						handle->Parmtype = handle->para_tx[i].Parmtype;
						if(handle->para_tx[i].IOstate == UARTUDP_O)
						{
							if(('K' == UARTUDP_RX_BUF[len-2])
							 &&('O' == UARTUDP_RX_BUF[len-3]))
							{
								handle->Order = UARTUDP_SET_;
                                break;
							}
						}
						else if(handle->para_tx[i].IOstate == UARTUDP_I)
						{
							if((')' == UARTUDP_RX_BUF[len-2])
							 ||(']' == UARTUDP_RX_BUF[len-2]))
							{
								handle->Order = UARTUDP_GET_;
								break;
							}
						}
						else
						{
							handle->Order = UARTUDP_RETAIN;
						}
					}
				}
			}
			if(i < handle->pnum_tx)
			{
				*num = i;
				switch(handle->Order)
				{
					case UARTUDP_SET_:{
						err = 0;
						break;
					}	
					case UARTUDP_GET_:{
						if((handle->Parmtype != UARTUDP_VOID)&&(handle->Class != UARTUDP_NOT))//命令是有效的
						{
							/************解析字符串**************/
							k = &UARTUDP_RX_BUF[1];
							j = uartudp_same(k,',');
							if ( (j*handle->para_tx[i].typelen) == handle->para_tx[i].plen )
							{
								addr = 0;
								databuff = malloc(sizeof(uint8_t) * handle->para_tx[i].plen);
								for(; j>0; j--)
								{
									uartudp_shifting("([,",&k);
									if(*k != '\0')  
									{										
										k++;
										k += uartudp_dataset(k,(uint8_t*)databuff+addr,handle->para_tx[i].Parmtype);
										addr += handle->para_tx[i].typelen;
									}
									else
									{
										break;
									}
								}
								if(j == 0)
								{
									for(j=0; j<handle->para_tx[i].plen; j++)
									{
										((uint8_t*)handle->para_tx[i].func)[j] = databuff[j];
									}
									err = 0;
								}
								free(databuff);
							}
						}
						break;
					}	
					default:
						break;
				}
			}
		}
		UARTUDP_RX_STA = 0;
	}
	
	return err;
}
static uint8_t uartudp_HOSTSendUsedPG(_s_uartudp_dev *handle,uint8_t num)
{
	uint8_t err;
	uint16_t i;
	uint8_t lrc;
	uint16_t addr;
	uint8_t j;
	err = 1;
	if((UARTUDP_TX_STA&RECEIVE_OK) == 0)
    {
		i = 0;
		UARTUDP_TX_BUF[i++] = sys_cmd_tab[0][0];
		i += uartudp_strcopy(UARTUDP_TX_BUF+i,handle->id);
		if(handle->para_tx[num].IOstate == UARTUDP_I) //get命令
        {
            handle->Order = UARTUDP_GET_;
			i += uartudp_typecopy(UARTUDP_TX_BUF+i,handle->para_tx[num].Class,handle->para_tx[num].Parmtype);
			i += uartudp_strcopy(UARTUDP_TX_BUF+i,sys_cmd_tab[3]); 
            i += uartudp_strcopy(UARTUDP_TX_BUF+i,handle->para_tx[num].name);           
            err = 0;  
        }
        else //set命令
		{
			handle->Order = UARTUDP_SET_;
			handle->Parmtype = handle->para_tx[num].Parmtype;
			handle->Class = handle->para_tx[num].Class;
			i += uartudp_strcopy(UARTUDP_TX_BUF+i,sys_cmd_tab[4]); 
			i += uartudp_strcopy(UARTUDP_TX_BUF+i,handle->para_tx[num].name); 
			if(handle->para_tx[num].Class == UARTUDP_PARA)
			{
				UARTUDP_TX_BUF[i++] = '(';
			}
			else if(handle->para_tx[num].Class == UARTUDP_ARRAY)
			{
				UARTUDP_TX_BUF[i++] = '[';
				UARTUDP_TX_BUF[i++] = '0';
                UARTUDP_TX_BUF[i++] = '-';
			}
			if((handle->Parmtype != UARTUDP_VOID)&&(handle->Class != UARTUDP_NOT))//命令是有效的
			{
				err = 0; 
				addr = 0;
				for(j=handle->para_tx[num].plen / handle->para_tx[num].typelen; j>0; j--)
				{
					i += uartudp_datacopy(UARTUDP_TX_BUF+i,(uint8_t*)handle->para_tx[num].func+addr,handle->Parmtype);
                    if(j > 1)
                    {
                        UARTUDP_TX_BUF[i++] = ',';
                        addr += handle->para_tx[num].typelen;
                    }
				}
				if(handle->para_tx[num].Class == UARTUDP_PARA)
				{
					UARTUDP_TX_BUF[i++] = ')';
				}
				else if(handle->para_tx[num].Class == UARTUDP_ARRAY)
				{
					UARTUDP_TX_BUF[i++] = ']';
				}
			}
		}
		if(0 == err)
        {
			
            i += uartudp_strcopy(UARTUDP_TX_BUF+i,handle->para_tx[num].id);
            /************写入效验码字符串**************/
            lrc = CheckSum(UARTUDP_TX_BUF+1,i-1);
            
            HEX_ASCII(&lrc,UARTUDP_TX_BUF+i); 

            i += 2;
            /************写入结束码字符串**************/
            UARTUDP_TX_BUF[i++] = sys_cmd_tab[1][0];
            UARTUDP_TX_BUF[i++] = sys_cmd_tab[1][1];
            
            UARTUDP_TX_STA = i;
            UARTUDP_TX_STA |= RECEIVE_OK;
        } 
	}
	
	return err;
}
static void uartudp_HOSTScan(_s_uartudp_dev *handle)
{
    uint8_t counts;
    uint8_t err;
    uint8_t num;
	uint16_t timebuff;
	uint16_t i;
    /**************************************************************************************
	***************************************************************************************
	**************************************接收部分*****************************************
	***************************************************************************************
	**************************************************************************************/
    err = uartudp_HOSTAnalysisUsedPG(handle,&num);
    if(!err)
    {
        if(UARTUDP_PG_TX_TxING == handle->para_tx[num].PGState)
		{
			handle->para_tx[num].PGState = UARTUDP_PG_TX_SUC;
		}
    }
    /**************************************************************************************
	***************************************************************************************
	**************************************发送部分*****************************************
	***************************************************************************************
	**************************************************************************************/
    for (counts=0; counts<handle->pnum_tx; counts++)
    {
		if(handle->para_tx[counts].PGState == UARTUDP_PG_TX_REQ)
		{
			if ( (handle->para_tx[counts].TransType  == UARTUDP_ASYNC)
			   ||( (handle->para_tx[counts].TransType == UARTUDP_CYCLIC)
			     &&(Tim_CheckTimeOut(handle->para_tx[counts].Timer,handle->Timer,handle->para_tx[counts].TransRate))
			     )
			   )
			{
				if(UARTUDP_TX_FREE == handle->TxSta)
                {
                    err = uartudp_HOSTSendUsedPG(handle,counts);
                    if(!err)
                    {
                        handle->para_tx[counts].PGState = UARTUDP_PG_TX_TxING;
                        handle->para_tx[counts].Timer = handle->Timer; 
                        handle->TxSta = UARTUDP_TX_TxING;
						
						if((uartudp_strcmp(handle->para_tx[counts].name,sys_cmd_tab[6]) == 1)//liu: sequence功能
					    &&(handle->para_tx[counts].IOstate == UARTUDP_O))
						{
							modification_sequence_buff[3] = ((uint8_t *)(handle->para_tx[counts].func))[0];
							modification_sequence_buff[4] = ((uint8_t *)(handle->para_tx[counts].func))[1];
							OrderSerialNumber = counts;
							sequence_flg = 1;
						}
                        break;
                    }
                }
			}
		}
		else if(handle->para_tx[counts].PGState == UARTUDP_PG_TX_TxING)
		{
			if(handle->para_tx[counts].TransRate < 20)
			{
				timebuff = 20;
			}
			else if(handle->para_tx[counts].TransRate > 1000)
			{
				timebuff = 1000;
			}
			else
			{
				timebuff = handle->para_tx[counts].TransRate;
			}

			if(Tim_CheckTimeOut(handle->para_tx[counts].Timer,handle->Timer,timebuff))
			{
                    if(handle->para_tx[counts].txerrnum >= 4)
                    {
                        handle->para_tx[counts].PGState = UARTUDP_PG_TX_ERR;//未响应,报错模式
                    }
                    else
                    {
						handle->para_tx[counts].Timer = handle->Timer; //目前加了这一条，不知道对这个程序是否有影响
                        handle->para_tx[counts].txerrnum++;
                        handle->para_tx[counts].PGState = UARTUDP_PG_TX_REQ; 
                    }
					if((OrderSerialNumber == counts)&&(sequence_flg == 1))//liu: sequence功能
					{
						OrderSerialNumber = 0xff;
						sequence_flg = 0;
					}
					
                    handle->TxSta = UARTUDP_TX_FREE;
					if(handle->para_tx[counts].OvertimeCallBack != 0)
					{
						handle->para_tx[counts].OvertimeCallBack(&handle->para_tx[counts]);
					}
			}
			break;
		}
		else if (handle->para_tx[counts].PGState == UARTUDP_PG_TX_SUC)
		{
			handle->para_tx[counts].txerrnum = 0;
			if(handle->para_tx[counts].TransType  == UARTUDP_CYCLIC)
			{
				handle->para_tx[counts].Timer = handle->Timer; 
                handle->para_tx[counts].PGState = UARTUDP_PG_TX_REQ; 
			}
			else
			{
				handle->para_tx[counts].PGState = UARTUDP_PG_TX_FREE; 
			}
			handle->TxSta = UARTUDP_TX_FREE; 
			if((OrderSerialNumber == counts)&&(sequence_flg == 1))//liu: sequence功能
			{
				for (i=0; i < handle->pnum_tx; i++)
				{
					if(i != OrderSerialNumber)
					{
						if(uartudp_strcmp(handle->para_tx[i].id,handle->para_tx[OrderSerialNumber].id))
						{
							handle->para_tx[i].id[2] = modification_sequence_buff[3];
							handle->para_tx[i].id[3] = modification_sequence_buff[4];
						}
					}
				}
				
				OrderSerialNumber = 0xff;
				sequence_flg = 0;
			}
			if(handle->para_tx[counts].ComSucceedCallBack != 0)
			{
				handle->para_tx[counts].ComSucceedCallBack(&handle->para_tx[counts]);
			}
		}
    }
}
#endif














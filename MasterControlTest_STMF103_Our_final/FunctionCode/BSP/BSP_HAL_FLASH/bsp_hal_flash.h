#ifndef __bsp_hal_flash_H__
#define __bsp_hal_flash_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "mytype.h"
/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/************************** STM32 �ڲ� FLASH ���� *****************************/
#define STM32_FLASH_SIZE        64  // ��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN        1    // stm32оƬ����FLASH д��ʹ��(0������;1��ʹ��)


#if STM32_FLASH_SIZE < 256
  #define STM_SECTOR_SIZE  1024 //�ֽ�
#else 
  #define STM_SECTOR_SIZE	 2048
#endif


#define STM_FLASH_BASE  0x800F000            //FLASH�����ŵ��׵�ַ   
#define STM_FLASH_SIZE  4     //FLASH�����ŵĵ�������С����λΪK��

#define FLASH_ERASE_OK 1

#define FLASH_WRITE_OK 1
#define FLASH_SAME_DATA 2
#define FLASH_WRITE_ERR 3
#define FLASH_ILLEGALITY_ADDRESS 4

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/

void NVM_Read(u32 ReadAddr,u8* pBuffer,u16 NumToRead);
u8 NVM_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite);


#endif /* __bsp_hal_flash_H__ */


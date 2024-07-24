#ifndef __bsp_hal_flash_H__
#define __bsp_hal_flash_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "mytype.h"
/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/************************** STM32 内部 FLASH 配置 *****************************/
#define STM32_FLASH_SIZE        64  // 所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN        1    // stm32芯片内容FLASH 写入使能(0，禁用;1，使能)


#if STM32_FLASH_SIZE < 256
  #define STM_SECTOR_SIZE  1024 //字节
#else 
  #define STM_SECTOR_SIZE	 2048
#endif


#define STM_FLASH_BASE  0x800F000            //FLASH所开放的首地址   
#define STM_FLASH_SIZE  4     //FLASH所开放的的容量大小（单位为K）

#define FLASH_ERASE_OK 1

#define FLASH_WRITE_OK 1
#define FLASH_SAME_DATA 2
#define FLASH_WRITE_ERR 3
#define FLASH_ILLEGALITY_ADDRESS 4

/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/

void NVM_Read(u32 ReadAddr,u8* pBuffer,u16 NumToRead);
u8 NVM_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite);


#endif /* __bsp_hal_flash_H__ */


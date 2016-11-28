//*******************************************************************************	 
//处理器  :K10DN512VLL10  K60X256VLL10
//作者    :ZPW
//修改日期:2016/03
//文件    :System32.h
//版本    ：V0.1
//********************************************************************************

#ifndef __SYSTEM32_H__
#define __SYSTEM32_H__

#include "MK60D10.h"

/********************************************GPIO口简化定义**************************************************************/
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射

//IO口地址映射
#define GPIOA_ODR_Addr    (PTA_BASE+0) //0x4001080C 
#define GPIOB_ODR_Addr    (PTB_BASE+0) //0x40010C0C 
#define GPIOC_ODR_Addr    (PTC_BASE+0) //0x4001100C 
#define GPIOD_ODR_Addr    (PTD_BASE+0) //0x4001140C 
#define GPIOE_ODR_Addr    (PTE_BASE+0) //0x4001180C 
#define GPIOF_ODR_Addr    (PTF_BASE+0) //0x40011A0C    
#define GPIOG_ODR_Addr    (PTG_BASE+0) //0x40011E0C    

#define GPIOA_IDR_Addr    (PTA_BASE+0x10) //0x40010808 
#define GPIOB_IDR_Addr    (PTB_BASE+0x10) //0x40010C08 
#define GPIOC_IDR_Addr    (PTC_BASE+0x10) //0x40011008 
#define GPIOD_IDR_Addr    (PTD_BASE+0x10) //0x40011408 
#define GPIOE_IDR_Addr    (PTE_BASE+0x10) //0x40011808 
#define GPIOF_IDR_Addr    (PTF_BASE+0x10) //0x40011A08 
#define GPIOG_IDR_Addr    (PTG_BASE+0x10) //0x40011E08 

//IO口操作,只对单一的IO口!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define IS_DELAY_US(nTime) (nTime >=0 && nTime <= 798915)
#define IS_DELAY_MS(nTime) (nTime >=0 && nTime <= 798)
#define IS_DELAY_EXMS(nTime) (nTime >=0 && nTime <= 4294967296)

void SysTick_Init(void);
void Delay_us(__IO uint32_t nTime);
void Delay_ms(__IO uint16_t nTime);
void Delay_EXms(__IO uint32_t nTime);
#endif

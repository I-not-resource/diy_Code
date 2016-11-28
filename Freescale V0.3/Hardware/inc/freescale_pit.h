 //*******************************************************************************	 
//处理器  :K10DN512VLL10  K60X256VLL10
//作者    :ZPW
//修改日期:2016/02
//文件    :freescale_pit.h
//版本    ：V0.0
//********************************************************************************

#ifndef __FREESCALE_PIT_H__
#define __FREESCALE_PIT_H__
#include "MK60D10.h"
#include "AllDefine.h"



typedef enum{
	PIT0 = 0X00,
	PIT1 = 0X01,
	PIT2 = 0X02,
	PIT3 = 0X03
}PITn_Typedef;


typedef struct
{
  uint8_t PITn;

  uint32_t TIM_Period;
	
}PIT_InitTypeDef;

void PIT_Init(PIT_InitTypeDef *PIT_InitStruct);
void PIT_Flag_Clear(uint8 PITn);


#endif



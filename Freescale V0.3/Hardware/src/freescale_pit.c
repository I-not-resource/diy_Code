 //*******************************************************************************	 
//处理器  :K10DN512VLL10  K60X256VLL10
//作者    :ZPW
//修改日期:2016/02
//文件    :freescale_pit.c
//版本    ：V0.0
//********************************************************************************

#include "freescale_pit.h"


/*--------------------------------------------------------------------------
函数名称：PIT_Init
函数功能：PIT中断定时器
函数备注：
--------------------------------------------------------------------------*/
void PIT_Init(PIT_InitTypeDef *PIT_InitStruct)
{
  
	PIT->MCR = 0x00;
	PIT->CHANNEL[PIT_InitStruct->PITn].TCTRL = 0X00;
	PIT->CHANNEL[PIT_InitStruct->PITn].LDVAL  = PIT_InitStruct->TIM_Period-1;
  NVIC_EnableIRQ((IRQn_Type)(PIT_InitStruct->PITn+68));
	PIT_Flag_Clear(PIT_InitStruct->PITn);
	PIT->CHANNEL[PIT_InitStruct->PITn].TCTRL |= (PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK);//开启定时器及中断，见参考手册976页
}

/*--------------------------------------------------------------------------
函数名称：PIT_Flag_Clear
函数功能：PIT中断标志位清除
函数备注：
--------------------------------------------------------------------------*/
 void PIT_Flag_Clear(uint8 PITn)
 {
 
   PIT->CHANNEL[PITn].TFLG|=PIT_TFLG_TIF_MASK;
	 
 }
 

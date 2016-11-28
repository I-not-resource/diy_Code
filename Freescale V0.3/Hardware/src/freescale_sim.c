 //*******************************************************************************	 
//处理器  :K10DN512VLL10  K60X256VLL10
//作者    :ZPW
//修改日期:2016/02
//文件    :freescale_sim.c
//版本    ：V0.2
//********************************************************************************

#include "freescale_sim.h"
PInfo_Type PInfo;
/*--------------------------------------------------------------------------
函数名称：SIM_SCGC1PeriphClockCmd
函数功能：SCGC4时钟控制函数
函数备注：
--------------------------------------------------------------------------*/
void SIM_SCGC1PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    SIM->SCGC1 |= SIM_Periph;
  }
  else
  {
    SIM->SCGC1 &= ~SIM_Periph;
  }

}

/*--------------------------------------------------------------------------
函数名称：SIM_SCGC2PeriphClockCmd
函数功能：SCGC4时钟控制函数
函数备注：
--------------------------------------------------------------------------*/
void SIM_SCGC2PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    SIM->SCGC2 |= SIM_Periph;
  }
  else
  {
    SIM->SCGC2 &= ~SIM_Periph;
  }

}

/*--------------------------------------------------------------------------
函数名称：SIM_SCGC3PeriphClockCmd
函数功能：SCGC4时钟控制函数
函数备注：
--------------------------------------------------------------------------*/
void SIM_SCGC3PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    SIM->SCGC3 |= SIM_Periph;
  }
  else
  {
    SIM->SCGC3 &= ~SIM_Periph;
  }

}

/*--------------------------------------------------------------------------
函数名称：SIM_SCGC4PeriphClockCmd
函数功能：SCGC4时钟控制函数
函数备注：
--------------------------------------------------------------------------*/
void SIM_SCGC4PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    SIM->SCGC4 |= SIM_Periph;
  }
  else
  {
    SIM->SCGC4 &= ~SIM_Periph;
  }

}

/*--------------------------------------------------------------------------
函数名称：SIM_SCGC5PeriphClockCmd
函数功能：SCGC5时钟控制函数
函数备注：
--------------------------------------------------------------------------*/
void SIM_SCGC5PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    SIM->SCGC5 |= SIM_Periph;
  }
  else
  {
    SIM->SCGC5 &= ~SIM_Periph;
  }

}

/*--------------------------------------------------------------------------
函数名称：SIM_SCGC6PeriphClockCmd
函数功能：SCGC6时钟控制函数
函数备注：
--------------------------------------------------------------------------*/
void SIM_SCGC6PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    SIM->SCGC6 |= SIM_Periph;
  }
  else
  {
    SIM->SCGC6 &= ~SIM_Periph;
  }

}

/*--------------------------------------------------------------------------
函数名称：GetProcessorInfo
函数功能：获得系统信息
函数备注：
--------------------------------------------------------------------------*/
void GetProcessorInfo(void)
{
	PInfo.FAMID=(((SIM->SDID)>>4) & 0x07);                   //获取Kinetis系列家族类型号 具体参见 SIM->SDID寄存器
	PInfo.PINID=(((SIM->SDID)>>0) & 0x0F);                   //获取Kinetis 引脚数量
	PInfo.ClockOption=EXTERNAL_PLL;                          //系统时钟选项
	SystemCoreClockUpdate();                                 //重新计算CoreClock
	PInfo.CoreClock=SystemCoreClock;                         //CoreClock
	PInfo.BusClock=(SystemCoreClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV2_MASK)>>SIM_CLKDIV1_OUTDIV2_SHIFT)+1));     //计算BusClock
	PInfo.FlexBusClock=(SystemCoreClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV3_MASK)>>SIM_CLKDIV1_OUTDIV3_SHIFT)+1)); //计算FlexBusClock
	PInfo.FlashClock=(SystemCoreClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV4_MASK)>>SIM_CLKDIV1_OUTDIV4_SHIFT)+1));   //计算FlashClock
  PInfo.FlashSize=(SIM->FCFG1>>24)&0x0F;                                                                         //此处获得的是PFlash代号，具体值请参见SIM_FCFG1
	PInfo.RAMSize  =(SIM->SOPT1>>12)&0x0F;                                                                         //此处获得的是RAM代号，具体值请参见SIM_SOPT1
}























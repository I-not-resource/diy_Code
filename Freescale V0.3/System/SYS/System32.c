 //*******************************************************************************	 
//处理器  :K10DN512VLL10  K60X256VLL10
//作者    :ZPW
//修改日期:2016/03
//文件    :System32.c
//版本    ：V0.1
//********************************************************************************


#include "System32.h"
static uint16_t fac_us = 0;
static uint32_t fac_ms = 0;
/*--------------------------------------------------------------------------
函数名称：SysTick_Init
函数功能：系统滴答时钟启动
函数备注：SYSCLK默认等于核心频率为48MHz
--------------------------------------------------------------------------*/
void SysTick_Init(void)
{
	SysTick->CTRL      |= 0x04;                       //设置系统滴答时钟源为系统内核频率
	GetProcessorInfo();                              //计算系统时钟
	fac_us              = PInfo.CoreClock/1000000;
	fac_ms              = (uint32_t)fac_us*1000;
}


/*--------------------------------------------------------------------------
函数名称：Delay_us
函数功能：微秒级延时，会占用CPU运行时间
函数备注：最大值——798915
--------------------------------------------------------------------------*/
void Delay_us(__IO uint32_t nTime)
{
	
	uint32_t State;
	
	assert_param(IS_DELAY_US(nTime));
	
	SysTick -> LOAD = nTime * fac_us;     //Load time
	SysTick -> VAL   = 0X00;              //Clear Counter
	SysTick -> CTRL |= 0X01;              //Start time
	do                                    //Wait the time will over
	{
		State = SysTick -> CTRL;
	}
	while((State & 0x01) && !(State & (1 << 16)));
	SysTick -> CTRL &= ~0X01;           //Close Counter
	SysTick -> VAL  = 0x00;             //Clear Counter
}

/*--------------------------------------------------------------------------
函数名称：毫秒级延时
函数功能：毫秒级延时，会占用CPU运行时间
函数备注：最大值——798
--------------------------------------------------------------------------*/
void Delay_ms(__IO uint16_t nTime)
{
	
	uint32_t State;
	
	assert_param(IS_DELAY_MS(nTime));
	
	SysTick -> LOAD = nTime * fac_ms;     //Load time
	SysTick -> VAL   = 0X00;              //Clear Counter
	SysTick -> CTRL |= 0X01;              //Start time
	do                                    //Wait the time will over
	{
		State = SysTick -> CTRL;
	}
	while((State & 0x01) && !(State & (1 << 16)));
	SysTick -> CTRL &= ~0X01;      //Close Counter
	SysTick -> VAL   = 0x00;       //Clear Counter	
}

/*--------------------------------------------------------------------------
函数名称：毫秒级延时
函数功能：毫秒级延时，会占用CPU运行时间
函数备注：最大值——4294967296
--------------------------------------------------------------------------*/
void Delay_EXms(__IO uint32_t nTime)
{
	
	uint32_t Timex;
  
	assert_param(IS_DELAY_EXMS(nTime));
	
	Timex = nTime / 512 + ((nTime % 512) > 0) ;
	for(;Timex>0;Timex--)
	  Delay_ms(512);
	
}










 //*******************************************************************************	 
//������  :K10DN512VLL10  K60X256VLL10
//����    :ZPW
//�޸�����:2016/03
//�ļ�    :System32.c
//�汾    ��V0.1
//********************************************************************************


#include "System32.h"
static uint16_t fac_us = 0;
static uint32_t fac_ms = 0;
/*--------------------------------------------------------------------------
�������ƣ�SysTick_Init
�������ܣ�ϵͳ�δ�ʱ������
������ע��SYSCLKĬ�ϵ��ں���Ƶ��Ϊ48MHz
--------------------------------------------------------------------------*/
void SysTick_Init(void)
{
	SysTick->CTRL      |= 0x04;                       //����ϵͳ�δ�ʱ��ԴΪϵͳ�ں�Ƶ��
	GetProcessorInfo();                              //����ϵͳʱ��
	fac_us              = PInfo.CoreClock/1000000;
	fac_ms              = (uint32_t)fac_us*1000;
}


/*--------------------------------------------------------------------------
�������ƣ�Delay_us
�������ܣ�΢�뼶��ʱ����ռ��CPU����ʱ��
������ע�����ֵ����798915
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
�������ƣ����뼶��ʱ
�������ܣ����뼶��ʱ����ռ��CPU����ʱ��
������ע�����ֵ����798
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
�������ƣ����뼶��ʱ
�������ܣ����뼶��ʱ����ռ��CPU����ʱ��
������ע�����ֵ����4294967296
--------------------------------------------------------------------------*/
void Delay_EXms(__IO uint32_t nTime)
{
	
	uint32_t Timex;
  
	assert_param(IS_DELAY_EXMS(nTime));
	
	Timex = nTime / 512 + ((nTime % 512) > 0) ;
	for(;Timex>0;Timex--)
	  Delay_ms(512);
	
}










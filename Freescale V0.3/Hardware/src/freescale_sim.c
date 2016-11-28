 //*******************************************************************************	 
//������  :K10DN512VLL10  K60X256VLL10
//����    :ZPW
//�޸�����:2016/02
//�ļ�    :freescale_sim.c
//�汾    ��V0.2
//********************************************************************************

#include "freescale_sim.h"
PInfo_Type PInfo;
/*--------------------------------------------------------------------------
�������ƣ�SIM_SCGC1PeriphClockCmd
�������ܣ�SCGC4ʱ�ӿ��ƺ���
������ע��
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
�������ƣ�SIM_SCGC2PeriphClockCmd
�������ܣ�SCGC4ʱ�ӿ��ƺ���
������ע��
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
�������ƣ�SIM_SCGC3PeriphClockCmd
�������ܣ�SCGC4ʱ�ӿ��ƺ���
������ע��
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
�������ƣ�SIM_SCGC4PeriphClockCmd
�������ܣ�SCGC4ʱ�ӿ��ƺ���
������ע��
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
�������ƣ�SIM_SCGC5PeriphClockCmd
�������ܣ�SCGC5ʱ�ӿ��ƺ���
������ע��
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
�������ƣ�SIM_SCGC6PeriphClockCmd
�������ܣ�SCGC6ʱ�ӿ��ƺ���
������ע��
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
�������ƣ�GetProcessorInfo
�������ܣ����ϵͳ��Ϣ
������ע��
--------------------------------------------------------------------------*/
void GetProcessorInfo(void)
{
	PInfo.FAMID=(((SIM->SDID)>>4) & 0x07);                   //��ȡKinetisϵ�м������ͺ� ����μ� SIM->SDID�Ĵ���
	PInfo.PINID=(((SIM->SDID)>>0) & 0x0F);                   //��ȡKinetis ��������
	PInfo.ClockOption=EXTERNAL_PLL;                          //ϵͳʱ��ѡ��
	SystemCoreClockUpdate();                                 //���¼���CoreClock
	PInfo.CoreClock=SystemCoreClock;                         //CoreClock
	PInfo.BusClock=(SystemCoreClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV2_MASK)>>SIM_CLKDIV1_OUTDIV2_SHIFT)+1));     //����BusClock
	PInfo.FlexBusClock=(SystemCoreClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV3_MASK)>>SIM_CLKDIV1_OUTDIV3_SHIFT)+1)); //����FlexBusClock
	PInfo.FlashClock=(SystemCoreClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV4_MASK)>>SIM_CLKDIV1_OUTDIV4_SHIFT)+1));   //����FlashClock
  PInfo.FlashSize=(SIM->FCFG1>>24)&0x0F;                                                                         //�˴���õ���PFlash���ţ�����ֵ��μ�SIM_FCFG1
	PInfo.RAMSize  =(SIM->SOPT1>>12)&0x0F;                                                                         //�˴���õ���RAM���ţ�����ֵ��μ�SIM_SOPT1
}























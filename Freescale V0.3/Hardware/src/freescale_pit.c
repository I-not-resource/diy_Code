 //*******************************************************************************	 
//������  :K10DN512VLL10  K60X256VLL10
//����    :ZPW
//�޸�����:2016/02
//�ļ�    :freescale_pit.c
//�汾    ��V0.0
//********************************************************************************

#include "freescale_pit.h"


/*--------------------------------------------------------------------------
�������ƣ�PIT_Init
�������ܣ�PIT�ж϶�ʱ��
������ע��
--------------------------------------------------------------------------*/
void PIT_Init(PIT_InitTypeDef *PIT_InitStruct)
{
  
	PIT->MCR = 0x00;
	PIT->CHANNEL[PIT_InitStruct->PITn].TCTRL = 0X00;
	PIT->CHANNEL[PIT_InitStruct->PITn].LDVAL  = PIT_InitStruct->TIM_Period-1;
  NVIC_EnableIRQ((IRQn_Type)(PIT_InitStruct->PITn+68));
	PIT_Flag_Clear(PIT_InitStruct->PITn);
	PIT->CHANNEL[PIT_InitStruct->PITn].TCTRL |= (PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK);//������ʱ�����жϣ����ο��ֲ�976ҳ
}

/*--------------------------------------------------------------------------
�������ƣ�PIT_Flag_Clear
�������ܣ�PIT�жϱ�־λ���
������ע��
--------------------------------------------------------------------------*/
 void PIT_Flag_Clear(uint8 PITn)
 {
 
   PIT->CHANNEL[PITn].TFLG|=PIT_TFLG_TIF_MASK;
	 
 }
 

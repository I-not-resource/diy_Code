 //*******************************************************************************	 
//������  :K10DN512VLL10  K60X256VLL10
//����    :ZPW
//�޸�����:2016/03
//�ļ�    :freescale_gpio.c
//�汾    ��V0.1
//********************************************************************************
#include "freescale_gpio.h"

/*--------------------------------------------------------------------------
�������ƣ�GPIO_Init
�������ܣ�IO�ڳ�ʼ��
������ע��
--------------------------------------------------------------------------*/
void GPIO_Init(PORT_Type* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  uint8_t _pin;
	GPIO_Type *_GPIOn_Type;
	
	switch((uint32_t)GPIOx)
	{
	  case PORTA_BASE:_GPIOn_Type = PTA;break;
		case PORTB_BASE:_GPIOn_Type = PTB;break;
		case PORTC_BASE:_GPIOn_Type = PTC;break;
		case PORTD_BASE:_GPIOn_Type = PTD;break;
		case PORTE_BASE:_GPIOn_Type = PTE;break;
	}
		
	for(_pin = 0 ; _pin < 32 ; _pin++)
	{
		if((GPIO_InitStruct->GPIO_Pin & (1<<_pin)) != 0)
		{
			GPIOx->PCR[_pin] &= ~0X7FF;
			GPIOx->PCR[_pin] |= 1<<8;
			
			if(GPIO_InitStruct->GPIO_Mode)
				_GPIOn_Type->PDDR                 |= 1<<_pin;
			else
			{
				_GPIOn_Type->PDDR                 &= ~(1<<_pin);
				GPIOx->PCR[_pin]                  |= 1<<1;
				GPIOx->PCR[_pin]                  &= ~GPIO_InitStruct->GPIO_PuPd;
			}
		}
	}
}

/*--------------------------------------------------------------------------
�������ƣ�GPIO_PinAFConfig
�������ܣ�IO�ڸ���	
������ע��
--------------------------------------------------------------------------*/
void GPIO_PinAFConfig(PORT_Type* GPIOx,uint8_t GPIO_PinSource,GPIO_AF_Typedef GPIO_AF)
{
	
	GPIOx->PCR[GPIO_PinSource] &= ~PORT_PCR_MUX_MASK;
	GPIOx->PCR[GPIO_PinSource]  = PORT_PCR_MUX(GPIO_AF);
	
}

































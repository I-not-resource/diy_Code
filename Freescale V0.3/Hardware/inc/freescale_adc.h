 //*******************************************************************************	 
//处理器  :K10DN512VLL10  K60X256VLL10
//作者    :ZPW
//修改日期:2016/03
//文件    :freescale_adc.h
//版本    ：V0.1
//********************************************************************************

#ifndef __FREESCALE_ADC_H_
#define __FREESCALE_ADC_H_
#include "MK60D10.h"





typedef enum
{
	
  ADC_TwoSamplingDelay_24Cycles = 0X00,
	ADC_TwoSamplingDelay_16Cycles = 0x01,
	ADC_TwoSamplingDelay_10Cycles = 0X02,
	ADC_TwoSamplingDelay_6Cycles  = 0X03,
	ADC_TwoSamplingDelay_4Cycles  = 0x04
	
}ADC_TwoSamplingDelay_Typedef;

typedef enum
{

	ADC_Prescaler_Div1 = 0x00,
	ADC_Prescaler_Div2 = 0x01,
	ADC_Prescaler_Div4 = 0x02,
	ADC_Prescaler_Div8 = 0x03

}ADC_Prescaler_Typedef;

typedef enum
{

	ADC_DMAAccessMode_Enable  = 0x01,
	ADC_DMAAccessMode_Disable = 0x00

}ADC_DMAAccessMode_Typedef;

typedef enum
{
	
  ADC_Resolution_8b  = 0x00,
	ADC_Resolution_10b = 0x02,
	ADC_Resolution_12b = 0x01,
	ADC_Resolution_16b = 0x03
	
}ADC_Resolution_Typedef;

typedef enum
{

	ADC_ClockSource_BLCK     = 0X00,
	ADC_ClockSource_BLCKHalf = 0X01,
	ADC_ClockSource_ALTCLK   = 0X02,
	ADC_ClockSource_ADACK    = 0X03

}ADC_ClockSource_Typedef;

typedef enum
{

	ADC_Mode_Signle = 0X00,
	ADC_Mode_DIFF   = 0X01

}ADC_Mode_Typedef;

typedef enum
{

	ADC_Channel_A = 0X00,
	ADC_Channel_B = 0X01

}ADC_Channel_Typedef;
typedef enum
{
	
	ADC_ADTRGMode_Soft = 0x00,
	ADC_ADTRGMode_Hard = 0x01

}ADC_ADTRGMode_Typedef;  //触发模式


typedef struct
{
	
	ADC_TwoSamplingDelay_Typedef ADC_TwoSamplingDelay; 
	
	ADC_ClockSource_Typedef      ADC_ClockSource;
	
	ADC_Prescaler_Typedef        ADC_Prescaler;
	
	ADC_Mode_Typedef             ADC_Mode;

	ADC_Resolution_Typedef       ADC_Resolution;
	
	ADC_Channel_Typedef          ADC_Channel;
	
	ADC_ADTRGMode_Typedef        ADC_ADTRGMode;
	
	ADC_DMAAccessMode_Typedef    ADC_DMAAccessMode;
	
	FunctionalState              ADC_ContinuousConvMode;
	
}ADC_InitTypeDef;



//function
void ADC_Init(ADC_Type* ADCx,ADC_InitTypeDef* ADC_InitStruct);
uint16_t ADC_GetConversionValue(ADC_Type* ADCx,ADC_Channel_Typedef ADC_Ch);
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_Type* ADCx,ADC_Channel_Typedef ADC_Ch);
void ADC_DiscModeChannelCountConfig(ADC_Type* ADCx, uint8_t Number,ADC_Channel_Typedef ADC_Ch);
uint8_t ADC_Calibrate(ADC_Type *adc);

#endif

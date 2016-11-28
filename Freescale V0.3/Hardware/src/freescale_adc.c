 //*******************************************************************************	 
//������  :K10DN512VLL10  K60X256VLL10
//����    :ZPW
//�޸�����:2016/03
//�ļ�    :freescale_adc.c
//�汾    ��V0.1
//********************************************************************************
#include "freescale_adc.h"


/*--------------------------------------------------------------------------
�������ƣ�ADC_Init
�������ܣ�ADC��ʼ������
������ע��
--------------------------------------------------------------------------*/
void ADC_Init(ADC_Type* ADCx,ADC_InitTypeDef* ADC_InitStruct)
{
	
//����ת������
	ADCx->CFG1 &= ~(ADC_CFG1_MODE_MASK); 
	ADCx->CFG1 |= ADC_InitStruct ->ADC_Resolution << 2;
	
//����ʱ��Դλ
	ADCx->CFG1 &= ~ADC_CFG1_ADICLK_MASK;
	ADCx->CFG1 |= ADC_InitStruct->ADC_ClockSource << 0;
	
//������ʱ��
	ADCx->CFG1 &= ~ADC_CFG1_ADLSMP_MASK;
	if(ADC_InitStruct->ADC_TwoSamplingDelay == ADC_TwoSamplingDelay_4Cycles)
		ADCx->CFG1 |= 0 << 4;
	else
	{
	  ADCx->CFG1 |= 1 << 4;
		
		ADCx->CFG2 &= ~ADC_CFG2_ADLSTS_MASK;
		ADCx->CFG2 |= ADC_InitStruct->ADC_TwoSamplingDelay << 0;
	}

//����ADC��Ƶ
	ADCx->CFG1 &= ~ADC_CFG1_ADIV_MASK;
	ADCx->CFG1 |= ADC_InitStruct->ADC_Prescaler << 5;
	
//����Ϊ���˻��߲��ģʽ
	ADCx->SC1[ADC_InitStruct->ADC_Channel] &= ~ADC_SC1_DIFF_MASK; 
	ADCx->SC1[ADC_InitStruct->ADC_Channel] |= ADC_InitStruct->ADC_Mode << 6;
	
//���ô���ģʽ�����orӲ��
	ADCx->SC2 &= ~ADC_SC2_ADTRG_MASK;
	ADCx->SC2 |= ADC_InitStruct->ADC_ADTRGMode << 6;
	
//��������ת��ģʽ
	ADCx->SC3 &= ~ADC_SC3_ADCO_MASK;
	ADCx->SC3 |= ADC_InitStruct->ADC_ContinuousConvMode << 3;
	
//DMA����
	ADCx->SC2 &= ~ADC_SC2_DMAEN_MASK;
	ADCx->SC2 |= ADC_InitStruct->ADC_DMAAccessMode << 2;
	
	ADC_Calibrate(ADCx);
}

/*--------------------------------------------------------------------------
�������ƣ�ADC_GetConversionValue
�������ܣ�AD�ɼ��������
������ע��
--------------------------------------------------------------------------*/
uint16_t ADC_GetConversionValue(ADC_Type* ADCx,ADC_Channel_Typedef ADC_Ch)
{

	return ADCx->R[ADC_Ch];

}

/*--------------------------------------------------------------------------
�������ƣ�ADC_GetSoftwareStartConvStatus
�������ܣ�ADC״̬��ʶ����
������ע������SET��1����ʾת�����
--------------------------------------------------------------------------*/
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_Type* ADCx,ADC_Channel_Typedef ADC_Ch)
{

	 FlagStatus bitstatus = RESET;
  
  /* Check the status of SWSTART bit */
  if ((ADCx->SC1[ADC_Ch] & ADC_SC1_COCO_MASK) != (uint32_t)RESET)
  {
    /* SWSTART bit is set */
    bitstatus = SET;
  }
  else
  {
    /* SWSTART bit is reset */
    bitstatus = RESET;
  }
  
  /* Return the SWSTART bit status */
  return  bitstatus;

}

/*--------------------------------------------------------------------------
�������ƣ�ADC_DiscModeChannelCountConfig
�������ܣ�ADC�ɼ�ͨ������
������ע��
--------------------------------------------------------------------------*/
void ADC_DiscModeChannelCountConfig(ADC_Type* ADCx, uint8_t Number,ADC_Channel_Typedef ADC_Ch)
{

	ADCx->SC1[ADC_Ch] &= ~(ADC_SC1_ADCH_MASK);	
	ADCx->SC1[ADC_Ch] |= ADC_SC1_ADCH(Number);

}
/*--------------------------------------------------------------------------
�������ƣ�ADC_Calibrate
�������ܣ�ADCӲ��У׼����
������ע���ٷ�ԭ����
--------------------------------------------------------------------------*/
//ADCӲ��У׼����
uint8_t ADC_Calibrate(ADC_Type *adc)
{
    uint16_t cal_var;
    struct {
        uint8_t  SC2;
        uint8_t  SC3;
    } saved_regs;
    saved_regs.SC2 = adc->SC2;
    saved_regs.SC3 = adc->SC3;
    /* Enable Software Conversion Trigger for Calibration Process */
    adc->SC2 &= ~ADC_SC2_ADTRG_MASK;
    /* Initiate calibration */
    adc->SC3 |=  ADC_SC3_AVGS(0x80);             
    /* Wait for conversion to complete */
    while (adc->SC2 & ADC_SC2_ADACT_MASK){}  
    /* Check if calibration failed */
    if (adc->SC3 & ADC_SC3_CALF_MASK)  {
        /* Clear calibration failed flag */
        adc->SC3 |= ADC_SC3_CALF_MASK;
        /* calibration failed */
        return 1;
    }
    /* Calculate plus-side calibration values according to Calibration function described in reference manual. */
    /* 1. Initialize (clear) a 16b variable in RAM */
    cal_var  = 0x0000;
    /* 2. Add the following plus-side calibration results CLP0, CLP1, CLP2, CLP3, CLP4, and CLPS to the variable. */
    cal_var +=  adc->CLP0; 
    cal_var +=  adc->CLP1;
    cal_var +=  adc->CLP2;
    cal_var +=  adc->CLP3;
    cal_var +=  adc->CLP4;
    cal_var +=  adc->CLPS;
    /* 3. Divide the variable by two. */
    cal_var = cal_var / 2;
    /* 4. Set the MSB of the variable. */
    cal_var += 0x8000;
    /* 5. Store the value in the plus-side gain calibration registers ADCPGH and ADCPGL. */
    adc->PG = cal_var;
    /* Repeat procedure for the minus-side gain calibration value. */ 
    /* 1. Initialize (clear) a 16b variable in RAM */
    cal_var  = 0x0000;
    /* 2. Add the following plus-side calibration results CLM0, CLM1, CLM2, CLM3, CLM4, and CLMS to the variable. */
    cal_var  = 0x0000;
    cal_var += adc->CLM0; 
    cal_var += adc->CLM1; 
    cal_var += adc->CLM2; 
    cal_var += adc->CLM3; 
    cal_var += adc->CLM4; 
    cal_var += adc->CLMS;
    /* 3. Divide the variable by two. */
    cal_var = cal_var / 2;
    /* 4. Set the MSB of the variable. */
    cal_var += 0x8000;
    /* 5. Store the value in the minus-side gain calibration registers ADCMGH and ADCMGL. */
    adc->MG = cal_var;
    /* Clear CAL bit */
    // adc_ptr->SC3 &= ~ADC_SC3_CAL_MASK; // done in next line
    adc->SC3 = saved_regs.SC3;
    adc->SC2 = saved_regs.SC2;
    return 0;
}





















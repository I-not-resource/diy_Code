 //*******************************************************************************	 
//������  :K10DN512VLL10  K60X256VLL10
//����    :ZPW
//�޸�����:2016/03
//�ļ�    :main.c
//�汾    ��V0.2
//���    ��keil MDK5.16a
//********************************************************************************

#include "Includes.h"

void ADC(void);
uint16_t ADC_Result(void);
void Usart_Send(uint8_t *message);
/*--------------------------------------------------------------------------
�������ƣ�main
�������ܣ�������
������ע��
--------------------------------------------------------------------------*/
int main()
{
	
	int i,j;
	char infor[100],message[10];
  GPIO_InitTypeDef  GPIO_Structure;
	PIT_InitTypeDef   PIT_Structure;
	USART_InitTypeDef USART_InitStructure;
	
	//����ʱ��
	SIM_SCGC4PeriphClockCmd(SIM_SCGC4Periph_UART0,ENABLE);
	SIM_SCGC5PeriphClockCmd(SIM_SCGC5Periph_GPIOA | SIM_SCGC5Periph_GPIOB,ENABLE);
	SIM_SCGC6PeriphClockCmd(SIM_SCGC6Periph_PIT,ENABLE);
	
	//GPIO��
	GPIO_Structure.GPIO_Pin   = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Structure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_Init(GPIOA,&GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_Structure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_Structure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin  = GPIO_Pin_16 | GPIO_Pin_17;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB,&GPIO_Structure);
	
	//��������
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource16,GPIO_AF_UART0);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource17,GPIO_AF_UART0);
	
	USART_InitStructure.USART_BaudRate             = 115200;
	USART_InitStructure.USART_WordLength           = USART_WordLength_8b;
	USART_InitStructure.USART_Parity               = USART_Parity_No;
	USART_InitStructure.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART0,&USART_InitStructure);
	
	//��ʱ���ж�
	PIT_Structure.PITn       = PIT0;
	PIT_Structure.TIM_Period = 5000000;
	PIT_Init(&PIT_Structure);
	
	SysTick_Init();
	ADC();
	
	sprintf(infor,"Model��%d\r\nPin_Num:%d\r\nFrequency:%d\r\nCoreClock:%d\r\n\r\n",PInfo.FAMID,PInfo.PINID,PInfo.ClockOption,PInfo.CoreClock);
	
	PAout(15) = 0;
	while(1)
	{
	  
		if(PAin(12) == 1)
		{
			PAout(14) = 0;
			i = 0;
			Usart_Send((uint8_t *)infor);
		  Delay_EXms(1000);

		}
		else
		{
			PAout(14) = 1;
			i = 0;
			while(USART_GetFlagStatus(UART0, USART_FLAG_RDRF))
			{
				message[i] = USART_ReceiveData(UART0);
				i++;
			}
			j = i;
			i = 0;
			while(i<j)
			{
				while(!USART_GetFlagStatus(UART0, USART_FLAG_TDRE));
				 USART_SendData(UART0,message[i]);		
				i++;
			}
		}
	  
	  
	}
return 0;
}

void ADC(void)
{

	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef  GPIO_Structure;
	
	SIM_SCGC6PeriphClockCmd(SIM_SCGC6Periph_ADC0,ENABLE);
	SIM_SCGC5PeriphClockCmd(SIM_SCGC5Periph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_ADC0);
	
	ADC_InitStructure.ADC_TwoSamplingDelay   = ADC_TwoSamplingDelay_24Cycles;  //����ʱ��
	ADC_InitStructure.ADC_Prescaler          = ADC_Prescaler_Div8;             //��Ƶ��8
	ADC_InitStructure.ADC_Resolution         = ADC_Resolution_8b;              //�������ȣ�8Bit
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Signle;                //����ģʽ
	ADC_InitStructure.ADC_ADTRGMode          = ADC_ADTRGMode_Soft;             //�������
	ADC_InitStructure.ADC_Channel            = ADC_Channel_A;                  //����ͨ��A
	ADC_InitStructure.ADC_ClockSource        = ADC_ClockSource_BLCKHalf;       //ADCʱ��Դ���������ʱ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                        //�����������ر�
	ADC_InitStructure.ADC_DMAAccessMode      = ADC_DMAAccessMode_Disable;      //DMAģʽ���ر�
	ADC_Init(ADC0,&ADC_InitStructure);

}

uint16_t ADC_Result(void)
{

	ADC_DiscModeChannelCountConfig(ADC0,10,ADC_Channel_A);
	while(!ADC_GetSoftwareStartConvStatus(ADC0,ADC_Channel_A));
  return ADC_GetConversionValue(ADC0,ADC_Channel_A);
	
}

void Usart_Send(uint8_t *message)
{

	uint16_t i=0;
	while(message[i] != '\0')
	{
	  while(!USART_GetFlagStatus(UART0, USART_FLAG_TDRE));
		 USART_SendData(UART0,message[i]);		
	  i++;
	}
	
	
}

void PIT0_IRQHandler()
{
  char message[10];
	PAout(15) = ~PAin(15);
  sprintf(message,"ADC:%d\r\n\r\n",ADC_Result());
	Usart_Send((uint8_t *)message);
  PIT_Flag_Clear(PIT0);
	
}

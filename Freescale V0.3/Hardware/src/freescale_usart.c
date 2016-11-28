 //*******************************************************************************	 
//������  :K10DN512VLL10  K60X256VLL10
//����    :ZPW
//�޸�����:2016/03
//�ļ�    :freescale_usart.c
//�汾    ��V0.0
//********************************************************************************

#include "freescale_usart.h"

/*--------------------------------------------------------------------------
�������ƣ�USART_Init
�������ܣ����ڳ�ʼ������
������ע��USARTx�������ں�
         USART_InitStruct���������������
--------------------------------------------------------------------------*/
void USART_Init(UART_Type *USARTx,USART_InitTypeDef* USART_InitStruct)
{
  uint8_t  brfa  = 0;
	uint16_t sbr   = 0;
	uint32_t clock = 0;
	GetProcessorInfo();
	clock = PInfo.BusClock;
	if((uint32_t)USARTx==UART0_BASE || (uint32_t)USARTx==UART1_BASE) 
		clock=PInfo.CoreClock;                           //UART0 UART1ʹ��CoreClock
	sbr = (uint16_t)((clock) / (USART_InitStruct->USART_BaudRate * 16));
	brfa = ((clock * 2) / USART_InitStruct->USART_BaudRate - ( sbr * 32));
	
	
	USARTx->C2 &= ~(UART_C2_RE_MASK|UART_C2_TE_MASK);	 //��ֹ���ͽ���,�μ��ֲ�1221ҳ
	//���ݸ�ʽ
	USARTx->C1 &= ~UART_C1_M_MASK;
	USARTx->C1 |= USART_InitStruct->USART_WordLength;
	//��żУ��
	USARTx->C1 &=~(UART_C1_PE_MASK);
	USARTx->C1 |= USART_InitStruct->USART_Parity;
	//��/��λ���ȴ���
	USARTx->S2 &= ~UART_S2_MSBF_MASK;
	//����������
	USARTx->BDH &= ~UART_BDH_SBR_MASK;
	USARTx->BDH |= (sbr>>8 & UART_BDH_SBR_MASK);//��5λ
	USARTx->BDL  = (sbr&UART_BDL_SBR_MASK);                                  //��8λ
	USARTx->C4  |= brfa&(UART_BDL_SBR_MASK>>3);
	//����ģʽ
	USARTx->C2  &= ~(UART_C2_RIE_MASK & UART_C2_TCIE_MASK);
	USARTx->C2  |= USART_InitStruct->USART_Mode;
	
}

/*--------------------------------------------------------------------------
�������ƣ�USART_SendData
�������ܣ��������ݷ��ͺ���
������ע��USARTx�������ں�
         Data����8Bit����
--------------------------------------------------------------------------*/
void USART_SendData(UART_Type* USARTx, uint8_t Data)
{
  USARTx->D = Data;
}

/*--------------------------------------------------------------------------
�������ƣ�USART_ReceiveData
�������ܣ��������ݽ��պ���
������ע��USARTx�������ں�
--------------------------------------------------------------------------*/
uint8_t USART_ReceiveData(UART_Type* USARTx)
{
  return USARTx->D;
}

/*--------------------------------------------------------------------------
�������ƣ�USART_GetFlagStatus
�������ܣ����ڱ�־״̬����
������ע��USARTx�������ں�
         USART_FLAG������־λ
��    �أ�״̬���
--------------------------------------------------------------------------*/
FlagStatus USART_GetFlagStatus(UART_Type* USARTx, uint8_t USART_FLAG)
{
  FlagStatus flag = RESET;
	
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	
	if((USARTx->S1 & USART_FLAG) != RESET)
		flag = SET;
	else
		flag = RESET;
	
	return flag;
}



































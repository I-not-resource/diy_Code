 //*******************************************************************************	 
//������  :K10DN512VLL10  K60X256VLL10
//����    :ZPW
//�޸�����:2016/03
//�ļ�    :freescale_usart.h
//�汾    ��V0.0
//********************************************************************************

#ifndef __FREESACLE_USART_H__
#define __FREESACLE_USART_H__
#include "MK60D10.h"
#include "AllDefine.h"



#define USART_WordLength_8b    0x00u
#define USART_WordLength_9b    0X10u
#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_8b) || \
                                      ((LENGTH) == USART_WordLength_9b))
																			
#define USART_Parity_No        0x00u
#define USART_Parity_Even      0x02u
#define USART_Parity_Odd       0x03u
#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))
																 
#define USART_Mode_Rx          0x04u
#define USART_Mode_Tx          0x08u
#define IS_USART_MODE(MODE) ((((MODE) & (uint16_t)0xFFF3) == 0x00) && ((MODE) != (uint16_t)0x00))

#define USART_FLAG_TDRE                      ((uint8_t)0x0080)    //�������ݼĴ����ձ�־λ
#define USART_FLAG_TC                        ((uint8_t)0x0040)    //������ɱ�־
#define USART_FLAG_RDRF                      ((uint8_t)0x0020)    //�������ݼĴ�������־λ
#define USART_FLAG_IDLE                      ((uint8_t)0x0010)    //�����߱�־λ
#define USART_FLAG_OR                        ((uint8_t)0x0008)    //�����������־λ
#define USART_FLAG_NF                        ((uint8_t)0x0004)    //������־
#define USART_FLAG_FE                        ((uint8_t)0x0002)    //֡�����־
#define USART_FLAG_PF                        ((uint8_t)0x0001)    //��żУ������־
#define IS_USART_FLAG(FLAG) (((FLAG) == USART_FLAG_PF)   || ((FLAG) == USART_FLAG_FE))  || \
                             ((FLAG) == USART_FLAG_TC)   || ((FLAG) == USART_FLAG_RDRF) || \
                             ((FLAG) == USART_FLAG_TDRE) ||((FLAG) == USART_FLAG_NF)    || \
                             ((FLAG) == USART_FLAG_CTS)  || ((FLAG) == USART_FLAG_OR)   ||

typedef struct
{
  uint32_t USART_BaudRate;            //������
	
  uint16_t USART_WordLength;          //���ݸ�ʽ����
	
	uint16_t USART_Parity;              //��żУ��λ
	
	uint16_t USART_Mode;                //����ģʽ
	
}USART_InitTypeDef;





void USART_Init(UART_Type *USARTx,USART_InitTypeDef* USART_InitStruct);
void USART_SendData(UART_Type* USARTx, uint8_t Data);
uint8_t USART_ReceiveData(UART_Type* USARTx);
ITStatus USART_GetFlagStatus(UART_Type* USARTx, uint8_t USART_FLAG);





#endif

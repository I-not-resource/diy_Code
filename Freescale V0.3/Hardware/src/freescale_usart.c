 //*******************************************************************************	 
//处理器  :K10DN512VLL10  K60X256VLL10
//作者    :ZPW
//修改日期:2016/03
//文件    :freescale_usart.c
//版本    ：V0.0
//********************************************************************************

#include "freescale_usart.h"

/*--------------------------------------------------------------------------
函数名称：USART_Init
函数功能：串口初始化函数
函数备注：USARTx——串口号
         USART_InitStruct——串口相关设置
--------------------------------------------------------------------------*/
void USART_Init(UART_Type *USARTx,USART_InitTypeDef* USART_InitStruct)
{
  uint8_t  brfa  = 0;
	uint16_t sbr   = 0;
	uint32_t clock = 0;
	GetProcessorInfo();
	clock = PInfo.BusClock;
	if((uint32_t)USARTx==UART0_BASE || (uint32_t)USARTx==UART1_BASE) 
		clock=PInfo.CoreClock;                           //UART0 UART1使用CoreClock
	sbr = (uint16_t)((clock) / (USART_InitStruct->USART_BaudRate * 16));
	brfa = ((clock * 2) / USART_InitStruct->USART_BaudRate - ( sbr * 32));
	
	
	USARTx->C2 &= ~(UART_C2_RE_MASK|UART_C2_TE_MASK);	 //禁止发送接受,参见手册1221页
	//数据格式
	USARTx->C1 &= ~UART_C1_M_MASK;
	USARTx->C1 |= USART_InitStruct->USART_WordLength;
	//奇偶校验
	USARTx->C1 &=~(UART_C1_PE_MASK);
	USARTx->C1 |= USART_InitStruct->USART_Parity;
	//高/地位优先传输
	USARTx->S2 &= ~UART_S2_MSBF_MASK;
	//波特率设置
	USARTx->BDH &= ~UART_BDH_SBR_MASK;
	USARTx->BDH |= (sbr>>8 & UART_BDH_SBR_MASK);//高5位
	USARTx->BDL  = (sbr&UART_BDL_SBR_MASK);                                  //低8位
	USARTx->C4  |= brfa&(UART_BDL_SBR_MASK>>3);
	//工作模式
	USARTx->C2  &= ~(UART_C2_RIE_MASK & UART_C2_TCIE_MASK);
	USARTx->C2  |= USART_InitStruct->USART_Mode;
	
}

/*--------------------------------------------------------------------------
函数名称：USART_SendData
函数功能：串口数据发送函数
函数备注：USARTx——串口号
         Data——8Bit数据
--------------------------------------------------------------------------*/
void USART_SendData(UART_Type* USARTx, uint8_t Data)
{
  USARTx->D = Data;
}

/*--------------------------------------------------------------------------
函数名称：USART_ReceiveData
函数功能：串口数据接收函数
函数备注：USARTx——串口号
--------------------------------------------------------------------------*/
uint8_t USART_ReceiveData(UART_Type* USARTx)
{
  return USARTx->D;
}

/*--------------------------------------------------------------------------
函数名称：USART_GetFlagStatus
函数功能：串口标志状态函数
函数备注：USARTx——串口号
         USART_FLAG——标志位
返    回：状态结果
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



































 //*******************************************************************************	 
//处理器  :K10DN512VLL10  K60X256VLL10
//作者    :ZPW
//修改日期:2016/03
//文件    :freescale_gpio.h
//版本    ：V0.2
//********************************************************************************

#ifndef __FREESCALE_GPIO_H__
#define __FREESCALE_GPIO_H__
#include "MK60D10.h"

#define GPIOA PORTA
#define GPIOB PORTB
#define GPIOC PORTC
#define GPIOD PORTD
#define GPIOE PORTE

#define GPIO_Pin_0                 ((uint32_t)0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 ((uint32_t)0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 ((uint32_t)0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 ((uint32_t)0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 ((uint32_t)0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 ((uint32_t)0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 ((uint32_t)0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 ((uint32_t)0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 ((uint32_t)0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 ((uint32_t)0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                ((uint32_t)0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                ((uint32_t)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((uint32_t)0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                ((uint32_t)0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                ((uint32_t)0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                ((uint32_t)0x8000)  /* Pin 15 selected */
#define GPIO_Pin_16                ((uint32_t)0x10000)  /* Pin 16 selected */
#define GPIO_Pin_17                ((uint32_t)0x20000)  /* Pin 17 selected */
#define GPIO_Pin_18                ((uint32_t)0x10000)  /* Pin 18 selected */
#define GPIO_Pin_19                ((uint32_t)0x80000)  /* Pin 19 selected */
#define GPIO_Pin_20                ((uint32_t)0x100000)  /* Pin 20 selected */
#define GPIO_Pin_21                ((uint32_t)0x200000)  /* Pin 21 selected */
#define GPIO_Pin_22                ((uint32_t)0x400000)  /* Pin 22 selected */
#define GPIO_Pin_23                ((uint32_t)0x800000)  /* Pin 23 selected */
#define GPIO_Pin_24                ((uint32_t)0x1000000)  /* Pin 24 selected */
#define GPIO_Pin_25                ((uint32_t)0x2000000)  /* Pin 25 selected */
#define GPIO_Pin_26                ((uint32_t)0x4000000)  /* Pin 26 selected */
#define GPIO_Pin_27                ((uint32_t)0x8000000)  /* Pin 27 selected */
#define GPIO_Pin_28                ((uint32_t)0x10000000)  /* Pin 29 selected */
#define GPIO_Pin_29                ((uint32_t)0x20000000)  /* Pin 30 selected */
#define GPIO_Pin_30                ((uint32_t)0x40000000)  /* Pin 31 selected */
#define GPIO_Pin_31                ((uint32_t)0x80000000)  /* Pin 32 selected */
#define GPIO_Pin_All               ((uint32_t)0xFFFFFFFF)  /* All pins selected */



typedef enum
{ 
  GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
  GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode */
	GPIO_Mode_AF   = 0x01
}GPIOMode_TypeDef;

typedef enum
{

	GPIO_PuPd_UP   = 0X00,
	GPIO_PuPd_DOWN = 0X01

}GPIOPuPd_TypeDef;

typedef struct
{
  uint32_t GPIO_Pin;

  GPIOMode_TypeDef GPIO_Mode;
	
	GPIOPuPd_TypeDef GPIO_PuPd;
	
}GPIO_InitTypeDef;

#define GPIO_PinSource0    0x00
#define GPIO_PinSource1    0x01
#define GPIO_PinSource2    0x02
#define GPIO_PinSource3    0x03
#define GPIO_PinSource4    0x04
#define GPIO_PinSource5    0x05
#define GPIO_PinSource6    0x06
#define GPIO_PinSource7    0x07
#define GPIO_PinSource8    0x08
#define GPIO_PinSource9    0x09
#define GPIO_PinSource10   0x0a
#define GPIO_PinSource11   0x0b
#define GPIO_PinSource12   0x0c
#define GPIO_PinSource13   0x0d
#define GPIO_PinSource14   0x0e
#define GPIO_PinSource15   0x0f
#define GPIO_PinSource16   0x10
#define GPIO_PinSource17   0x11
#define GPIO_PinSource18   0x12
#define GPIO_PinSource19   0x13

typedef enum
{
//0
	GPIO_AF_ADC0   = 0X00,  //PTE24 PTE25 PTA07 PTA08 PTB00 PTB01 PTB02 PTB03 PTC00 PTC01 PTC02 PTD01 PTD05 PTD06 
	GPIO_AF_ADC1   = 0X00,  //PTE00 PTE01 PTE02 PTE03 PTA17 PTB00 PTB01 PTB04 PTB05 PTB06 PTB07 PTB10 PTB11 PTC08 PTC09 PTC10 PTC11

	
//2
	GPIO_AF2_UART0 = 0x02,  //PTA01-02
	GPIO_AF_SPI0   = 0X02,  //(CS\SCK\OUT\IN) PTA14-15-16-17   PTC4/3/2/1/0-5-6-7 PTD0/4/5/6-1-2-3
	GPIO_AF_SPI1   = 0X02,  //(CS\SCK\OUT\IN) PTB10/9-11-16-17
	GPIO_AF_SPI2   = 0X02,  //(CS\SCK\OUT\IN) PTB20-21-22-23
	
//3
  GPIO_AF_UART0  = 0X03,  //PTA14-15 PTB16-17 PTD6-7
	GPIO_AF_UART1  = 0X03,  //PTC03-04 PTE00-01
	GPIO_AF_UART2  = 0X03,  //PTD02-03
	GPIO_AF_UART3  = 0X03,  //PTC16-17 PTE04-05 PTB10-11
  GPIO_AF_UART4  = 0X03,  //PTC14-15 PTE24-25
	GPIO_AF_UART5  = 0X03,  //PTD08-09 PTE08-09

	
}GPIO_AF_Typedef;


//function
void GPIO_Init(PORT_Type* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_PinAFConfig(PORT_Type* GPIOx,uint8_t GPIO_PinSource,GPIO_AF_Typedef GPIO_AF);

#endif

//*******************************************************************************	 
//������  :K10DN512VLL10  K60X256VLL10
//����    :ZPW
//�޸�����:2016/02
//�ļ�    :freescale_sim.h
//�汾    ��V0.2
//********************************************************************************

#ifndef __FREESCALE_SIM_H__
#define __FREESCALE_SIM_H__
#include "MK60D10.h"
#include "AllDefine.h"

//SCGC1
#define SIM_SCGC1Periph_UART4   ((uint32)0x00000400)
#define SIM_SCGC1Periph_UART5   ((uint32)0x00000800)

//SCGC2
#define SIM_SCGC2Periph_ENET    ((uint32)0x00000001)
#define SIM_SCGC2Periph_DAC0    ((uint32)0x00001000)
#define SIM_SCGC2Periph_DAC1    ((uint32)0x00002000)

//SCGC3
#define SIM_SCGC3Periph_RNGB     ((uint32)0x00000001)
#define SIM_SCGC3Periph_FLEXCAN1 ((uint32)0x00000010)
#define SIM_SCGC3Periph_SPI2     ((uint32)0x00001000)
#define SIM_SCGC3Periph_SDHC     ((uint32)0x00020000)
#define SIM_SCGC3Periph_FTM2     ((uint32)0x01000000)
#define SIM_SCGC3Periph_ADC1     ((uint32)0x08000000)

//SCGC4
#define SIM_SCGC4Periph_EWM      ((uint32)0x00000002)
#define SIM_SCGC4Periph_CMT      ((uint32)0x00000004)
#define SIM_SCGC4Periph_I2C0     ((uint32)0x00000040)
#define SIM_SCGC4Periph_I2C1     ((uint32)0x00000080)
#define SIM_SCGC4Periph_UART0    ((uint32)0x00000400)
#define SIM_SCGC4Periph_UART1    ((uint32)0x00000800)
#define SIM_SCGC4Periph_UART2    ((uint32)0x00001000)
#define SIM_SCGC4Periph_UART3    ((uint32)0x00002000)
#define SIM_SCGC4Periph_USBOTG   ((uint32)0x00040000)
#define SIM_SCGC4Periph_CMP      ((uint32)0x00080000)
#define SIM_SCGC4Periph_VREF     ((uint32)0x00100000)
#define SIM_SCGC4Periph_LLWU     ((uint32)0x10000000)

//SCGC5
#define SIM_SCGC5Periph_LPTIMER  ((uint32)0x000001)
#define SIM_SCGC5Periph_REGFILE  ((uint32)0x000002)
#define SIM_SCGC5Periph_TSI      ((uint32)0x000020)
#define SIM_SCGC5Periph_GPIOA    ((uint32)0x000200)
#define SIM_SCGC5Periph_GPIOB    ((uint32)0x000400)
#define SIM_SCGC5Periph_GPIOC    ((uint32)0x000800)
#define SIM_SCGC5Periph_GPIOD    ((uint32)0x001000)
#define SIM_SCGC5Periph_GPIOE    ((uint32)0x002000)

//SCGC6
#define SIM_SCGC6Periph_FTFL     ((uint32)0x00000001)
#define SIM_SCGC6Periph_DMAMUX   ((uint32)0x00000002)
#define SIM_SCGC6Periph_FLEXCAN0 ((uint32)0x00000010)
#define SIM_SCGC6Periph_SPI0     ((uint32)0x00001000)
#define SIM_SCGC6Periph_SPI1     ((uint32)0x00002000)
#define SIM_SCGC6Periph_I2S      ((uint32)0x00008000)
#define SIM_SCGC6Periph_CRC      ((uint32)0x00040000)
#define SIM_SCGC6Periph_USBDCD   ((uint32)0x00200000)
#define SIM_SCGC6Periph_PDB      ((uint32)0x00400000)
#define SIM_SCGC6Periph_PIT      ((uint32)0x00800000)
#define SIM_SCGC6Periph_FTM0     ((uint32)0x01000000)
#define SIM_SCGC6Periph_FTM1     ((uint32)0x02000000)
#define SIM_SCGC6Periph_ADC0     ((uint32)0x08000000)
#define SIM_SCGC6Periph_RTC      ((uint32)0x20000000)


//Ƶ������
#define INTERNAL_PLL   0x00
/*
    �ڲ�����
	  Core/System Clock: 96M 
		Bus         Clock: 48M
	  FlexBus     Clock: 48M
		Flash       Clock: 24M
*/
#define EXTERNAL_PLL   0x01
/*
    �ⲿ���� 8M
	  Core/System Clock: 96M 
		Bus         Clock: 48M
	  FlexBus     Clock: 48M
		Flash       Clock: 24M
*/
#define OVERCLOCK_PLL  0x02
/*
    �ⲿ���� 8M ��Ƶ
	  Core/System Clock: 200M 
		Bus         Clock: 100M
		FlexBus     Clock: 100M
	  Flash       Clock: 25M
*/

//��������Ϣ����Ƶ��Ϣ�ṹ
typedef struct
{
	uint8_t FAMID;        //Kinetisϵ�м������ͺ� ����μ� SIM->SDID�Ĵ���
	uint8_t PINID;        //������Ϣ
	uint8_t ClockOption;  //��Ƶѡ��
	uint32_t CoreClock;   //�ں�ʱ��
	uint32_t BusClock;    //����ʱ��
	uint32_t FlexBusClock;//FlexBusʱ��
	uint32_t FlashClock;  //Flashʱ��
	uint16_t FlashSize;   //Flash��С
	uint16_t RAMSize;     //RAM��С
} PInfo_Type;

extern PInfo_Type PInfo;

//function
void SIM_SCGC1PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState);
void SIM_SCGC2PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState);
void SIM_SCGC3PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState);
void SIM_SCGC4PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState);
void SIM_SCGC5PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState);
void SIM_SCGC6PeriphClockCmd(uint32 SIM_Periph,FunctionalState NewState);

void GetProcessorInfo(void);
#endif


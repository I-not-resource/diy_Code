//*******************************************************************************	 
//处理器  :K10DN512VLL10  K60X256VLL10
//作者    :ZPW
//修改日期:2016/03
//文件    :freescale_config.h
//版本    ：V0.1
//********************************************************************************

#ifndef __FREESCALE_CONFG_H__
#define __FREESCALE_CONFG_H__

#include "freescale_sim.h"
#include "freescale_gpio.h"
#include "freescale_pit.h"
#include "freescale_usart.h"
#include "freescale_adc.h"

#include <assert.h>

#ifdef  USE_FULL_ASSERT

  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif


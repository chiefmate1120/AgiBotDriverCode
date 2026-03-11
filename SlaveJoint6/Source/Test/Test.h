/**
  ******************************************************************************
  * @file    test.h
  * @author  FuZhijie
  * @version V1.8.1
  * @date    2024.04.08
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 0 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEST_H
#define __TEST_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "NuMicro.h"


typedef struct 
{
	uint32_t moment_1;
	uint32_t moment_2;
	uint32_t moment_3;	
	uint32_t moment_4;	
	uint32_t moment_5;	
	uint32_t moment_6;

	uint32_t result_1_2;
	uint32_t result_3_4;
	uint32_t result_5_6;
}TimeQuantum;

extern TimeQuantum TimeTest;    //≤‚ ‘ ±º‰






void Timex_Init(TIMER_T *Timex,uint32_t Prescale);
					
								
#endif /* __MAIN_H */


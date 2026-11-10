/**
  ******************************************************************************
  * @file    Test.c
  * @author  CopyJie
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


#include "Test.h"
TimeQuantum TimeTest; 

/**
  * @brief  Main program
  * @param  Prescale 
  * @retval None
  */
void Timex_Init(TIMER_T *Timex,uint32_t Prescale)
{
	if(Timex==TIMER1)
	{
		/* Enable time tick peripheral */
		CLK_EnableModuleClock(TMR1_MODULE);
    /* Select IP clock source */
		CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_HIRC, 0);   //12M
	}
	else if(Timex==TIMER2)
	{
		/* Enable time tick peripheral */
		CLK_EnableModuleClock(TMR2_MODULE);
    /* Select IP clock source */
		CLK_SetModuleClock(TMR2_MODULE, CLK_CLKSEL1_TMR2SEL_HIRC, 0);   //12M	
	}
	else if(Timex==TIMER3)
	{
		/* Enable time tick peripheral */
		CLK_EnableModuleClock(TMR3_MODULE);
    /* Select IP clock source */
		CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3SEL_HIRC, 0);   //12M	
	}
	 /* Selection frequency */
	  Timex->CTL |= TIMER_PERIODIC_MODE;
        TIMER_SET_PRESCALE_VALUE(Timex, Prescale);					//11  1M    //  119   100k
        TIMER_SET_CMP_VALUE(Timex, 0xFFFFFF);
	/* Start time tick */	
		TIMER_ResetCounter(Timex);  //«Â¡„	
//		TIMER_Start(Timex);
}





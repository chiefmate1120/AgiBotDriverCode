/*
 *********************************************************************************
 *     Copyright (c) 2020 ASIX Electronics Corporation All rights reserved.
 *
 *     This is unpublished proprietary source code of ASIX Electronics Corporation
 *
 *     The copyright notice above does not evidence any actual or intended
 *     publication of such source code.
 *********************************************************************************
 */

/* INCLUDE FILE DECLARATIONS */
#include <stdio.h>
#include "NuMicro.h"
#include "applinterface.h"
#include "AX58200_MotorControl.h"
#include "AX58200_FoeAppl.h"
#include "bootmode.h"
#include "coeappl.h"
#include "mc.h"
#include "spi_4671.h"
#include "task_tmc4671.h"
#include "spi_ENC2.h"
#include "spi_ENC3.h"
#include "FMC_DATA.h"
#include "led.h"
#include "SPI_Application.h"
#include "SPI_Config.h"
/* NAMING CONSTANT DECLARATIONS */
#define PLL_CLOCK					192000000	// Hz


/* GLOBAL VARIABLES DECLARATIONS */
extern UINT32	u32Devicetype;

/* LOCAL SUBPROGRAM DECLARATIONS */
void SYS_Init(void);
void BPWM(void);
void Timer_Init(void);
extern void task(void);

extern struct
{
	unsigned int count;
	unsigned char loop;
	unsigned char mode;
}led;
extern unsigned char PID_FLAG;

/* LOCAL SUBPROGRAM BODIES */
/*
 * ----------------------------------------------------------------------------
 * Function Name: int main()
 * Purpose: Main program of this firmware
 * Params:	None
 * Returns:	None, it is infinite loop
 * Note:	
 * ----------------------------------------------------------------------------
 */
unsigned int TMC4671_ID;
unsigned char ENC2_counter;
unsigned int ENC2_raw;
extern unsigned char datasave;
extern unsigned char position1st;
extern unsigned int Keyvalue;
extern unsigned int ENCValue;
int main()
{
	/* System initialization */
	SYS_Init();
    SPI_Init();

	delay_us(20000);
	delay_us(20000);
	delay_us(20000);
	PID_FLAG=2;
	task_tmc4671_init();
	/* ESC hardware initialization */
	if (HW_Init())
	{
		printf("Initialize ESC Hardware Timeout!\n");
		HW_Release();
	}
	else
	{
		/* ESC software initialization */
		MainInit();
		AX58200_FoeInit();
		CiA402_Init();
		printf("Start EtherCAT Slave Stack OK!\n");

	}
	
	while (1)
	{
		/* User code here */
		task();
		BL_JumpToLdrom();	
	}
} /* End of main() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: void SYS_Init(void)
 * Purpose: System initialization
 * Params:	None
 * Returns:	None
 * Note:
 * ----------------------------------------------------------------------------
 */
void SYS_Init(void)
{
	/* Unlock protected registers */
	SYS_UnlockReg();

	/* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
	PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

	/* Enable External XTAL (4~24 MHz) */
	CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

	/* Waiting for 12MHz clock ready */
	CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

	/* Set core clock as PLL_CLOCK from PLL */
	CLK_SetCoreClock(PLL_CLOCK);
	
	/* Set PCLK0/PCLK1 to HCLK/2 */
	CLK->PCLKDIV = (CLK_PCLKDIV_PCLK0DIV2 | CLK_PCLKDIV_PCLK1DIV2);

	

	/* Enable system tick */
	CLK_EnableSysTick(CLK_CLKSEL0_STCLKSEL_HXT, CLK_GetHXTFreq()/1000);//Set 1ms tick
	
	/* Update System Core Clock */
	/* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
	SystemCoreClockUpdate();
	
	/* Set GPA multi-function pins for UART0 RXD(GPA.6) and TXD(GPA.7) */
	SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA6MFP_Msk | SYS_GPA_MFPL_PA7MFP_Msk);
	SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA6MFP_UART0_RXD | SYS_GPA_MFPL_PA7MFP_UART0_TXD);
	
	/* Select PCLK0 as the clock source of SPI0 */
    CLK_SetModuleClock(SPI0_MODULE, CLK_CLKSEL2_SPI0SEL_PCLK1, MODULE_NoMsk);
	
	/* Enable SPI0 peripheral clock */
    CLK_EnableModuleClock(SPI0_MODULE);
	
	/* Lock protected registers */
	SYS_LockReg();
	
	/* Init UART to 115200-8n1 for print message */
	/* Enable IP clock */
//	CLK_EnableModuleClock(UART0_MODULE);
//	
//	/* Select IP clock source */
//	CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));
//	UART_Open(UART0, 115200);
//	
//	UART_EnableInt(UART0, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RLSIEN_Msk  ));          //允许接收中断 允许线中断
//    UART0->FIFO |= UART_FIFO_RFITL_1BYTE;//;;  UART_FIFO_RFITL_1BYTE
//	NVIC_EnableIRQ(UART0_IRQn);
//    NVIC_SetPriority(UART0_IRQn, 1); //中断优先级
	
} /* End of SYS_Init() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: void SysTick_Handler(void)
 * Purpose: 
 * Params:	None
 * Returns:	None
 * Note:
 * ----------------------------------------------------------------------------
 */
void SysTick_Handler(void)
{
	//MC_Run();
	
} /* End of SysTick_Handler() */

void BPWM(void)
{
	/* Enable BPWM0 clock source */
    CLK_EnableModuleClock(BPWM0_MODULE);
    /* Select BPWM module clock source */
    CLK_SetModuleClock(BPWM0_MODULE, CLK_CLKSEL2_BPWM0SEL_PLL, 0);
    SYS_ResetModule(BPWM0_MODULE);
    /* Set PG.10 multi-function pin for BPWM0 channel 4 */
    SYS->GPA_MFPL &= ~SYS_GPA_MFPL_PA4MFP_Msk;
    SYS->GPA_MFPL |= SYS_GPA_MFPL_PA4MFP_BPWM0_CH4;
	SYS->GPA_MFPL &= ~SYS_GPA_MFPL_PA0MFP_Msk;
    SYS->GPA_MFPL |= SYS_GPA_MFPL_PA0MFP_BPWM0_CH0;
	BPWM_ConfigOutputChannel(BPWM0, 4, 28000, 0);
	BPWM_ConfigOutputChannel(BPWM0, 0, 28000, 0);
    /* Enable output of BPWM0 channel 4 */
    BPWM_EnableOutput(BPWM0, BPWM_CH_4_MASK|BPWM_CH_0_MASK);
    /* Start BPWM counter */
    BPWM_Start(BPWM0, BPWM_CH_4_MASK|BPWM_CH_0_MASK);
    /* Enable output of BPWM0 channel 4 */
    BPWM_EnableOutput(BPWM0, BPWM_CH_4_MASK|BPWM_CH_0_MASK);
    /* Start BPWM counter */
    BPWM_Start(BPWM0, BPWM_CH_4_MASK|BPWM_CH_0_MASK);
}

void TIMER0_Init()
{

    /* Set timer0 periodic time-out period is 3us if timer clock is 12 MHz */
    TIMER_SET_CMP_VALUE(TIMER0, 36);//TIMER0->CMP = 36;

    /* Start timer counter in periodic mode and enable timer interrupt trigger EADC */
    TIMER0->CTL = TIMER_PERIODIC_MODE;
    TIMER0->TRGCTL |= TIMER_TRGCTL_TRGEADC_Msk;

}

void Timer_Init(void)
{ 
	/* Enable IP clock */
    CLK_EnableModuleClock(TMR0_MODULE);
    /* Select IP clock source */
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HIRC, 0);
	TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1000); 
	/*如果要修改预分频和比较寄存器的值可以用下面两个宏*/ //TIMER_SET_CMP_VALUE(TIMER0, 0xFFFFFF);
	//修改比较寄存器的值
	 //TIMER_SET_PRESCALE_VALUE(TIMER0, 0x0);
	//修改预分频的值 /* 使能Timer0中断 */ 
	
	TIMER_EnableInt(TIMER0); 
	NVIC_EnableIRQ(TMR0_IRQn); 
	// 启动 Timer 0 
	TIMER_Start(TIMER0); 
}

 void TMR0_IRQHandler(void) 
{   
//	task();
	led.count++;
	brake.counter++;
	brake2.counter++;
	TIMER_ClearIntFlag(TIMER0);
}
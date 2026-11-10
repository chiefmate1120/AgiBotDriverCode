#include "task_tmc4671.h"
#include "usart.h"
#include "AX58200_Hw.h"
#include "spi_4671.h"
#include "AX58200_MotorControl.h"
#include "led.h"

extern void BPWM(void);

struct
{
	unsigned int count;
	unsigned char loop;
	unsigned char mode;
}led;

void task_led(void)
{
	switch (led.mode)
	{
		case 0://off
		{
//			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 100);//G
//			BPWM_ConfigOutputChannel(BPWM1, 2,15000, 100);//R
//			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 100);//B
//			BPWM_DisableOutput(BPWM1, BPWM_CH_0_MASK);
//			BPWM_DisableOutput(BPWM1, BPWM_CH_2_MASK);
//			BPWM_DisableOutput(BPWM1, BPWM_CH_3_MASK);
			
			led.loop=0;
			break;
		}
		case 1://blue
		{
			BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
			BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
			BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
			
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
			BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 50);
			led.loop=0;
			break;
		}
		case 2://blue flash
		{
			switch (led.loop)
			{
				case 0:
				{
					led.count=0;
					led.loop=2;
					BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
					break;
				}
				case 1:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 50);
						led.count=0;
						led.loop=2;
					}
					break;
				}
				case 2:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						led.count=0;
						led.loop=1;
					}
					break;
				}
				default: led.loop=0;break;
			}
			break;
		}
		case 3://yellow flash
		{
			switch (led.loop)
			{
				case 0:
				{
					led.count=0;
					led.loop=2;
					BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
					break;
				}
				case 1:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 50);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 50);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						led.count=0;
						led.loop=2;
					}
					break;
				}
				case 2:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						led.count=0;
						led.loop=1;
					}
					break;
				}
				default: led.loop=0;break;
			}
			break;
		}
		case 4://red flash
		{
			switch (led.loop)
			{
				case 0:
				{
					led.count=0;
					led.loop=2;
					BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
					break;
				}
				case 1:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 50);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						led.count=0;
						led.loop=2;
					}
					break;
				}
				case 2:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						led.count=0;
						led.loop=1;
					}
					break;
				}
				default: led.loop=0;break;
			}
			break;
		}
		case 5://green
		{
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 60);
			BPWM_ConfigOutputChannel(BPWM1, 2,15000, 2);
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 2);
			led.loop=0;
			break;
		}
		case 6://green flash
		{
			switch (led.loop)
			{
				case 0:
				{
					led.count=0;
					led.loop=2;
					BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
					break;
				}
				case 1:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 50);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 1);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 1);
						led.count=0;
						led.loop=2;
					}
					break;
				}
				case 2:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						led.count=0;
						led.loop=1;
					}
					break;
				}
				default: led.loop=0;break;
			}
			break;
		}
		case 7://red
		{
			BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
			BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
			BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 1);
			BPWM_ConfigOutputChannel(BPWM1, 2,15000, 50);
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 1);
			led.loop=0;
			break;
		}
		case 8://green
		{
			BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
			BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
			BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 50);
			BPWM_ConfigOutputChannel(BPWM1, 2,15000, 50);
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
			led.loop=0;
			break;
		}
		case 9://green flash
		{
			switch (led.loop)
			{
				case 0:
				{
					led.count=0;
					led.loop=1;
					BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
					break;
				}
				case 1:
				{
					if(led.count>100)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 2);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 2);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 65);
						led.count=0;
						led.loop=2;
					}
					break;
				}
				case 2:
				{
					if(led.count>100)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						led.count=0;
						led.loop=3;
					}
					break;
				}
				case 3:
				{
					if(led.count>100)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 50);
						led.count=0;
						led.loop=4;
					}
					break;
				}
				case 4:
				{
					if(led.count>100)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						led.count=0;
						led.loop=5;
					}
					break;
				}
				case 5:
				{
					if(led.count>800)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 50);
						led.count=0;
						led.loop=1;
					}
					break;
				}
				default: led.loop=0;break;
			}
			break;
		}
		case 10://green
		{
			BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
			BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
			BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 10);
			BPWM_ConfigOutputChannel(BPWM1, 2,15000, 80);
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
			led.loop=0;
			break;
		}
		case 11://red flash
		{
			switch (led.loop)
			{
				case 0:
				{
					led.count=0;
					led.loop=2;
					BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_2_MASK);
					BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
					break;
				}
				case 1:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 10);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 80);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						led.count=0;
						led.loop=2;
					}
					break;
				}
				case 2:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 2,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						led.count=0;
						led.loop=1;
					}
					break;
				}
				default: led.loop=0;break;
			}
			break;
		}
		default: led.mode=0;break;
	}
}

void PWM_ON(void)
{
	static unsigned char loop=0;
	switch(loop)
	{
		case 0:
		{
			if(led.mode==0)
			{
				HW_MultiFuncPins(PB, BIT_8, 0);		
				GPIO_SetMode(PB, BIT_8, GPIO_MODE_OUTPUT);
				GPIO_SetSlewCtl(PB, BIT_8, GPIO_SLEWCTL_FAST);
				GPIO_SetPullCtl(PB, BIT_8, GPIO_PUSEL_PULL_UP);
				
				HW_MultiFuncPins(PB, BIT_9, 0);		
				GPIO_SetMode(PB, BIT_9, GPIO_MODE_OUTPUT);
				GPIO_SetSlewCtl(PB, BIT_9, GPIO_SLEWCTL_FAST);
				GPIO_SetPullCtl(PB, BIT_9, GPIO_PUSEL_PULL_UP);
				
				HW_MultiFuncPins(PB, BIT_11, 0);		
				GPIO_SetMode(PB, BIT_11, GPIO_MODE_OUTPUT);
				GPIO_SetSlewCtl(PB, BIT_11, GPIO_SLEWCTL_FAST);
				GPIO_SetPullCtl(PB, BIT_11, GPIO_PUSEL_PULL_UP);
				
				HW_GPIO_WritePin(PB,BIT_8, 0);
				HW_GPIO_WritePin(PB,BIT_9, 0);
				HW_GPIO_WritePin(PB,BIT_11, 0);
			}
			else
			{
				BPWM();
				loop=1;
			}
			break;
		}
		case 1:
		{
			if(led.mode==0)
			{
				loop=0;
			}
			break;
		}
		default:loop=0;
	}
}
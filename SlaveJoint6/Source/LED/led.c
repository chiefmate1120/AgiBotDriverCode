#include "task_tmc4671.h"
#include "usart.h"
#include "AX58200_Hw.h"
#include "spi_4671.h"
#include "AX58200_MotorControl.h"
#include "led.h"

struct
{
	unsigned int count;
	unsigned char loop;
	unsigned char mode;
}led;

void LED_BPWM(void)
{
	
	/* Enable BPWM0 clock source */
    CLK_EnableModuleClock(BPWM1_MODULE);

    /* Select BPWM module clock source */
    CLK_SetModuleClock(BPWM1_MODULE, CLK_CLKSEL2_BPWM1SEL_PLL, 0);

    SYS_ResetModule(BPWM1_MODULE);


    /* Set PG.10 multi-function pin for BPWM0 channel 4 */
    SYS->GPB_MFPL &= ~SYS_GPB_MFPL_PB7MFP_Msk;
    SYS->GPB_MFPL|= SYS_GPB_MFPL_PB7MFP_BPWM1_CH4;
	
	SYS->GPB_MFPH &= ~SYS_GPB_MFPH_PB8MFP_Msk;
    SYS->GPB_MFPH|= SYS_GPB_MFPH_PB8MFP_BPWM1_CH3;
	
	SYS->GPB_MFPH &= ~SYS_GPB_MFPH_PB11MFP_Msk;
    SYS->GPB_MFPH|= SYS_GPB_MFPH_PB11MFP_BPWM1_CH0;

    /* Enable output of BPWM0 channel 4 */
    BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);
	BPWM_EnableOutput(BPWM1, BPWM_CH_3_MASK);
	BPWM_EnableOutput(BPWM1, BPWM_CH_4_MASK);

    /* Start BPWM counter */
    BPWM_Start(BPWM1, BPWM_CH_0_MASK);
	BPWM_Start(BPWM1, BPWM_CH_3_MASK);
	BPWM_Start(BPWM1, BPWM_CH_4_MASK);
}

void task_led(void)
{
	switch (led.mode)
	{
		case 0://off
		{
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
			BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
			led.loop=0;
			break;
		}
		case 1://blue
		{
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);//R
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);//G
			BPWM_ConfigOutputChannel(BPWM1, 4,15000, 100);//B
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
					break;
				}
				case 1:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
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
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 100);
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
					break;
				}
				case 1:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
						led.count=0;
						led.loop=2;
					}
					break;
				}
				case 2:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 100);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 100);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
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
					break;
				}
				case 1:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
						led.count=0;
						led.loop=2;
					}
					break;
				}
				case 2:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 100);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);//G
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
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
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 100);
			BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
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
					break;
				}
				case 1:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 100);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
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
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
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
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 100);
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
			BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
			led.loop=0;
			break;
		}
		case 8://green
		{
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 100);
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 100);
			BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
			led.loop=0;
			break;
		}
		case 9://blue flash
		{
			switch (led.loop)
			{
				case 0:
				{
					led.count=0;
					led.loop=1;
					break;
				}
				case 1:
				{
					if(led.count>100)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 100);
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
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
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
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 100);
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
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
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
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 100);
						led.count=0;
						led.loop=1;
					}
					break;
				}
				default: led.loop=0;break;
			}
			break;
		}
		case 10://blue
		{
			BPWM_ConfigOutputChannel(BPWM1, 0,15000, 80);//R
			BPWM_ConfigOutputChannel(BPWM1, 3,15000, 10);//G
			BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);//B
			led.loop=0;
			break;
		}
		case 11://yellow flash
		{
			switch (led.loop)
			{
				case 0:
				{
					led.count=0;
					led.loop=2;
					break;
				}
				case 1:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 0);
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);
						led.count=0;
						led.loop=2;
					}
					break;
				}
				case 2:
				{
					if(led.count>200)
					{
						BPWM_ConfigOutputChannel(BPWM1, 0,15000, 80);//R
						BPWM_ConfigOutputChannel(BPWM1, 3,15000, 10);//G
						BPWM_ConfigOutputChannel(BPWM1, 4,15000, 0);//B
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
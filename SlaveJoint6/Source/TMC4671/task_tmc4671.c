#include "task_tmc4671.h"
#include "usart.h"
#include "AX58200_Hw.h"
#include "spi_4671.h"
#include "TMC4671.h"
#include "FMC_DATA.h"
#include "AX58200_MotorControl.h"

INT16 abn_offset;
short ABZ_HALL_offset;

unsigned char currentmode;
unsigned int readback;
unsigned int state;
unsigned int speed;
int temp_current;
int targetspeed;

int Set_speed;
int Set_position;
int Current_FB;
unsigned char Position_PID;
struct
{
	unsigned char loop;
	unsigned char flag;
	unsigned int counter;
}breakout;
HeartbeatClass Heartbeat; 
PhaseCurrentClass PhaseMissing;

void delay_us(unsigned int us)
{
//	unsigned int i,j;
//	for(i=us;i>0;i--)
//	{
//		for(j=50;j>0;j--);
//	}
    CLK_SysTickDelay(us);
}

void delay_ms(unsigned int ms)
{
	unsigned int i;
	for(i=0;i>1000;i++)
	{
		CLK_SysTickDelay(ms);
	}    
}

struct
{
	UINT16 Current_P;
	UINT16 Current_I;
	UINT16 Velocity_P;
	UINT16 Velocity_I;
	UINT16 Position_P;
	UINT16 Position_I;     
}PID4671;

void set_4671PID(void)
{
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_P_TORQUE_I, PID4671.Current_P<<16|PID4671.Current_I);
	tmc4671_writeInt(0, TMC4671_PID_FLUX_P_FLUX_I, PID4671.Current_P<<16|PID4671.Current_I);
	
	tmc4671_writeInt(0, TMC4671_PID_VELOCITY_P_VELOCITY_I, PID4671.Velocity_P<<16|PID4671.Velocity_I);       //速度环PI，P=2000，I=80
	tmc4671_writeInt(0, TMC4671_PID_POSITION_P_POSITION_I, PID4671.Position_P<<16|0);
}

void task_tmc4671_init(void)
{	
	int temp;
	unsigned char count=0;
	PID4671.Current_P=3500;
	PID4671.Current_I=10000;
	PID4671.Velocity_P=3500;
	PID4671.Velocity_I=100;
	PID4671.Position_P=1500;
	PID4671.Position_I=0;    
//	FMC_PID_Read();
	PID0x6090.PIDCurrentP=PID4671.Current_P;
	PID0x6090.PIDCurrentI=PID4671.Current_I;
	PID0x6090.PIDVelocityP=PID4671.Velocity_P;
	PID0x6090.PIDVelocityI=PID4671.Velocity_I;
	PID0x6090.PIDPositionP=PID4671.Position_P;
	// Motor type &  PWM configuration
	tmc4671_writeInt(0, TMC4671_MOTOR_TYPE_N_POLE_PAIRS, 0x00030008);
	tmc4671_writeInt(0, TMC4671_PWM_POLARITIES, 0x00000000);
	tmc4671_writeInt(0, TMC4671_PWM_MAXCNT, 0x00000F9F);
	tmc4671_writeInt(0, TMC4671_PWM_BBM_H_BBM_L, 0x00001919);
	tmc4671_writeInt(0, TMC4671_PWM_SV_CHOP, 0x00000007);

	// ADC configuration
	tmc4671_writeInt(0, TMC4671_ADC_I_SELECT, 0x24000100);
	tmc4671_writeInt(0, TMC4671_dsADC_MCFG_B_MCFG_A, 0x00100010);
	tmc4671_writeInt(0, TMC4671_dsADC_MCLK_A, 0x20000000);
	tmc4671_writeInt(0, TMC4671_dsADC_MCLK_B, 0x00000000);
	tmc4671_writeInt(0, TMC4671_dsADC_MDEC_B_MDEC_A, 0x014E014E);
	tmc4671_writeInt(0, TMC4671_ADC_I0_SCALE_OFFSET, 0xFF868309);           //FE77
	tmc4671_writeInt(0, TMC4671_ADC_I1_SCALE_OFFSET, 0xFF868193);

	// Digital hall settings
	tmc4671_writeInt(0, TMC4671_HALL_MODE, 0x00000000);
	tmc4671_writeInt(0, TMC4671_HALL_PHI_E_PHI_M_OFFSET, 0xC3740000);


	// ABN encoder settings
	tmc4671_writeInt(0, TMC4671_ABN_DECODER_MODE, 0x00001000);
	tmc4671_writeInt(0, TMC4671_ABN_DECODER_PPR, 10016*4);           //0x00009C40
	tmc4671_writeInt(0, TMC4671_ABN_DECODER_COUNT, 0x00000FE0);
	tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x6BAE0000);

	set_4671PID();
	
	// Limits
	tmc4671_writeInt(0, TMC4671_PIDOUT_UQ_UD_LIMITS, 30000);
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_LIMITS, 6000);
	tmc4671_writeInt(0, TMC4671_PID_ACCELERATION_LIMIT, 1500);
	tmc4671_writeInt(0, TMC4671_PID_VELOCITY_LIMIT, 1500);
	tmc4671_writeInt(0, TMC4671_PID_POSITION_LIMIT_LOW, 0x80000001);
	tmc4671_writeInt(0, TMC4671_PID_POSITION_LIMIT_HIGH, 0x7FFFFFFF);

	// Init encoder (mode 0)
	
	tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);
	tmc4671_writeInt(0, TMC4671_PHI_E_SELECTION, 0x00000001);
	tmc4671_writeInt(0, TMC4671_PHI_E_EXT, 0x00000000);
	tmc4671_writeInt(0, TMC4671_UQ_UD_EXT, 0x000007D0);
	delay_us(1000);
	tmc4671_writeInt(0, TMC4671_ABN_DECODER_COUNT, 0x00000000);

	// Feedback selection
	tmc4671_writeInt(0, TMC4671_PHI_E_SELECTION, 0x00000005);
	tmc4671_writeInt(0, TMC4671_VELOCITY_SELECTION, 0x00000009);
	tmc4671_writeInt(0, TMC4671_POSITION_SELECTION, 0x00000009);	
	
	stop=1;
	
	abn_offset=tmc4671_getHALLPHI();
	ABZ_HALL_offset=abn_offset;
	tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, abn_offset<<16);
	tmc4671_writeInt(0, TMC4671_PHI_E_SELECTION, 0x00000003);
	tmc4671_setActualPosition(0, 0);
}

void ramp(void)
{
	int currentspeed;
	currentspeed=tmc4671_getActualVelocity(0);
	if(currentspeed<=200)
	{
		tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_TARGET, 0x00000000);
		stop=0;
		show_speed=0;
		DIS;
	}
	else
	{
		currentspeed-=currentspeed/50;
		tmc4671_setTargetVelocity(0, currentspeed);
	}
}

void _4671_test(void)
{
	unsigned int position;
	unsigned char position_start;

	if(stop)
	{
		breakout.flag=0;
        stop=0;
        if(breakout.loop==0)
        {
            DIS;
            Set_ADCOFFSET();
        }
    }
}

void task_break(void)
{
	switch(breakout.loop)
	{
		case 0:
		{
			breakout.counter=0;
			if(breakout.flag)
			{
				BPWM_ConfigOutputChannel(BPWM0, 0, 28000, 100);
				breakout.loop=1;
			}
			break;
		}
		case 1:
		{
			if(breakout.counter>400)
			{
				if(currentmode)
				{
					BPWM_ConfigOutputChannel(BPWM0, 0,28000, 40);
				}
				else
				{
					BPWM_ConfigOutputChannel(BPWM0, 0,28000, 40);
				}
				breakout.loop=2;
			}
			if(!breakout.flag)
			{
				BPWM_ConfigOutputChannel(BPWM0, 0, 28000, 0);
				breakout.loop=3;
				breakout.counter=0;
			}
			break;
		}
		case 2:
		{
			if(!breakout.flag)
			{
				BPWM_ConfigOutputChannel(BPWM0, 0, 28000, 0);
				breakout.loop=3;
				breakout.counter=0;
			}
			break;
		}
		case 3:
		{
			if(breakout.counter>200)
			{
				breakout.loop=0;
			}
			break;
		}        
		default: break;
	}
}

void Set_ADCOFFSET(void)
{
	unsigned int temp,I0_temp,I1_temp;
	temp=tmc4671_readInt(0, 0x02);
	I0_temp=temp&0xFFFF;
	I1_temp=(temp&0xFFFF0000)>>16;
	tmc4671_writeInt(0, TMC4671_ADC_I0_SCALE_OFFSET, 0xFF860000+I0_temp);
	tmc4671_writeInt(0, TMC4671_ADC_I1_SCALE_OFFSET, 0xFF860000+I1_temp);
}







//PD 位置式PD控制器
struct
{
	float err;			//当前偏差
	float Kp;     		//比例系数
	float Kd;     		//微分系数
	
	float Uk;			  //Uk
	float umax;
	float umin;
}
PD_PARAMETER;

void PD_Init(void)
{
	PD_PARAMETER.err=0;
	PD_PARAMETER.Kp=1;//1
	PD_PARAMETER.Kd=100;//100;

	PD_PARAMETER.Uk=0;
	PD_PARAMETER.umax=15000;
	PD_PARAMETER.umin=-15000;
}

float PID_controller(float set_position, float actual_position, float actual_speed)	 //位置式PD
{
	PD_PARAMETER.err= set_position-actual_position;

 	PD_PARAMETER.Uk= PD_PARAMETER.Kp*PD_PARAMETER.err-\
			 PD_PARAMETER.Kd*actual_speed;


	//PID.Udk *=0.2;//0.5;//0.1

	if(PD_PARAMETER.Uk>PD_PARAMETER.umax)
	{
		PD_PARAMETER.Uk = PD_PARAMETER.umax;
	}
	else if(PD_PARAMETER.Uk<PD_PARAMETER.umin)
		 {
			 PD_PARAMETER.Uk = PD_PARAMETER.umin;
		 }
		 
	return PD_PARAMETER.Uk;
}

 

/*

PE4 ->  KEY4
PA4 ->  KEY5

PB7 ->  LED_R
PB8 ->  LED_G
PB11 ->  LED_B

PC5 -> DRV8350_Fault

*/

void Key_init(void)
{
/***************************************************/  
    //KEY1
 	HW_MultiFuncPins(PB, BIT_7, 0);		
	GPIO_SetMode(PB, BIT_7, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PB, BIT_7, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PB, BIT_7, GPIO_PUSEL_PULL_UP);      
    //KEY2
 	HW_MultiFuncPins(PB, BIT_8, 0);		
	GPIO_SetMode(PB, BIT_8, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PB, BIT_8, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PB, BIT_8, GPIO_PUSEL_PULL_UP);       
    //KEY3
 	HW_MultiFuncPins(PB, BIT_11, 0);		
	GPIO_SetMode(PB, BIT_11, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PB, BIT_11, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PB, BIT_11, GPIO_PUSEL_PULL_UP);          
 /***************************************************/ 
    //FAULT
 	HW_MultiFuncPins(PC, BIT_4, 0);		
	GPIO_SetMode(PC, BIT_4, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PC, BIT_4, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_4, GPIO_PUSEL_PULL_UP);    
}

unsigned int Task_Key(void)
{
	unsigned int temp=0;
    //低电平报错
	if(HW_GPIO_ReadPin(PC,BIT_4))
	{
		temp&=~(1<<31);
        ErrorMessage.MotorDriveError=0;        
	}
	else
	{
		temp|=(1<<31);
        ErrorMessage.MotorDriveError=(1<<31);         
	}
    
    //按键输入
    //KEY1
//	if(HW_GPIO_ReadPin(PB,BIT_7))
//	{
//		temp&=~(1<<0);
//	}
//	else
//	{
//		temp|=(1<<0);
//	}
   //KEY2
	if(HW_GPIO_ReadPin(PB,BIT_8))
	{
		temp&=~(1<<1);
	}
	else
	{
		temp|=(1<<1);
	}
   //KEY3
	if(HW_GPIO_ReadPin(PB,BIT_11))
	{
		temp&=~(1<<2);
	}
	else
	{
		temp|=(1<<2);
	}    
	return temp;
}



// 定义求平均值的函数
int average(int arr[], int n) 
{
    int sum = 0;
    for (int i = 0; i < n; i++) {
    sum += arr[i];
    }
    return (int)sum / n;
}
// 定义求平均值的函数
int sum(int a[], int n) 
{
    int s = 0;
    for (int i = 0; i < n; i++) 
    {
        s += a[i];
    }
    return s;
}
// 求最大值的函数
int max_of_three(int a, int b, int c) 
{
    int max = a;
    if (b > max) 
    {
        max = b;
    }
    if (c > max) 
    {
        max = c;
    }
    return max;
}
// 求最小值的函数
int min_of_three(int a, int b, int c) 
{
    int min = a;
    if (b < min) {
        min = b;
    }
    if (c < min) {
        min = c;
    }
    return min;
}

void PhaseMissingDetection()
{
    int i=0;
    if(PhaseMissing.CalculateFlag)
    {
        PhaseMissing.PhaseCurrentSum_U = sum(PhaseMissing.PhaseCurrentGroup_U,sizeof(PhaseMissing.PhaseCurrentGroup_U)/sizeof(PhaseMissing.PhaseCurrentGroup_U[0]));
        PhaseMissing.PhaseCurrentSum_V = sum(PhaseMissing.PhaseCurrentGroup_V,sizeof(PhaseMissing.PhaseCurrentGroup_V)/sizeof(PhaseMissing.PhaseCurrentGroup_V[0]));
        PhaseMissing.PhaseCurrentSum_W = sum(PhaseMissing.PhaseCurrentGroup_W,sizeof(PhaseMissing.PhaseCurrentGroup_W)/sizeof(PhaseMissing.PhaseCurrentGroup_W[0]));
        PhaseMissing.Result_MAX = max_of_three(PhaseMissing.PhaseCurrentSum_U,PhaseMissing.PhaseCurrentSum_V,PhaseMissing.PhaseCurrentSum_W);
        PhaseMissing.Result_MIN = min_of_three(PhaseMissing.PhaseCurrentSum_U,PhaseMissing.PhaseCurrentSum_V,PhaseMissing.PhaseCurrentSum_W);        
        PhaseMissing.Result = PhaseMissing.Result_MIN/PhaseMissing.Result_MAX *100;
        PhaseMissing.CalculateFlag=0;
        PhaseMissing.StartFlag =1;
        PhaseMissing.ResultFlag =1;
    }    
}


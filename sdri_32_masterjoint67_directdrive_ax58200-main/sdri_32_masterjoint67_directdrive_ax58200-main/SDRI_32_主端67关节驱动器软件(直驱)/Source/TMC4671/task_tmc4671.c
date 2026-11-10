#include "task_tmc4671.h"
#include "usart.h"
#include "AX58200_Hw.h"
#include "spi_4671.h"
#include "TMC4671.h"
#include "FMC_DATA.h"
#include "AX58200_MotorControl.h"

#define PID_TORQUE_P  0x56
#define PID_TORQUE_I  0x57
#define PID_VELOCITY_P  0x58
#define PID_VELOCITY_I  0x59
#define PID_POSITION_P  0x5a
#define PID_POSITION_I  0x5b

extern unsigned char slave;
extern unsigned char PID_FLAG;
unsigned int readback;
unsigned int state;
unsigned int speed;
int temp_current;
int targetspeed;

int Set_speed;
int Set_position;
int Current_FB;
unsigned char Position_PID;

PIDClass PID_Motor1;
PIDClass PID_Motor2;

BrakeClass brake;
BrakeClass brake2;

void delay_us(unsigned int us)
{
	unsigned int i,j;
	for(i=us;i>0;i--)
	{
		for(j=192;j>0;j--);
	}
}





void set_4671PID(void)
{
	if(slave==1)
	{
		tmc4671_write(TMC4671_PID_TORQUE_P_TORQUE_I, PID_Motor1.Current_P<<16|PID_Motor1.Current_I);
		tmc4671_write(TMC4671_PID_FLUX_P_FLUX_I, PID_Motor1.Current_P<<16|PID_Motor1.Current_I);
		tmc4671_write(TMC4671_PID_VELOCITY_P_VELOCITY_I, PID_Motor1.Velocity_P<<16|PID_Motor1.Velocity_I);       //速度环PI，P=2000，I=80
		tmc4671_write(TMC4671_PID_POSITION_P_POSITION_I, PID_Motor1.Position_P<<16|PID_Motor1.Position_I);
	}
	else if(slave==2)
	{
		tmc4671_write(TMC4671_PID_TORQUE_P_TORQUE_I, PID_Motor2.Current_P<<16|PID_Motor2.Current_I);
		tmc4671_write(TMC4671_PID_FLUX_P_FLUX_I, PID_Motor2.Current_P<<16|PID_Motor2.Current_I);
		tmc4671_write(TMC4671_PID_VELOCITY_P_VELOCITY_I, PID_Motor2.Velocity_P<<16|PID_Motor2.Velocity_I);       //速度环PI，P=2000，I=80
		tmc4671_write(TMC4671_PID_POSITION_P_POSITION_I, PID_Motor2.Position_P<<16|PID_Motor2.Position_I);
	}
}

void task_tmc4671_init(void)
{	
	PID_Motor1.Current_P=15000;
	PID_Motor1.Current_I=2000;
	PID_Motor1.Velocity_P=20000;
	PID_Motor1.Velocity_I=300;
	PID_Motor1.Position_P=1500;
	PID_Motor1.Position_I=3000;
    
	PID_Motor2.Current_P=4000;
	PID_Motor2.Current_I=1000;
	PID_Motor2.Velocity_P=20000;
	PID_Motor2.Velocity_I=300;
	PID_Motor2.Position_P=2000;
	PID_Motor2.Position_I=3000;
	
//	FMC_PID_Read();
	PID0x6090.PIDCurrentP=PID_Motor1.Current_P;
	PID0x6090.PIDCurrentI=PID_Motor1.Current_I;
	PID0x6090.PIDVelocityP=PID_Motor1.Velocity_P;
	PID0x6090.PIDVelocityI=PID_Motor1.Velocity_I;
	PID0x6090.PIDPositionP=PID_Motor1.Position_P;
	PID0x6090.PIDPositionI=PID_Motor1.Position_I;
	
	PID0x6091.PIDCurrentP=PID_Motor2.Current_P;
	PID0x6091.PIDCurrentI=PID_Motor2.Current_I;
	PID0x6091.PIDVelocityP=PID_Motor2.Velocity_P;
	PID0x6091.PIDVelocityI=PID_Motor2.Velocity_I;
	PID0x6091.PIDPositionP=PID_Motor2.Position_P;
	PID0x6091.PIDPositionI=PID_Motor2.Position_I;
}

void ramp(void)
{

}

void _4671_test(void)
{
	if(stop)
	{
		brake.flag=0;
		stop=0;
		DIS;
	}
}

void task_brake(void)
{
	switch(brake.loop)
	{
		case 0:
		{
			brake.counter=0;
			if(brake.flag)
			{
				BPWM_ConfigOutputChannel(BPWM0, 4,28000, 100);
				brake.loop=1;
			}
			break;
		}
		case 1:
		{
			if(brake.counter>400)
			{
				BPWM_ConfigOutputChannel(BPWM0, 4,28000, 30);
				brake.loop=2;
			}
			if(!brake.flag)
			{
				BPWM_ConfigOutputChannel(BPWM0, 4,28000, 0);
				brake.loop=0;
			}
			break;
		}
		case 2:
		{
			if(!brake.flag)
			{
				BPWM_ConfigOutputChannel(BPWM0, 4,28000, 0);
				brake.loop=0;
			}
			break;
		}
		default: break;
	}
}

void task_brake2(void)
{
	switch(brake2.loop)
	{
		case 0:
		{
			brake2.counter=0;
			if(brake2.flag)
			{
				BPWM_ConfigOutputChannel(BPWM0, 0,28000, 100);
				brake2.loop=1;
			}
			break;
		}
		case 1:
		{
			if(brake2.counter>400)
			{
				BPWM_ConfigOutputChannel(BPWM0, 0,28000, 30);
				brake2.loop=2;
			}
			if(!brake2.flag)
			{
				BPWM_ConfigOutputChannel(BPWM0, 0,28000, 0);
				brake2.loop=0;
			}
			break;
		}
		case 2:
		{
			if(!brake2.flag)
			{
				BPWM_ConfigOutputChannel(BPWM0, 0,28000, 0);
				brake2.loop=0;
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
	tmc4671_writeInt(0, TMC4671_ADC_I0_SCALE_OFFSET, 0xFF970000+I0_temp);
	tmc4671_writeInt(0, TMC4671_ADC_I1_SCALE_OFFSET, 0xFF970000+I1_temp);
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

void Key_init(void)
{
	HW_MultiFuncPins(PA, BIT_0, 0);		
	GPIO_SetMode(PA, BIT_0, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PA, BIT_0, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PA, BIT_0, GPIO_PUSEL_PULL_UP);
	
	HW_MultiFuncPins(PH, BIT_9, 0);		
	GPIO_SetMode(PH, BIT_9, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PH, BIT_9, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PH, BIT_9, GPIO_PUSEL_PULL_UP);
	
	HW_MultiFuncPins(PC, BIT_4, 0);		
	GPIO_SetMode(PC, BIT_4, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PC, BIT_4, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_4, GPIO_PUSEL_PULL_UP);
	
	HW_MultiFuncPins(PC, BIT_5, 0);		
	GPIO_SetMode(PC, BIT_5, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PC, BIT_5, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_5, GPIO_PUSEL_PULL_UP);
	
	HW_MultiFuncPins(PC, BIT_14, 0);		
	GPIO_SetMode(PC, BIT_14, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PC, BIT_14, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_14, GPIO_PUSEL_PULL_UP);
	
	
	
	HW_MultiFuncPins(PB, BIT_7, 0);		
	GPIO_SetMode(PB, BIT_7, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PB, BIT_7, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PB, BIT_7, GPIO_PUSEL_PULL_UP);
	
	HW_MultiFuncPins(PB, BIT_3, 0);		
	GPIO_SetMode(PB, BIT_3, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PB, BIT_3, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PB, BIT_3, GPIO_PUSEL_PULL_UP);
	
}

unsigned int Task_Key(void)
{
	static unsigned int temp=0,temp1=0,count,count1;
	static unsigned char CountPH9=0,TempPH9=0;
	static unsigned char CountPC14=0,TempPC14=0;
	static unsigned char CountPB3=0,TempPB3=0;
	static unsigned char CountPC4=0,TempPC4=0;
	
	static unsigned char loop,num=0;
	
//	TempPH9+=!HW_GPIO_ReadPin(PH,BIT_9);
//	CountPH9++;
//	if(CountPH9==20)
//	{
//		if(TempPH9>18)
//		{
//			temp1|=(1<<2);
//		}
//		else
//		{
//			temp1&=~(1<<2);
//		}
//		CountPH9=0;
//		TempPH9=0;
//	}
	
	switch(loop)
	{
		case 0:
		{
			count1++;
			if(count1>3000)
			{
				num=0;
				count1=0;
			}
			if(num>=2)
			{
				loop=3;
				count=0;
			}
			else
			{
				CountPH9=0;
				loop=1;
			}
			break;
		}
		case 1:
		{
			if(!HW_GPIO_ReadPin(PH,BIT_9))
			{
				CountPH9++;
			}
			else
			{
				CountPH9=0;
			}
			if(CountPH9>1)
			{
				temp|=(1<<2);
				loop=2;
			}
			break;
		}
		case 2:
		{
			if(HW_GPIO_ReadPin(PH,BIT_9))
			{
				CountPH9++;
			}
			else
			{
				CountPH9=0;
			}
			if(CountPH9>1)
			{
				temp&=~(1<<2);
				loop=0;
				num++;
				count1=0;
			}
			break;
		}
		case 3:
		{
			num=0;
			count++;
			if(count>2000)
			{
				loop=0;
			}
			break;
		}
	}
	
	TempPC14+=!HW_GPIO_ReadPin(PC,BIT_14);
	CountPC14++;
	if(CountPC14==20)
	{
		if(TempPC14>18)
		{
			temp|=(1<<1);
		}
		else
		{
			temp&=~(1<<1);
		}
		CountPC14=0;
		TempPC14=0;
	}
	
	TempPB3+=!HW_GPIO_ReadPin(PB,BIT_3);
	CountPB3++;
	if(CountPB3==20)
	{
		if(TempPB3>18)
		{
			temp|=(1<<0);
		}
		else
		{
			temp&=~(1<<0);
		}
		CountPB3=0;
		TempPB3=0;
	}
	
//	if(HW_GPIO_ReadPin(PH,BIT_9))
//	{
//		temp&=~(1<<0);
//	}
//	else
//	{
//		temp|=(1<<0);
//	}
	
	
//	if(HW_GPIO_ReadPin(PC,BIT_14))
//	{
//		temp&=~(1<<1);
//	}
//	else
//	{
//		temp|=(1<<1);
//	}
	
//	if(HW_GPIO_ReadPin(PE,BIT_5))
//	{
//		temp&=~(1<<2);
//	}
//	else
//	{
//		temp|=(1<<2);
//	}
	
	
	if(HW_GPIO_ReadPin(PC,BIT_4))
	{
		temp&=~(1<<4);
	}
	else
	{
		temp|=(1<<4);
	}
	
	if(HW_GPIO_ReadPin(PB,BIT_7))
	{
		temp&=~(1<<5);
	}
	else
	{
		temp|=(1<<5);
	}
	
	if(HW_GPIO_ReadPin(PC,BIT_5))
	{
		temp&=~(1<<6);
	}
	else
	{
		temp|=(1<<6);
	}
	
	
	
	return temp;
}
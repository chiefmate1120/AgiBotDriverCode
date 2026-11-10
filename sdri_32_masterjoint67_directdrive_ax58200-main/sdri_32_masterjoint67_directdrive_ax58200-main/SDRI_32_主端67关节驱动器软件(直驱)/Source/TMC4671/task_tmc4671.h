#ifndef _task_tmc4671_h_
#define _task_tmc4671_h_
#include"TMC4671.h"

#define EN     HW_GPIO_WritePin(PB,BIT_4, 1);
#define DIS     HW_GPIO_WritePin(PB,BIT_4, 0);


typedef struct
{
	UINT32 Current_P;
	UINT32 Current_I;
	UINT32 Velocity_P;
	UINT32 Velocity_I;
	UINT32 Position_P;
	UINT32 Position_I;
}PIDClass;

extern PIDClass PID_Motor1;
extern PIDClass PID_Motor2;

typedef struct
{
	unsigned char loop;
	unsigned char flag;
	unsigned int counter;
}BrakeClass;

extern BrakeClass brake;
extern BrakeClass brake2;


extern void task_tmc4671_init(void);
extern void _4671_test(void);
void tmc6200_writeInt(uint8_t motor, uint8_t address, int32_t value);
extern void task_tmc6200_init(void);
extern int32_t tmc6200_readInt(uint8_t motor, uint8_t address);
extern void delay_us(unsigned int us);

extern void Set_ADCOFFSET(void);
extern void task_brake(void);

extern int Set_speed;
extern int Set_position;
extern unsigned char Position_PID;
extern int Current_FB;
extern unsigned char Position_PID;

extern int temp_current;
extern void set_4671PID(void);

extern unsigned int Task_Key(void);
extern void Key_init(void);

extern unsigned char Rx_data[5];

#endif
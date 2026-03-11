#ifndef _task_tmc4671_h_
#define _task_tmc4671_h_
#include"TMC4671.h"

#define EN     HW_GPIO_WritePin(PB,BIT_6, 1);
#define DIS     HW_GPIO_WritePin(PB,BIT_6, 0);

typedef struct
{
	BOOL Heartbeat_Flag;
	unsigned int Heartbeat_count;
}HeartbeatClass;
extern HeartbeatClass Heartbeat;

typedef struct
{  
   //实时电流
   int PhaseCurrent_U;
   int PhaseCurrent_V;
   int PhaseCurrent_W;   
   //数据
   int PhaseCurrentGroup_U[50];
   int PhaseCurrentGroup_V[50];
   int PhaseCurrentGroup_W[50];
   //平均值
   int PhaseCurrentSum_U;
   int PhaseCurrentSum_V;
   int PhaseCurrentSum_W;    

   //结果
   float Result_MAX; 
   float Result_MIN;   
   float Result;
   //缺相判断次数
   int Result_count;        
   //数据量及标志位
   int Count;           //数据计数周期
   BOOL StartFlag;      //记录标志
   BOOL CalculateFlag;  //开始计算标志 
   BOOL ResultFlag;     //输出计算结果标志   结果小于10，即认为缺相  
   //HALL信息，不另外建立结构体了
    int HALL_UVW;
    int HALL_U;
    int HALL_V;
    int HALL_W;   
    int HALLErrorCount;
   //
   
}PhaseCurrentClass;
extern PhaseCurrentClass PhaseMissing;




extern int Set_speed;
extern int Set_position;
extern unsigned char Position_PID;
extern int Current_FB;
extern unsigned char Position_PID;
extern int temp_current;
extern unsigned char currentmode;
extern INT16 abn_offset;
extern short ABZ_HALL_offset;


extern void set_4671PID(void);
extern void Key_init(void);
extern unsigned int Task_Key(void);
extern void PhaseMissingDetection(void);
extern void task_tmc4671_init(void);
extern void _4671_test(void);
extern void tmc6200_writeInt(uint8_t motor, uint8_t address, int32_t value);
extern void task_tmc6200_init(void);
extern int32_t tmc6200_readInt(uint8_t motor, uint8_t address);
extern void delay_us(unsigned int us);
extern void delay_ms(unsigned int ms);
extern void Set_ADCOFFSET(void);
extern void task_break(void);
// 求最大值的函数
int max_of_three(int a, int b, int c);
// 求最小值的函数
int min_of_three(int a, int b, int c);


#endif
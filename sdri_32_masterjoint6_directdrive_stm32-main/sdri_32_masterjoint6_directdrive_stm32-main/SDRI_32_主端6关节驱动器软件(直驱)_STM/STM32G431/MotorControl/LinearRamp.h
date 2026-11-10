#ifndef _Linear_Ramp_h_
#define _Linear_Ramp_h_

extern unsigned char PositionRamp;
extern unsigned char RampTemp;

extern void Task_ProfilePosition(void);
extern void linearRamp_init(void);
extern unsigned char PP_flag;
extern void Task_ProfilePositionTest(void);
#endif

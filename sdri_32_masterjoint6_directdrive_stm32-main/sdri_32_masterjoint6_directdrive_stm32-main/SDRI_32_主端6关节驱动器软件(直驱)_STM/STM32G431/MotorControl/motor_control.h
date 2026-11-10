#ifndef _motor_control_h_
#define _motor_control_h_

#include "stm32g4xx_hal.h"
#include "mc_tuning.h"

#define Rx_485	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
#define Tx_485	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)

extern unsigned char Rxdata[5];
extern unsigned char Txdata[5];

extern unsigned char TxBuf[50];
extern unsigned char RxBuf[50];
extern unsigned int RxBufTemp;

extern int ENCOffset;
extern int ENCCount;
extern int PositionOffset;

extern short TargetSpeed,TargetSpeed_pre;

extern int Iq_temp;
extern unsigned char PositionMode;
extern unsigned char AlignmentFlag;
extern unsigned short int motormode;

extern int TargetPosition;
extern int TargetVelocity;
extern int TargetTorque;
extern int ActualPosition;
extern int ActualVelocity;
extern int ActualTorque;
extern unsigned int MotorMode;
extern unsigned int ActualState;

extern int PIDPositionKp,PIDPositionKi;
extern int TargetRampPosition;

extern unsigned char RxFlag;
extern unsigned char RxTemp;

void MotorData_Updata(void);
extern void control_value_task(void);
extern void control_value_update(void);

extern void PID_Position_init(PID_Handle_t pHandle);
extern int PositionVelocity(PID_Handle_t pHandle);

extern void delay_us(unsigned int n);
extern void Position_Iq_task(void);
extern short positon_angle_task(void);

#endif

#ifndef __MOTOR_INTERFACE_H
#define __MOTOR_INTERFACE_H

#include "General_Enc.h"
#include "MotorConfig.h"
#include "MotorBoardConfig.h"
#include "MotorDebugConfig.h"

//Position PID
#define P_KPDIV	(4096)
#define P_KIDIV (16384)
#define P_KDDIV (128)

#define P_KPDIV_LOG	(LOG2(P_KPDIV))
#define P_KIDIV_LOG (LOG2(P_KIDIV))
#define P_KDDIV_LOG (LOG2(P_KDDIV))


/* Speed PID parameter dividers */
#define SP_KPDIV                      (1)
#define SP_KIDIV                      (512)
#define SP_KDDIV                      (16)
#define SP_KPDIV_LOG                  LOG2(SP_KPDIV)
#define SP_KIDIV_LOG                  LOG2(SP_KIDIV)
#define SP_KDDIV_LOG                  LOG2(SP_KDDIV)

//Current Pid
#define TF_KPDIV                      16384
#define TF_KIDIV                      16384
#define TF_KDDIV                      8192
#define TF_KPDIV_LOG                  LOG2(16384)
#define TF_KIDIV_LOG                  LOG2(16384)
#define TF_KDDIV_LOG                  LOG2(8192)



#include "mc_api.h"
#include "mc_math.h"
#include "pid_regulator.h"
#include "drive_parameters.h"
#include "encoder_speed_pos_fdbk.h"


extern PID_Handle_t PIDPositionHandle_M1;

void PositionLoop_Contorl(void);
int16_t User_GetElAngle( void );
void User_GetMultipleLaps(int32_t CurrentPos);

void ENC_Init( ENCODER_Handle_t * pHandle );
bool ENC_CalcAvrgMecSpeedUnit( ENCODER_Handle_t * pHandle, int16_t * pMecSpeedUnit );
int16_t PI_Controller( PID_Handle_t * pHandle, int32_t wProcessVarError );



#endif


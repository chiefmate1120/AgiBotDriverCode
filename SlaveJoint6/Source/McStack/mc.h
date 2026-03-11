/*
 ******************************************************************************
 *     Copyright (c) 2020	ASIX Electronic Corporation      All rights reserved.
 *
 *     This is unpublished proprietary source code of ASIX Electronic Corporation
 *
 *     The copyright notice above does not evidence any actual or intended
 *     publication of such source code.
 ******************************************************************************
 */
#ifndef __MC_H__
#define __MC_H__

/* INCLUDE FILE DECLARATIONS */
#include "mc_misc.h"
#include "curfbk.h"
#include "spdfbk.h"
#include "pwmdrv.h"
#include "adc_manager.h"
#include "hall.h"
#include "six_steps_pwm.h"

/* NAMING CONSTANT DECLARATIONS */
		
/* TYPE DECLARATIONS */
typedef enum
{
	MCSTE_IDLE    = 0,
	MCSTE_CURFBK_CALIB,
	MCSTE_ENC_ALIGN,	
	MCSTE_STARTUP,			
	MCSTE_RUN,		
	MCSTE_SHUTDOWN,		
	MCSTE_STOP,			
	MCSTE_FAULT,
	MCSTE_FAULT_RESET,	
} MC_STATE_E;

typedef enum
{
	MCCMD_NULL = 0,
	MCCMD_START,	
	MCCMD_SET_SPEED,					
	MCCMD_STOP,
	MCCMD_FAULT_RESET,	
} MC_CMD_E;

typedef struct
{
	MC_STATE_E   State;
	volatile MC_CMD_E     Cmd;
	MC_FAULT_E   CurrFault;
	MC_FAULT_E   Fault;	
	uint32_t     DownCnt;

	CURFBK_T    *pCurfbk;	
	SPDFBK_T    *pSpdfbk;
	PWMDRV_T    *pPwmdrv;
} MC_T;

/* MACRO DECLARATIONS */

/* GLOBAL VARIABLES */
extern MC_T McObj;

/* EXPORTED SUBPROGRAM SPECIFICATIONS */
int32_t      MC_Init(void);
void         MC_DeInit(void);
void         MC_Run(void);
void         MC_StartMotor(MC_T *pObj);
void         MC_StopMotor(MC_T *pObj);
MC_STATE_E   MC_GetState(MC_T *pObj);
int32_t      MC_SetSpeedRPM(MC_T *pObj, int32_t TargetSpeedRPM, uint16_t RampDurationMs);
int32_t      MC_GetSpeedRPM(MC_T *pObj);
void         MC_FaultReset(MC_T *pObj);
MC_FAULT_E   MC_GetFaultStatus(MC_T *pObj);
int32_t      MC_GetMecPosition(MC_T *pObj);

#endif /* __MC_H__ */

/* End of mc.h */


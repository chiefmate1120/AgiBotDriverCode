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
#ifndef __SIX_STEPS_PWM_H__
#define __SIX_STEPS_PWM_H__

/* INCLUDE FILE DECLARATIONS */
#include "mc_misc.h"
#include "pwmdrv.h"
#include "spdfbk.h"

/* NAMING CONSTANT DECLARATIONS */

/* TYPE DECLARATIONS */
typedef struct {
	EPWM_T    *pInst;	
	uint8_t    MultiFunc;	
	GPIO_T    *pPort;	
	uint32_t   PwmPinUH;
	uint32_t   PwmPinVH;
	uint32_t   PwmPinWH;
	uint32_t   PwmPinUL;
	uint32_t   PwmPinVL;
	uint32_t   PwmPinWL;
	uint32_t   EnabledChannels;
	uint32_t   Frequency;	
	uint32_t   MaxPwmValue;
} SSPWM_CFG_T;

typedef struct {
	void      *pInst;
	uint8_t   (*GetPhaseStatus)(void *pInst);
} SSPWM_CBF_T;

typedef struct
{
	SSPWM_CFG_T  *pCfg;
	SSPWM_CBF_T  *pCbf;	
	PWMDRV_T      Pwmdrv;
	volatile uint32_t Duty;
} SSPWM_T;

/* MACRO DECLARATIONS */

/* GLOBAL VARIABLES */

/* EXPORTED SUBPROGRAM SPECIFICATIONS */
PWMDRV_T* SSPWM_Init(SSPWM_T *pObj, SSPWM_CFG_T *pCfg, SSPWM_CBF_T *pCbf);
void      SSPWM_DeInit(SSPWM_T *pObj);

int32_t   SSPWM_PhaseChange(void *pVObj, uint8_t PhaseStatus);
void      SSPWM_Control(void *pVObj, MC_STSP_E Ctrl);

#endif /* __SIX_STEPS_PWM_H__ */

/* End of six_steps_pwm.h */

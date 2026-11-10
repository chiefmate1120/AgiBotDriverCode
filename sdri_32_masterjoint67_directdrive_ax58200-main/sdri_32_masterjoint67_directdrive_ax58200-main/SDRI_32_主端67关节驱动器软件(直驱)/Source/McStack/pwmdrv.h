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
#ifndef __PWMDRV_H__
#define __PWMDRV_H__

/* INCLUDE FILE DECLARATIONS */
#include "mc_misc.h"

/* NAMING CONSTANT DECLARATIONS */

/* TYPE DECLARATIONS */
typedef struct {
	void     *pInst;
	void     (*Control)(void *pVObj, MC_STSP_E Ctrl);

	volatile int32_t TargetDuty;	
	volatile int32_t Duty;
	uint8_t   PhaseStatus;
	MC_STSP_E CtrlStatus;	
} PWMDRV_T;

/* MACRO DECLARATIONS */

/* GLOBAL VARIABLES */

/* EXPORTED SUBPROGRAM SPECIFICATIONS */
void      PWMDRV_Control(PWMDRV_T *pObj, MC_STSP_E Ctrl);
void      PWMDRV_SetDuty(PWMDRV_T *pObj, uint32_t Duty);
MC_STSP_E PWMDRV_GetCtrlStatus(PWMDRV_T *pObj);
#endif /* __PWMDRV_H__ */

/* End of pwmdrv.h */

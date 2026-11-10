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
#ifndef __SPDFBK_H__
#define __SPDFBK_H__

/* INCLUDE FILE DECLARATIONS */
#include "mc_misc.h"

/* NAMING CONSTANT DECLARATIONS */

/* TYPE DECLARATIONS */
typedef struct {
	void     *pInst;
	void     (*Control)(void *pVObj, MC_STSP_E Ctrl);
	volatile int32_t  DeltaTickCnt;
	volatile int32_t  AccTickCnt;
	volatile int32_t  TickPerSec;	
	volatile int32_t  SpeedRPM;		
	volatile uint8_t  PhaseStatus;
	         uint32_t SpkErrChkCnt;			
} SPDFBK_T;

/* MACRO DECLARATIONS */

/* GLOBAL VARIABLES */

/* EXPORTED SUBPROGRAM SPECIFICATIONS */
void       SPDFBK_Control(SPDFBK_T *pObj, MC_STSP_E Ctrl);
MC_FAULT_E SPDFBK_CalcSpeed(SPDFBK_T *pObj);
void       SPDFBK_Reset(SPDFBK_T *pObj);
int32_t    SPDFBK_GetSpeedRPM(SPDFBK_T *pObj);
int32_t    SPDFBK_GetMecAngle(SPDFBK_T *pObj);

#endif /* __SPDFBK_H__ */

/* End of spdfbk.h */

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
#ifndef __HALL_H__
#define __HALL_H__

/* INCLUDE FILE DECLARATIONS */
#include "mc_misc.h"
#include "spdfbk.h"
#include "six_steps_pwm.h"

/* NAMING CONSTANT DECLARATIONS */

/* TYPE DECLARATIONS */
typedef struct {
	IRQn_Type  IrqType;
	uint32_t   PreemptPrio;	
	uint32_t   SubPrio;	
	GPIO_T    *pPort;
	uint32_t   StatusPinU;
	uint32_t   StatusPinV;	
	uint32_t   StatusPinW;
} HALL_CFG_T;

typedef struct {
	void      *pInst;
	int32_t   (*PhaseChange)(void *pInst, uint8_t PhaseStatus);
} HALL_CBF_T;

typedef struct {
	HALL_CFG_T  *pCfg;
	HALL_CBF_T  *pCbf;
	SPDFBK_T     Spdfbk;
} HALL_T;

/* MACRO DECLARATIONS */

/* GLOBAL VARIABLES */

/* EXPORTED SUBPROGRAM SPECIFICATIONS */
SPDFBK_T* HALL_Init(HALL_T *pObj, HALL_CFG_T *pCfg, HALL_CBF_T *pCbf);
void      HALL_DeInit(HALL_T *pObj);
void      HALL_Irq(HALL_T *pObj);

uint8_t   HALL_GetPhaseStatus(void *pVObj);
void      HALL_Control(void *pVObj, MC_STSP_E Ctrl);
#endif /* __HALL_H__ */

/* End of hall.h */

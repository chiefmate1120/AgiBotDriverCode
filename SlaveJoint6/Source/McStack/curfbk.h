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
#ifndef __CURFBK_H__
#define __CURFBK_H__

/* INCLUDE FILE DECLARATIONS */
#include "mc_misc.h"

/* NAMING CONSTANT DECLARATIONS */
typedef enum {
	PHSCUR_A = 0,
	PHSCUR_B,	
	PHSCUR_C,		
} CURFBK_PHSCUR_E;

/* TYPE DECLARATIONS */
typedef struct {
	uint16_t PhaseCurrA;
	uint16_t PhaseCurrB;
	uint16_t PhaseCurrC;	
} CURFBK_T;

/* MACRO DECLARATIONS */

/* GLOBAL VARIABLES */

/* EXPORTED SUBPROGRAM SPECIFICATIONS */
uint16_t      CURFBK_GetPhaseCurrent(CURFBK_T *pObj, CURFBK_PHSCUR_E Index);

#endif /* __CURFBK_H__ */

/* End of curfbk.h */

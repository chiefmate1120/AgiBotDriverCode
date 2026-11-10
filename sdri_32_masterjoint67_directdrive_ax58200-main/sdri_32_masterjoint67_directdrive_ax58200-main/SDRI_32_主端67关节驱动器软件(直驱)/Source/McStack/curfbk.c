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

/* INCLUDE FILE DECLARATIONS */
#include "curfbk.h"

/* NAMING CONSTANT DECLARATIONS */

/* TYPE DECLARATIONS */

/* GLOBAL VARIABLES DECLARATIONS */

/* LOCAL VARIABLES DECLARATIONS */

/* LOCAL SUBPROGRAM DECLARATIONS */

/* LOCAL SUBPROGRAM BODIES */

/* EXPORTED SUBPROGRAM BODIES */

/*
 * ----------------------------------------------------------------------------
 * Function Name: CURFBK_GetPhaseCurrent()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
uint16_t CURFBK_GetPhaseCurrent(CURFBK_T *pObj, CURFBK_PHSCUR_E Index)
{
	if (Index == PHSCUR_A)
	{
		return (pObj->PhaseCurrA);
	}
	else if (Index == PHSCUR_B)
	{
		return (pObj->PhaseCurrB);
	}
	else if (Index == PHSCUR_C)
	{
		return (pObj->PhaseCurrC);
	}
	return 0;
} /* End of CURFBK_GetPhaseCurrent() */

/* End of curfbk.c */

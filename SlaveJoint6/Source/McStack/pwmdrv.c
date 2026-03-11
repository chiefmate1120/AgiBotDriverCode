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
#include "pwmdrv.h"
#include <string.h>

/* NAMING CONSTANT DECLARATIONS */

/* TYPE DECLARATIONS */

/* GLOBAL VARIABLES DECLARATIONS */

/* LOCAL VARIABLES DECLARATIONS */

/* LOCAL SUBPROGRAM DECLARATIONS */

/* LOCAL SUBPROGRAM BODIES */

/* EXPORTED SUBPROGRAM BODIES */

/*
 * ----------------------------------------------------------------------------
 * Function Name: PWMDRV_Control()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void PWMDRV_Control(PWMDRV_T *pObj, MC_STSP_E Ctrl)
{
	pObj->Control(pObj->pInst, Ctrl);
#if PWMDRV_DBGPIN_ENABLE
	MCMISC_WRITE_GPIO_PIN(MC_DBG_PORT1, MC_DBG_PIN1, (Ctrl == MC_START) ? 1:0);
#endif	
} /* End of PWMDRV_Control() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: PWMDRV_SetDuty()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void PWMDRV_SetDuty(PWMDRV_T *pObj, uint32_t Duty)
{
	pObj->Duty = Duty;
} /* End of PWMDRV_SetDuty() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: PWMDRV_GetCtrlStatus()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
MC_STSP_E PWMDRV_GetCtrlStatus(PWMDRV_T *pObj)
{
	return (pObj->CtrlStatus);
} /* End of PWMDRV_GetCtrlStatus() */

/* End of pwmdrv.c */

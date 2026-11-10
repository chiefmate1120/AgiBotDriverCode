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
#include "spdfbk.h"

/* NAMING CONSTANT DECLARATIONS */

/* TYPE DECLARATIONS */

/* GLOBAL VARIABLES DECLARATIONS */

/* LOCAL VARIABLES DECLARATIONS */

/* LOCAL SUBPROGRAM DECLARATIONS */

/* LOCAL SUBPROGRAM BODIES */

/* EXPORTED SUBPROGRAM BODIES */

/*
 * ----------------------------------------------------------------------------
 * Function Name: SPDFBK_Control()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void SPDFBK_Control(SPDFBK_T *pObj, MC_STSP_E Ctrl)
{
	pObj->Control(pObj->pInst, Ctrl);
} /* End of SPDFBK_Control() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: SPDFBK_CalcSpeed()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
MC_FAULT_E SPDFBK_CalcSpeed(SPDFBK_T *pObj)
{
	int32_t tmp32;
	
	tmp32 = pObj->DeltaTickCnt * MC_CTRL_LOOP_FREQ_HZ;
	pObj->DeltaTickCnt = 0;
	
	pObj->TickPerSec = (((pObj->TickPerSec*99)+tmp32))/100;
	
	pObj->SpeedRPM = (pObj->TickPerSec*60)/(6*MCPF_POLE_PAIRS);
	if (pObj->TickPerSec)
	{
		pObj->SpkErrChkCnt = 0;
	}
	else if (pObj->SpkErrChkCnt < MC_SPD_ERR_CHK_TIME)
	{
		pObj->SpkErrChkCnt++;
	}
	else
	{
		return MCFS_SPEED_FBK;		
	}
	return MCFS_NO_ERR;
} /* End of SPDFBK_CalcSpeed() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: SPDFBK_Reset()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void SPDFBK_Reset(SPDFBK_T *pObj)
{
	pObj->AccTickCnt = 0;
	pObj->DeltaTickCnt = 0;	
	pObj->TickPerSec = 0;
	pObj->SpeedRPM = 0;
	pObj->SpkErrChkCnt = 0;

} /* End of SPDFBK_Reset() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: SPDFBK_GetSpeedRPM()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
int32_t SPDFBK_GetSpeedRPM(SPDFBK_T *pObj)
{
	return (pObj->SpeedRPM);
} /* End of SPDFBK_GetSpeedRPM() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: SPDFBK_GetMecAngle()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
int32_t SPDFBK_GetMecAngle(SPDFBK_T *pObj)
{
	return (pObj->AccTickCnt);
} /* End of SPDFBK_GetMecAngle() */

/* End of spdfbk.c */

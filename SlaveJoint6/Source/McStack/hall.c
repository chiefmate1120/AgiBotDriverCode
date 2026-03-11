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
#include "hall.h"
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
 * Function Name: HALL_Init()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
SPDFBK_T* HALL_Init(HALL_T *pObj, HALL_CFG_T *pCfg, HALL_CBF_T *pCbf)
{
	/* Check arguments */
	if (pObj==NULL || pCfg==NULL || pCbf==NULL)
	{
		return NULL;
	}
	
	SYS_UnlockReg();

	/* Enable HALL U/V/W input pins */
	MCMISC_SetMFP(pCfg->pPort, pCfg->StatusPinU | pCfg->StatusPinV | pCfg->StatusPinW, 0);	
	GPIO_SetMode(pCfg->pPort, pCfg->StatusPinU | pCfg->StatusPinV | pCfg->StatusPinW, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(pCfg->pPort, pCfg->StatusPinU | pCfg->StatusPinV | pCfg->StatusPinW, GPIO_SLEWCTL_HIGH);
	GPIO_SetPullCtl(pCfg->pPort, pCfg->StatusPinU | pCfg->StatusPinV | pCfg->StatusPinW, GPIO_PUSEL_DISABLE);
	
	/* Enable schmitt trigger input */
	pCfg->pPort->SMTEN |= (pCfg->StatusPinU | pCfg->StatusPinV | pCfg->StatusPinW);
	
	/* Disable interrupt */
	GPIO_DisableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinU));
	GPIO_DisableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinV));
	GPIO_DisableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinW));
		
	/* Enable NVIC */	
	NVIC_SetPriority(pCfg->IrqType, NVIC_EncodePriority(MC_INT_PRIORITY_GROUP, pCfg->PreemptPrio, pCfg->SubPrio));
	NVIC_EnableIRQ(pCfg->IrqType);
	
	SYS_LockReg();	
	
	/* Data structure initilization */
	memset(pObj, 0, sizeof(HALL_T));

	/* Binding configuration */
	pObj->pCfg = pCfg;
	
	/* Binding callback functions */
	pObj->pCbf = pCbf;

	/* Initialize structure for upper speed feedback layer */
	pObj->Spdfbk.pInst = pObj;
	pObj->Spdfbk.Control = HALL_Control;
	
	return (&pObj->Spdfbk);
} /* End of HALL_Init() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HALL_DeInit()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void HALL_DeInit(HALL_T *pObj)
{
	HALL_CFG_T *pCfg = pObj->pCfg;
	
	SYS_UnlockReg();

	/* Disable interrupt */
	GPIO_DisableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinU));
	GPIO_DisableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinV));
	GPIO_DisableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinW));
		
	/* Disable NVIC */	
	NVIC_DisableIRQ(pCfg->IrqType);
	
	SYS_LockReg();
	
	/* Debind configuration */
	pObj->pCfg = NULL;
	pObj->pCbf = NULL;
	
} /* End of HALL_DeInit() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HALL_GetPhaseStatus()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
uint8_t HALL_GetPhaseStatus(void *pVObj)
{
	HALL_T     *pObj = (HALL_T*)pVObj;	
	HALL_CFG_T *pCfg = pObj->pCfg;	
	SPDFBK_T *pSpdfbk = &pObj->Spdfbk;	
	uint32_t tmp32 = pCfg->pPort->PIN;
	
	/* Get current phase status */
	if (tmp32 & pCfg->StatusPinU)
	{
		pSpdfbk->PhaseStatus |= BIT_0;
	}
	else
	{
		pSpdfbk->PhaseStatus &= ~BIT_0;
	}

	if (tmp32 & pCfg->StatusPinV)
	{
		pSpdfbk->PhaseStatus |= BIT_1;
	}
	else
	{
		pSpdfbk->PhaseStatus &= ~BIT_1;
	}

	if (tmp32 & pCfg->StatusPinW)
	{
		pSpdfbk->PhaseStatus |= BIT_2;
	}
	else
	{
		pSpdfbk->PhaseStatus &= ~BIT_2;
	}		
	return (pSpdfbk->PhaseStatus);
} /* End of HALL_GetPhaseStatus() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HALL_Irq()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void HALL_Irq(HALL_T *pObj)
{
	HALL_CFG_T *pCfg = pObj->pCfg;
	HALL_CBF_T *pCbf = pObj->pCbf;	
	SPDFBK_T *pSpdfbk = &pObj->Spdfbk;
	
	if (GPIO_GET_INT_FLAG(pCfg->pPort, pCfg->StatusPinU | pCfg->StatusPinV | pCfg->StatusPinW))
	{
		HALL_GetPhaseStatus(pObj);
		
		/* Counting phase change event */
		pSpdfbk->AccTickCnt++;
		pSpdfbk->DeltaTickCnt++;		
		if (pCbf)
		{
			pCbf->PhaseChange(pCbf->pInst, pSpdfbk->PhaseStatus);
		}
		
		/* Clear interrupt flags */
		GPIO_CLR_INT_FLAG(pCfg->pPort, pCfg->StatusPinU | pCfg->StatusPinV | pCfg->StatusPinW);
	}
} /* End of HALL_Irq() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HALL_Control()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void HALL_Control(void *pVObj, MC_STSP_E Ctrl)
{
	HALL_T     *pObj = (HALL_T*)pVObj;
	HALL_CFG_T *pCfg = pObj->pCfg;
	SPDFBK_T   *pSpdfbk = &pObj->Spdfbk;
	
	if (Ctrl == MC_START)
	{
		pSpdfbk->AccTickCnt = 0;		
		pSpdfbk->DeltaTickCnt = 0;
		pSpdfbk->TickPerSec = 0;	
		pSpdfbk->SpeedRPM = 0;		
		pSpdfbk->PhaseStatus = 0;
		
		/* Enable interrupt */
		GPIO_EnableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinU), GPIO_INT_BOTH_EDGE);
		GPIO_EnableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinV), GPIO_INT_BOTH_EDGE);
		GPIO_EnableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinW), GPIO_INT_BOTH_EDGE);	
	}
	else
	{
		/* Disable interrupt */
		GPIO_DisableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinU));
		GPIO_DisableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinV));
		GPIO_DisableInt(pCfg->pPort, MCMISC_BitToNum(pCfg->StatusPinW));			
	}
} /* End of HALL_Control() */
	
/* End of hall.c */

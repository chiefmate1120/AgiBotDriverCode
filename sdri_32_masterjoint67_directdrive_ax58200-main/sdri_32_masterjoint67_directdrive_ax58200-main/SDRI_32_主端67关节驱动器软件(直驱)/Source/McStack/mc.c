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
#include "mc.h"
#include <string.h>

/* NAMING CONSTANT DECLARATIONS */

/* TYPE DECLARATIONS */

/* GLOBAL VARIABLES DECLARATIONS */

/* LOCAL VARIABLES DECLARATIONS */
SSPWM_CFG_T SspwmCfg = {
	.pInst       = PWM_INSTANCE,	
	.MultiFunc   = PWM_MFP,
	.pPort       = PWM_PORT,	
	.PwmPinUH    = PWM_UH_PIN,
	.PwmPinVH    = PWM_VH_PIN,
	.PwmPinWH    = PWM_WH_PIN,
	.PwmPinUL    = PWM_UL_PIN,
	.PwmPinVL    = PWM_VL_PIN,
	.PwmPinWL    = PWM_WL_PIN,
	.EnabledChannels = EPWM_CH_0_MASK|EPWM_CH_1_MASK|EPWM_CH_2_MASK|EPWM_CH_3_MASK|EPWM_CH_4_MASK|EPWM_CH_5_MASK,	
	.Frequency   = 18000,//Set PWM frequency = 18KHz
	.MaxPwmValue = 1000,
};
SSPWM_T SspwmObj;

HALL_CFG_T HallCfg = {
	.IrqType     = HALL_IRQ,
	.PreemptPrio = HALL_INT_PREEMPTION_PRIORITY,
	.SubPrio     = HALL_INT_SUB_PRIORITY,	
	.pPort       = HALL_PORT,
	.StatusPinU  = HALL_U_PIN,	
	.StatusPinV  = HALL_V_PIN,	
	.StatusPinW  = HALL_W_PIN,		
};
HALL_CBF_T HallCbf = {
	.pInst        = &SspwmObj,
	.PhaseChange  = SSPWM_PhaseChange,
};
HALL_T HallObj;

SSPWM_CBF_T SspwmCbf = {
	.pInst           = &HallObj,
	.GetPhaseStatus  = HALL_GetPhaseStatus,
};

CURFBK_T CurfbkObj;
ADCMNG_CFG_T AdcMngCfg = {
	.pInst       = ADCMNG_INSTANCE,
	.MultiFunc   = ADCMNG_MFP,
	.IrqType     = ADCMNG_IRQ_TYPE,	
	.pPort       = ADCMNG_PORT,	
	.Channels[0] = ADCMNG_CURFBK_U_PIN,
	.Channels[1] = ADCMNG_CURFBK_V_PIN,
	.Channels[2] = ADCMNG_CURFBK_W_PIN,
	.pConvBuf[0] = &(CurfbkObj.PhaseCurrA),	
	.pConvBuf[1] = &(CurfbkObj.PhaseCurrB),	
	.pConvBuf[2] = &(CurfbkObj.PhaseCurrC),	
	.AllocChNum  = 3,
};
ADCMNG_T AdcMngObj;

MC_T McObj;
static uint8_t mc_inited = 0;

/* LOCAL SUBPROGRAM DECLARATIONS */
static int32_t mc_Task(MC_T *pObj);

/* LOCAL SUBPROGRAM BODIES */
/*
 * ----------------------------------------------------------------------------
 * Function Name: mc_Task()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
static int32_t mc_Task(MC_T *pObj)
{
	PWMDRV_T *pPwmdrv = pObj->pPwmdrv;
	SPDFBK_T *pSpdfbk = pObj->pSpdfbk;	
	
	if (pObj->DownCnt)
	{
		pObj->DownCnt--;
	}
	
	pObj->CurrFault = MCFS_NO_ERR;
	
	/* Calculate speed */
	if (PWMDRV_GetCtrlStatus(pPwmdrv) == MC_START)
	{	
		pObj->CurrFault |= SPDFBK_CalcSpeed(pSpdfbk);
	}
	else
	{
		SPDFBK_CalcSpeed(pSpdfbk);
	}
	
	/* Fault detection */
	if (pObj->Fault == 0 && pObj->CurrFault != 0)
	{
		PWMDRV_Control(pPwmdrv, MC_STOP);		
		pObj->Fault = pObj->CurrFault;
		pObj->State = MCSTE_FAULT;
	}
	
	switch (pObj->State)
	{
	case MCSTE_IDLE:
		if (pObj->Cmd == MCCMD_START)
		{
			pObj->Cmd = MCCMD_NULL;
			pObj->State = MCSTE_CURFBK_CALIB;
		}
		break;
	
	case MCSTE_CURFBK_CALIB:
		pObj->State = MCSTE_ENC_ALIGN;
		break;
	
	case MCSTE_ENC_ALIGN:
		pObj->DownCnt = 100;		
		pObj->State = MCSTE_STARTUP;		
		break;

	case MCSTE_STARTUP:
		if (pObj->DownCnt == 0)
		{		
			pObj->Cmd = MCCMD_SET_SPEED;
			SPDFBK_Reset(pSpdfbk);
			PWMDRV_SetDuty(pPwmdrv, 200);//Set startup duty
			PWMDRV_Control(pPwmdrv, MC_START);			
			pObj->DownCnt = 500;//Wait for start up			
			pObj->State = MCSTE_RUN;
		}
		break;
	
	case MCSTE_RUN:
		if (pObj->DownCnt)
		{
			break;
		}
		
		if (pObj->Cmd == MCCMD_STOP)
		{
			pObj->Cmd = MCCMD_NULL;			
			PWMDRV_SetDuty(pPwmdrv, 100);//Set shutdown duty
			pObj->DownCnt = 500;//Wait for shutdown
			pObj->State = MCSTE_SHUTDOWN;
		}
		else if (pObj->Cmd == MCCMD_SET_SPEED)
		{
			pObj->Cmd = MCCMD_NULL;
			PWMDRV_SetDuty(pPwmdrv, pPwmdrv->TargetDuty);
		}
		break;
	
	case MCSTE_SHUTDOWN:
		if (pObj->DownCnt)
		{
			break;
		}
		pObj->State = MCSTE_STOP;			
		break;
	
	case MCSTE_STOP:
		PWMDRV_Control(pPwmdrv, MC_STOP);
		pObj->State = MCSTE_IDLE;			
		break;
	
	case MCSTE_FAULT:
		if (pObj->Cmd == MCCMD_FAULT_RESET)
		{
			pObj->Cmd = MCCMD_NULL;
			
			/* Clear fault status */
			pObj->Fault = MCFS_NO_ERR;			
			pObj->State = MCSTE_FAULT_RESET;
		}
		break;
	
	case MCSTE_FAULT_RESET:
		pObj->State = MCSTE_IDLE;
		break;	
	}
	
	return MCSTS_OK;
} /* End of mc_Task() */

/* EXPORTED SUBPROGRAM BODIES */
/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_Init()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
int32_t MC_Init(void)
{
	memset(&McObj, 0, sizeof(McObj));
	
	/* Initialize HALL sensing module */
 	McObj.pSpdfbk	= HALL_Init(&HallObj, &HallCfg, &HallCbf);
	if (McObj.pSpdfbk == NULL)
	{
		MC_PRINT("Initialize HALL sensing module fail\r\n");	
		return MCSTS_ERR;
	}

	/* Initialize ADC module */
	if (ADCMNG_Init(&AdcMngObj, &AdcMngCfg) != MCSTS_OK)
	{
		MC_PRINT("Initialize ADC module fail\r\n");
		return MCSTS_ERR;		
	}
	McObj.pCurfbk = &CurfbkObj;
	
	/* Initialize PWM module */
	McObj.pPwmdrv = SSPWM_Init(&SspwmObj, &SspwmCfg, &SspwmCbf);
	if (McObj.pPwmdrv == NULL)
	{
		MC_PRINT("Initialize PWM module fail\r\n");			
		return MCSTS_ERR;
	}

	/* Start speed feedback module */
	SPDFBK_Control(McObj.pSpdfbk, MC_START);
	
#if MC_DBGPIN_ENABLE	
	MCMISC_SetMFP(MC_DBG_PORT0, MC_DBG_PIN0, 0);
	GPIO_SetMode(MC_DBG_PORT0, MC_DBG_PIN0, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(MC_DBG_PORT0, MC_DBG_PIN0, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(MC_DBG_PORT0, MC_DBG_PIN0, GPIO_PUSEL_PULL_UP);
	MCMISC_WRITE_GPIO_PIN(MC_DBG_PORT0, MC_DBG_PIN0, 0);
	
	MCMISC_SetMFP(MC_DBG_PORT1, MC_DBG_PIN1, 0);
	GPIO_SetMode(MC_DBG_PORT1, MC_DBG_PIN1, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(MC_DBG_PORT1, MC_DBG_PIN1, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(MC_DBG_PORT1, MC_DBG_PIN1, GPIO_PUSEL_PULL_UP);	
	MCMISC_WRITE_GPIO_PIN(MC_DBG_PORT1, MC_DBG_PIN1, 0);
#endif
	
	/* Set up inited flag */
	mc_inited = 1;
	
	return MCSTS_OK;
} /* End of MC_Init() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_Run()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void MC_Run(void)
{
	if (mc_inited)
	{
		/* Start phase current conversion */
		ADCMNG_Start(&AdcMngObj);		
		
		mc_Task(&McObj);
	}
} /* End of MC_Run() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_DeInit()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void MC_DeInit(void)
{
	mc_inited = 0;
	
} /* End of MC_DeInit() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_StartMotor()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void MC_StartMotor(MC_T *pObj)
{
	pObj->Cmd = MCCMD_START;
} /* End of MC_StartMotor() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_StopMotor()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void MC_StopMotor(MC_T *pObj)
{
	pObj->Cmd = MCCMD_STOP;
} /* End of MC_StopMotor() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_GetState()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
MC_STATE_E MC_GetState(MC_T *pObj)
{
	return pObj->State;
} /* End of MC_GetState() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_SetSpeedRPM()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
int32_t MC_SetSpeedRPM(MC_T *pObj, int32_t TargetSpeedRPM, uint16_t RampDurationMs)
{
#if 0
	pObj->pPwmdrv->TargetDuty = ((TargetSpeedRPM * 1000)/MCPF_RATE_SPEED);
#else
	if (TargetSpeedRPM < 500)
	{
		pObj->pPwmdrv->TargetDuty = ((TargetSpeedRPM * 1000)/MCPF_RATE_SPEED);
	}
	else if (TargetSpeedRPM < 1000)
	{
		pObj->pPwmdrv->TargetDuty = (((TargetSpeedRPM-500)*(333-166))/(1750-510)) + 166;
	}
	else if (TargetSpeedRPM < 1500)
	{
		pObj->pPwmdrv->TargetDuty = (((TargetSpeedRPM-1000)*(500-333))/(2502-1750)) + 233;
	}
	else if (TargetSpeedRPM < 2000)
	{
		pObj->pPwmdrv->TargetDuty = (((TargetSpeedRPM-1500)*(666-500))/(3235-2502)) + 300;
	}
	else if (TargetSpeedRPM < 2500)
	{
		pObj->pPwmdrv->TargetDuty = (((TargetSpeedRPM-2000)*(833-666))/(3760-3235)) + 366;		
	}
	else if (TargetSpeedRPM < 3000)
	{
		pObj->pPwmdrv->TargetDuty = (((TargetSpeedRPM-2500)*(1000-833))/(4500-3760)) + 466;				
	}	
#endif
	pObj->Cmd = MCCMD_SET_SPEED;
	return 0;
} /* End of MC_SetSpeedRPM() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_GetSpeedRPM()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
int32_t MC_GetSpeedRPM(MC_T *pObj)
{
	return SPDFBK_GetSpeedRPM(pObj->pSpdfbk);
} /* End of MC_GetSpeedRPM() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_FaultReset()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void MC_FaultReset(MC_T *pObj)
{
	pObj->Cmd = MCCMD_FAULT_RESET;
} /* End of MC_FaultReset() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_GetFaultStatus()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
MC_FAULT_E MC_GetFaultStatus(MC_T *pObj)
{
	return pObj->Fault;
} /* End of MC_GetFaultStatus() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: MC_GetMecPosition()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
int32_t MC_GetMecPosition(MC_T *pObj)
{
	return 0;
} /* End of MC_GetMecPosition() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HALL_IRQHandler()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void HALL_IRQHandler(void)
{
	HALL_Irq(&HallObj);
} /* End of HALL_IRQHandler() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: ADCMNG_IRQHandler()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void ADCMNG_IRQHandler(void)
{
	ADCMNG_Irq(&AdcMngObj);
} /* End of ADCMNG_IRQHandler() */

/* End of mc.c */

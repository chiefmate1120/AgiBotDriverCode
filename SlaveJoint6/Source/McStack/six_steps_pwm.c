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
#include "six_steps_pwm.h"
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
 * Function Name: SSPWM_Init()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
PWMDRV_T* SSPWM_Init(SSPWM_T *pObj, SSPWM_CFG_T *pCfg, SSPWM_CBF_T *pCbf)
{
	uint32_t i, bitMask = 0x01ul, tmp32;
	
	/* Check arguments */
	if (pObj==NULL || pCfg==NULL || pCbf==NULL)
	{
		return NULL;
	}
	
	SYS_UnlockReg();

	/* Enable PWM module clock */
	MCMISC_EpwmClkSource(pCfg->pInst, MC_ENABLE);
	
	/* PWM phase-U,V,W low side configuration  */	
	MCMISC_SetMFP(pCfg->pPort, pCfg->EnabledChannels, pCfg->MultiFunc);

	/* Set channel frequency and duty configuration as follows */	
	tmp32 = ((CLK_GetPLLClockFreq()/(pCfg->MaxPwmValue+1))/pCfg->Frequency)-1;
	bitMask = 0x01ul;
	for (i=0; i<EPWM_CHANNEL_NUM; i++)
	{
		if (pCfg->EnabledChannels & bitMask)
		{
			/* Set period */
			EPWM_SET_CNR(pCfg->pInst, i, pCfg->MaxPwmValue-1);
			
			/* Set timer clock prescaler */
			EPWM_SET_PRESCALER(pCfg->pInst, i, tmp32);			
			
			/* Enable dead zone */
			EPWM_EnableDeadZone(pCfg->pInst, i, 10);			
			
			/* Set up counter type */
			pCfg->pInst->CTL1 &= ~(0x3ul << (i*2));
			pCfg->pInst->CTL1 |= (EPWM_UP_COUNTER << (i*2));
			
			/* Set duty */
			EPWM_SET_CMR(pCfg->pInst, i, 0);			
		}
		bitMask <<= 1;
	}
		
	/* Enable sync. start */
	EPWM_ENABLE_TIMER_SYNC(pCfg->pInst, pCfg->EnabledChannels, (pCfg->pInst==EPWM0)?EPWM_SSCTL_SSRC_EPWM0:EPWM_SSCTL_SSRC_EPWM1);
	
	/* EPWM period point trigger ADC enable */
//	EPWM_EnableADCTrigger(pCfg->pInst, 0, EPWM_TRG_ADC_EVEN_ZERO);

	/* Set output level at zero, compare up, period(center) and compare down of specified channel */
	EPWM_SET_OUTPUT_LEVEL(pCfg->pInst, pCfg->EnabledChannels, EPWM_OUTPUT_HIGH, EPWM_OUTPUT_LOW, EPWM_OUTPUT_NOTHING, EPWM_OUTPUT_NOTHING);	

//	EPWM_ENABLE_COMPLEMENTARY_MODE(pCfg->pInst);
//	EPWM_DISABLE_COMPLEMENTARY_MODE(pCfg->pInst);
	
//	EPWM_ENABLE_GROUP_MODE(pCfg->pInst);	
//	EPWM_DISABLE_GROUP_MODE(pCfg->pInst);

	/* Enable output inverter */
//	EPWM_ENABLE_OUTPUT_INVERTER(pCfg->pInst, pCfg->EnabledChannels);	
	
	/* Clear counts */
	EPWM_CLR_COUNTER(pCfg->pInst, pCfg->EnabledChannels);	
	/* Wait for counter cleared */
	while (pCfg->pInst->CNTCLR);


	SYS_LockReg();
	
	/* Data structure initilization */
	memset(pObj, 0, sizeof(SSPWM_T));
	
	/* Binding configuration */
	pObj->pCfg = pCfg;
	pObj->pCbf = pCbf;
	
	/* Initialize structure for upper pwmdrv layer */
	pObj->Pwmdrv.pInst = pObj;
	pObj->Pwmdrv.Control = SSPWM_Control;
	
	return (&pObj->Pwmdrv);
} /* End of SSPWM_Init() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: SSPWM_DeInit()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void SSPWM_DeInit(SSPWM_T *pObj)
{
	SSPWM_CFG_T *pCfg = pObj->pCfg;
	
	SYS_UnlockReg();

	/* Stop PWM outputs */
	SSPWM_Control(pObj, MC_STOP);
	
	/* Disable PWM module clock */
	MCMISC_EpwmClkSource(pCfg->pInst, MC_DISABLE);
	
	/* Transit IO into tri-state */
	MCMISC_SetMFP(pCfg->pPort, pCfg->PwmPinUH | pCfg->PwmPinVH | pCfg->PwmPinWH | pCfg->PwmPinUL | pCfg->PwmPinVL | pCfg->PwmPinWL, 0);
	GPIO_SetMode(pCfg->pPort, pCfg->PwmPinUH | pCfg->PwmPinVH | pCfg->PwmPinWH | pCfg->PwmPinUL | pCfg->PwmPinVL | pCfg->PwmPinWL, GPIO_MODE_INPUT);
	
	SYS_LockReg();
	
	/* Debind configuration */
	pObj->pCfg = NULL;
} /* End of SSPWM_DeInit() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: SSPWM_Control()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void SSPWM_Control(void *pVObj, MC_STSP_E Ctrl)
{
	SSPWM_T     *pObj = pVObj;	
	SSPWM_CFG_T *pCfg = pObj->pCfg;
	PWMDRV_T    *pPwmdrv = &(pObj->Pwmdrv);

	if (Ctrl == MC_START)
	{
		/* Clear counts */
		EPWM_CLR_COUNTER(pCfg->pInst, pCfg->EnabledChannels);	
		/* Wait for counter cleared */
		while (pCfg->pInst->CNTCLR);
		
		/* Reset CMR */		
		EPWM_SET_CMR(pCfg->pInst, 0, 0);			
		EPWM_SET_CMR(pCfg->pInst, 1, 0);	
		EPWM_SET_CMR(pCfg->pInst, 2, 0);
		EPWM_SET_CMR(pCfg->pInst, 3, 0);
		EPWM_SET_CMR(pCfg->pInst, 4, 0);
		EPWM_SET_CMR(pCfg->pInst, 5, 0);
		
		/* Enable outputs */
		EPWM_EnableOutput(pCfg->pInst, pCfg->EnabledChannels);

		/* Start counters */
		EPWM_TRIGGER_SYNC_START(pCfg->pInst);
	}
	else
	{
		/* Disable all outputs */
		EPWM_DisableOutput(pCfg->pInst, pCfg->EnabledChannels);

		/* Stop all counters */
		EPWM_ForceStop(pCfg->pInst, pCfg->EnabledChannels);		
	}
	pPwmdrv->CtrlStatus = Ctrl;
	
	/* Auto driving PWM */
	if (Ctrl == MC_START)
	{
		SSPWM_PhaseChange(pVObj, pObj->pCbf->GetPhaseStatus(pObj->pCbf->pInst));		
	}		
} /* End of SSPWM_Control() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: SSPWM_PhaseChange()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * Status Step -MOSU-  -MOSV-  -MOSW-  --GDVU-- --GDVV-- --GDVW--
 *    WVU      H   L   H   L   H   L   HIN nLIN HIN nLIN HIN nLIN
 *    -      0 OFF ON  OFF ON  OFF ON  L   L    L   L    L   L
 *    010    1 ON  OFF OFF PWM OFF OFF H   H    PWM L    H   L
 *    110    2 ON  OFF OFF OFF OFF PWM H   H    H   L    PWM L
 *    100    3 OFF OFF ON  OFF OFF PWM H   L    H   H    PWM L
 *    101    4 OFF PWM ON  OFF OFF OFF PWM L    H   H    H   L
 *    001    5 OFF PWM OFF OFF ON  OFF PWM L    H   L    H   H
 *    011    6 OFF OFF OFF PWM ON  OFF H   L    PWM L    H   H
 *    -      7 ON  OFF ON  OFF ON  OFF H   H    H   H    H   H
 * ----------------------------------------------------------------------------
 */
int32_t SSPWM_PhaseChange(void *pVObj, uint8_t Status)
{
	SSPWM_T     *pObj = pVObj;
	SSPWM_CFG_T *pCfg = pObj->pCfg;
	PWMDRV_T    *pPwmdrv = &pObj->Pwmdrv;
	
	if (pPwmdrv->CtrlStatus != MC_START)
	{
		return 0;
	}
	
	if (pPwmdrv->Duty > pCfg->MaxPwmValue)
	{
		pPwmdrv->Duty = pCfg->MaxPwmValue;
	}
	
	/* Set actual duty based on get driver polarity */
	pObj->Duty = pCfg->MaxPwmValue-pPwmdrv->Duty;

	pPwmdrv->PhaseStatus = Status;	
	switch(Status)
	{
	case 4://WVU = b100
		/* U: HIN=100%, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 0, pCfg->MaxPwmValue);
		EPWM_SET_CMR(pCfg->pInst, 1, 0);
		
		/* V: HIN=100%, nLIN=100% */
		EPWM_SET_CMR(pCfg->pInst, 2, pCfg->MaxPwmValue);
		EPWM_SET_CMR(pCfg->pInst, 3, pCfg->MaxPwmValue);
		
		/* W: HIN=Duty, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 4, pObj->Duty);	
		EPWM_SET_CMR(pCfg->pInst, 5, 0);	
		break;
	case 5://WVU = b101
		/* U: HIN=Duty, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 0, pObj->Duty);
		EPWM_SET_CMR(pCfg->pInst, 1, 0);
		
		/* V: HIN=100%, nLIN=100% */
		EPWM_SET_CMR(pCfg->pInst, 2, pCfg->MaxPwmValue);
		EPWM_SET_CMR(pCfg->pInst, 3, pCfg->MaxPwmValue);
		
		/* W: HIN=100%, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 4, pCfg->MaxPwmValue);	
		EPWM_SET_CMR(pCfg->pInst, 5, 0);		
		break;
	case 1://WVU = b001
		/* U: HIN=Duty, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 0, pObj->Duty);
		EPWM_SET_CMR(pCfg->pInst, 1, 0);
		
		/* V: HIN=100%, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 2, pCfg->MaxPwmValue);
		EPWM_SET_CMR(pCfg->pInst, 3, 0);
		
		/* W: HIN=100%, nLIN=100% */
		EPWM_SET_CMR(pCfg->pInst, 4, pCfg->MaxPwmValue);	
		EPWM_SET_CMR(pCfg->pInst, 5, pCfg->MaxPwmValue);		
		break;
	case 3://WVU = b011
		/* U: HIN=100%, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 0, pCfg->MaxPwmValue);
		EPWM_SET_CMR(pCfg->pInst, 1, 0);
		
		/* V: HIN=Duty, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 2, pObj->Duty);
		EPWM_SET_CMR(pCfg->pInst, 3, 0);
		
		/* W: HIN=100%, nLIN=100% */
		EPWM_SET_CMR(pCfg->pInst, 4, pCfg->MaxPwmValue);	
		EPWM_SET_CMR(pCfg->pInst, 5, pCfg->MaxPwmValue);	
		break;
	case 2://WVU = b010
		/* U: HIN=100%, nLIN=100% */
		EPWM_SET_CMR(pCfg->pInst, 0, pCfg->MaxPwmValue);
		EPWM_SET_CMR(pCfg->pInst, 1, pCfg->MaxPwmValue);
		
		/* V: HIN=Duty, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 2, pObj->Duty);
		EPWM_SET_CMR(pCfg->pInst, 3, 0);
		
		/* W: HIN=100%, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 4, pCfg->MaxPwmValue);	
		EPWM_SET_CMR(pCfg->pInst, 5, 0);	
		break;
	case 6://WVU = b110
		/* U: HIN=100%, nLIN=100% */
		EPWM_SET_CMR(pCfg->pInst, 0, pCfg->MaxPwmValue);
		EPWM_SET_CMR(pCfg->pInst, 1, pCfg->MaxPwmValue);
		
		/* V: HIN=100%, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 2, pCfg->MaxPwmValue);
		EPWM_SET_CMR(pCfg->pInst, 3, 0);
		
		/* W: HIN=Duty, nLIN=0% */
		EPWM_SET_CMR(pCfg->pInst, 4, pObj->Duty);	
		EPWM_SET_CMR(pCfg->pInst, 5, 0);	
		break;
	default:
		break;
	}
	
	return 0;
} /* End of SSPWM_PhaseChange() */

/* End of six_steps_pwm.c */

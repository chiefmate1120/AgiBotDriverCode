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
#include "adc_manager.h"
#include "mc_misc.h"
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
 * Function Name: ADCMNG_Init()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
int32_t ADCMNG_Init(ADCMNG_T *pObj, ADCMNG_CFG_T *pCfg)
{
	uint32_t i;
	
	/* Check arguments */
	if (pObj==NULL || pCfg==NULL)
	{
		return MCSTS_ERR;
	}
	
	SYS_UnlockReg();
		
	/* Enable EADC module clock */
	MCMISC_EadcClkSource(pCfg->pInst, MC_ENABLE);
	
	/* EADC pin configuration */
	for (i=0; i<pCfg->AllocChNum; i++)
	{
		/* Check buffer point */
		if (pCfg->pConvBuf[i] == NULL)
		{
			return MCSTS_ERR;
		}

		/* Generate sample module flags */
		pCfg->SampleModuleMask |= (0x01<<i);
		
		MCMISC_SetMFP(pCfg->pPort, pCfg->Channels[i], pCfg->MultiFunc);
		GPIO_SetMode(pCfg->pPort, pCfg->Channels[i], GPIO_MODE_INPUT);
		GPIO_DISABLE_DIGITAL_PATH(pCfg->pPort, pCfg->Channels[i]);
	}

	/* Set input mode as single-end and enable the A/D converter */	
	EADC_Open(pCfg->pInst, EADC_CTL_DIFFEN_SINGLE_END);
	
	/* Configure EADC modules */
	for (i=0; i<pCfg->AllocChNum; i++)
	{	
		EADC_ConfigSampleModule(pCfg->pInst, i, EADC_SOFTWARE_TRIGGER, MCMISC_BitToNum(pCfg->Channels[i]));
	}
	
	/* Clear the interrupt flag for safe */
	EADC_CLR_INT_FLAG(pCfg->pInst, EADC_STATUS2_ADIF0_Msk);
						
	/* Enable sample module interrupts */
	EADC_ENABLE_SAMPLE_MODULE_INT(pCfg->pInst, 0, pCfg->SampleModuleMask);
						
	/* Enable ADINT0 interrupt in NVIC */
	EADC_ENABLE_INT(pCfg->pInst, BIT_0);
  NVIC_EnableIRQ(pCfg->IrqType);

	SYS_LockReg();
	
	/* Data structure initilization */
	memset(pObj, 0, sizeof(ADCMNG_T));

	/* Data structure binding with configuration */
	pObj->pCfg = pCfg;
	
	return MCSTS_OK;
} /* End of ADCMNG_Init() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: ADCMNG_DeInit()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void ADCMNG_DeInit(ADCMNG_T *pObj)
{
	ADCMNG_CFG_T *pCfg = pObj->pCfg;
	
	SYS_UnlockReg();

	/* Disable sample module interrupts */
	EADC_DISABLE_SAMPLE_MODULE_INT(pCfg->pInst, 0, pCfg->SampleModuleMask);

	/* Close ADC module */
	EADC_Close(pCfg->pInst);

	/* Disable EADC module clock */
	MCMISC_EadcClkSource(pCfg->pInst, MC_DISABLE);	

	/* Disable ADINT0 interrupt in NVIC */
	EADC_DISABLE_INT(pCfg->pInst, BIT_0);
  NVIC_DisableIRQ(pCfg->IrqType);
	
	SYS_LockReg();
	
	/* Debind configuration */
	pObj->pCfg = NULL;

} /* End of ADCMNG_DeInit() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: ADCMNG_Start()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void ADCMNG_Start(ADCMNG_T *pObj)
{
	uint32_t i;
	ADCMNG_CFG_T *pCfg = pObj->pCfg;
	
	/* Clear the interrupt flag for safe */
	EADC_CLR_INT_FLAG(pCfg->pInst, EADC_STATUS2_ADIF0_Msk);

	/* Software trigger start conversion */
	EADC_START_CONV(pCfg->pInst, pCfg->SampleModuleMask);

	/* Check converiosn done */
	while (pCfg->pInst->SWTRG & pCfg->SampleModuleMask);
	
	/* Get converted data into application buffer */
	for (i=0; i<pCfg->AllocChNum; i++)
	{			
		*(pCfg->pConvBuf[i]) = (uint16_t)(EADC_GET_CONV_DATA(pCfg->pInst, i));
	}

} /* End of ADCMNG_Start() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: ADCMNG_Stop()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void ADCMNG_Stop(ADCMNG_T *pObj)
{
	ADCMNG_CFG_T *pCfg = pObj->pCfg;
	
	/* Cancel pended conversion */
	EADC_STOP_CONV(pCfg->pInst, pCfg->SampleModuleMask);
	
} /* End of ADCMNG_Stop() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: ADCMNG_Irq()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void ADCMNG_Irq(ADCMNG_T *pObj)
{
	ADCMNG_CFG_T *pCfg = pObj->pCfg;	
#if ADCMNG_DBGPIN_ENABLE			
	MCMISC_WRITE_GPIO_PIN(MC_DBG_PORT0, MC_DBG_PIN0, 1);
	MCMISC_WRITE_GPIO_PIN(MC_DBG_PORT0, MC_DBG_PIN0, 0);
#endif	
	if (EADC_GET_INT_FLAG(pCfg->pInst, EADC_STATUS2_ADIF0_Msk))
	{
		EADC_CLR_INT_FLAG(pCfg->pInst, EADC_STATUS2_ADIF0_Msk);      /* Clear the interrupt flag */
	}
} /* End of ADCMNG_Irq() */

/* End of adc_manager.c */

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
#ifndef __ADC_MANAGER_H__
#define __ADC_MANAGER_H__

/* INCLUDE FILE DECLARATIONS */
#include "mc_misc.h"

/* NAMING CONSTANT DECLARATIONS */
#define ADCMNG_MAX_CH_NUM        15

/* TYPE DECLARATIONS */
typedef struct {
	EADC_T    *pInst;
	uint8_t    MultiFunc;
	IRQn_Type  IrqType;	
	GPIO_T    *pPort;
	uint32_t   Channels[ADCMNG_MAX_CH_NUM];	/* Low index has high sampling priority */	
	uint16_t  *pConvBuf[ADCMNG_MAX_CH_NUM];		
	uint32_t   AllocChNum;
	uint32_t   SampleModuleMask;
} ADCMNG_CFG_T;

typedef struct
{
	ADCMNG_CFG_T   *pCfg;
} ADCMNG_T;

/* MACRO DECLARATIONS */

/* GLOBAL VARIABLES */

/* EXPORTED SUBPROGRAM SPECIFICATIONS */
int32_t  ADCMNG_Init(ADCMNG_T *pObj, ADCMNG_CFG_T *pCfg);
void     ADCMNG_DeInit(ADCMNG_T *pObj);
void     ADCMNG_Start(ADCMNG_T *pObj);
void     ADCMNG_Stop(ADCMNG_T *pObj);
void     ADCMNG_Irq(ADCMNG_T *pObj);
#endif /* __ADC_MANAGER_H__ */

/* End of adc_manager.h */

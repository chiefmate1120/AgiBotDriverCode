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
#ifndef __MC_CFG_H__
#define __MC_CFG_H__

/* INCLUDE FILE DECLARATIONS */
#include "mc_types.h"

/* NAMING CONSTANT DECLARATIONS */
#define MC_DEBUG_ENABLE         0
#define MC_DBGPIN_ENABLE        1
#define ADCMNG_DBGPIN_ENABLE    1
#define PWMDRV_DBGPIN_ENABLE    1

#if (MC_DEBUG_ENABLE)
	#define MC_PRINT               printf
#else
	#define MC_PRINT(f, ...)       {}
#endif

/* Control Layer Setting */
#define MC_CTRL_LOOP_FREQ_HZ             1000	//Hz
#define MC_SPD_ERR_CHK_TIME              2000	//ms
		
/* Motor Profile */
#define MCPF_POLE_PAIRS                  4	
#define MCPF_RATE_VOLTAGE                24   //Volt.
#define MCPF_RATE_POWER                  80   //Watt.
#define MCPF_RATE_TORQUE                 2.6  //Kg-cm
#define MCPF_RATE_CURRENT                4    //+-5%Amp.
#define MCPF_RATE_SPEED                  3000 //+-5%RPM
#define MCPF_ENCODER_REVOLUTION          2500

/* Interrupt Priority Setting */
#define MC_INT_PRIORITY_GROUP            3
#define HALL_INT_PREEMPTION_PRIORITY     5
#define HALL_INT_SUB_PRIORITY            0	

/* HALL Interface IO Definition */
#define HALL_IRQHandler     GPA_IRQHandler
#define HALL_IRQ            GPA_IRQn
#define HALL_PORT           PA
#define HALL_U_PIN          BIT_1
#define HALL_V_PIN          BIT_0
#define HALL_W_PIN          BIT_2

/* PWM U/V/W IO Definition */
#define PWM_INSTANCE        EPWM1
#define PWM_MFP             12
#define PWM_IRQHandler      EPWM1P0_IRQHandler
#define PWM_IRQ             EPWM1P0_IRQn
#define PWM_PORT            PC
#define PWM_UH_PIN          BIT_5
#define PWM_UL_PIN          BIT_4
#define PWM_VH_PIN          BIT_3
#define PWM_VL_PIN          BIT_2
#define PWM_WH_PIN          BIT_1
#define PWM_WL_PIN          BIT_0
#define PWM_BRK_PORT        PE
#define PWM_BRK_PIN         BIT_7

/* Three Phase Current Sensing IO Definition */
#define ADCMNG_INSTANCE     EADC0
#define ADCMNG_MFP          1
#define ADCMNG_IRQHandler   EADC00_IRQHandler
#define ADCMNG_IRQ_TYPE     EADC00_IRQn
#define ADCMNG_PORT         PB
#define ADCMNG_CURFBK_U_PIN BIT_11
#define ADCMNG_CURFBK_V_PIN BIT_8
#define ADCMNG_CURFBK_W_PIN BIT_7

/* Debug IO Definition */
#define MC_DBG_PORT0        PH
#define MC_DBG_PIN0         BIT_9
#define MC_DBG_PORT1        PH
#define MC_DBG_PIN1         BIT_8

/* TYPE DECLARATIONS */

/* GLOBAL VARIABLES */

/* EXPORTED SUBPROGRAM SPECIFICATIONS */


#endif /* __MC_CFG_H__ */

/* End of mc_cfg.h */

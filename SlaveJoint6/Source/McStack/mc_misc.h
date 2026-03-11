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
#ifndef __MC_MISC_H__
#define __MC_MISC_H__

/* INCLUDE FILE DECLARATIONS */
#include "mc_cfg.h"

/* NAMING CONSTANT DECLARATIONS */

/* TYPE DECLARATIONS */

/* MACRO DECLARATIONS */
#define MCMISC_WRITE_GPIO_PIN(port, pin, ctrl)    {if (ctrl!=0) \
                                                     port->DOUT |= pin; \
                                                   else\
                                                     port->DOUT &= (~pin);}

#define MCMISC_TOGGLE_GPIO_PIN(port, pin)         (port->DOUT ^= pin)
#define MCMISC_READ_GPIO_PIN(port, pin)           ((port->PIN & pin)?1:0)

/* GLOBAL VARIABLES */

/* EXPORTED SUBPROGRAM SPECIFICATIONS */
int32_t  MCMISC_SetMFP(GPIO_T *pInst, uint32_t Pins, uint8_t MultiFuncValue);
void     MCMISC_TmrClkSource(TIMER_T *pInst, MC_EADA_E Ctrl);
void     MCMISC_SpiClkSource(SPI_T *pInst, MC_EADA_E Ctrl);
void     MCMISC_UartClkSource(UART_T *pInst, MC_EADA_E Ctrl);
void     MCMISC_EcapClkSource(ECAP_T *pInst, MC_EADA_E Ctrl);
void     MCMISC_EpwmClkSource(EPWM_T *pInst, MC_EADA_E Ctrl);
void     MCMISC_EadcClkSource(EADC_T *pInst, MC_EADA_E Ctrl);
uint32_t MCMISC_BitToNum(uint32_t Bit);

#endif /* __MC_MISC_H__ */

/* End of mc_misc.h */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Dwt_driver.h
  * @brief   DWT (Data Watchpoint and Trace) timer driver header file
  ******************************************************************************
  */

#ifndef __DWT_DRIVER_H
#define __DWT_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
/**
  * @brief  Initialize DWT timer
  * @retval None
  */
void DWT_Init(void);

/**
  * @brief  Delay in microseconds using DWT
  * @param  us: Delay time in microseconds
  * @retval None
  */
void DWT_Delay_us(uint32_t us);

/**
  * @brief  Start counting: Clear CYCCNT register
  * @retval None
  */
uint32_t DwtStartCount(void);

/**
  * @brief  End counting: Get current CNT value and convert to microseconds
  * @retval Current time in microseconds
  */
uint32_t DwtEndCount(uint32_t start_cycles);

uint32_t DwtTimeOutCheck(uint32_t start_cycles, uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* __DWT_DRIVER_H */


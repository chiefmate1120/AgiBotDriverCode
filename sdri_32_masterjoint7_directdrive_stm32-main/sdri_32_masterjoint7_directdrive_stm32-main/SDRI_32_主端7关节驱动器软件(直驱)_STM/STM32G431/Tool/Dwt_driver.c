/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Dwt_driver.c
  * @brief   DWT (Data Watchpoint and Trace) timer driver implementation
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Dwt_driver.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint8_t dwt_initialized = 0;
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize DWT timer
  * @retval None
  */
void DWT_Init(void)
{
  /* Check if DWT is already initialized */
  if (dwt_initialized)
  {
    return;
  }

  /* Enable TRC (Trace and Debug Control) */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  /* Clear CYCCNT register */
  DWT->CYCCNT = 0;

  /* Enable CYCCNT (Cycle Counter) */
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  /* Mark as initialized */
  dwt_initialized = 1;
}

/**
  * @brief  Delay in microseconds using DWT
  * @param  us: Delay time in microseconds
  * @retval None
  */
void DWT_Delay_us(uint32_t us)
{
  uint32_t start_tick, end_tick, delay_ticks;

  /* Initialize DWT if not already done */
  if (!dwt_initialized)
  {
    DWT_Init();
  }

  /* Calculate delay in cycles */
  /* SystemCoreClock is in Hz, so cycles per microsecond = SystemCoreClock / 1000000 */
  delay_ticks = (us * (SystemCoreClock / 1000000U));

  /* Get current cycle count */
  start_tick = DWT->CYCCNT;

  /* Wait until the delay has elapsed */
  do
  {
    end_tick = DWT->CYCCNT;
  } while ((end_tick - start_tick) < delay_ticks);
}

/**
  * @brief  Start counting: Clear CYCCNT register
  * @retval None
  */
uint32_t DwtStartCount(void)
{
  /* Initialize DWT if not already done */
  if (!dwt_initialized)
  {
    DWT_Init();
  }

	return DWT->CYCCNT;
}


/**
  * @brief  End counting: Get current CNT value and convert to microseconds
  * @retval Current time in microseconds
  */
uint32_t DwtEndCount(uint32_t start_cycles)
{

  uint32_t current_cycles;
  uint32_t temp;
  /* Initialize DWT if not already done */
  if (!dwt_initialized)
  {
    DWT_Init();
  }

  /* Get current cycle count */
  current_cycles = DWT->CYCCNT;

  /* Convert cycles to microseconds */
  /* SystemCoreClock is in Hz, so microseconds = (cycles * 1000000) / SystemCoreClock */
  /* Use 64-bit calculation to avoid overflow */
//   temp = (uint64_t)((current_cycles-start_cycles) * 1000000ULL) / SystemCoreClock;
  temp = (current_cycles-start_cycles);

  return (uint32_t)temp;
}


uint32_t DwtTimeOutCheck(uint32_t start_cycles, uint32_t us)
{

  uint32_t current_cycles;
  uint32_t temp;
  /* Initialize DWT if not already done */
  if (!dwt_initialized)
  {
    DWT_Init();
  }

  /* Get current cycle count */
  current_cycles = DWT->CYCCNT;

  /* Convert cycles to microseconds */
  /* SystemCoreClock is in Hz, so microseconds = (cycles * 1000000) / SystemCoreClock */
  /* Use 64-bit calculation to avoid overflow */
//   temp = (uint64_t)((current_cycles-start_cycles) * 1000000ULL) / SystemCoreClock;
  temp = (current_cycles-start_cycles);
	
	if(temp > us * 170)
	{
		return 1;
	}
	
	return 0;

}

/* USER CODE END */


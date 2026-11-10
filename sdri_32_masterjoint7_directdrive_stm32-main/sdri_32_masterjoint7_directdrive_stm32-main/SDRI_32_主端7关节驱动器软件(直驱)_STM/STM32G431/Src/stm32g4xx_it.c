/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor_control.h"
#include "ax58200_protocol.h"
#include "MotorInterface.h"
#include "Enc_Config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/
extern unsigned char SetZeroFlag,TxLength,Rxlength,Received,SetZeroValue;
extern unsigned char usart_rx[10],Aligned,usart_cmd;
int position_div8,position_div12;
extern void SetZeroAngle(void);

int EncState;
uint32_t EncCommunicationCount;
/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

extern volatile uint32_t SpiRunCount;
/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */
	HAL_SPI_DMAStop(&hspi1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
	control_value_task();
	HAL_SPI_TransmitReceive_DMA(&hspi1, TxBuf, RxBuf, PROTOCO_SIZE);
	
	__HAL_DMA_DISABLE_IT(&hdma_spi1_rx,DMA_IT_HT|DMA_IT_TE);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
	SpiRunCount++;
  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/* USER CODE BEGIN 1 */


extern volatile uint32_t EncRunCount;
/**
  * @brief This function handles DMA1 channel3 global interrupt.
  */
void DMA1_Channel3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel3_IRQn 0 */
  /* USER CODE END DMA1_Channel3_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA1_Channel3_IRQn 1 */
	HAL_UART_DMAStop(&huart1);
	
	uint32_t DataProcessErr;
	EncCommunicationCount = 0;
	DataProcessErr = GeneralEnc_DataProcess((GeneralEnc_t *)Enc_Array[DEFAULT_ENC]);
	if(!DataProcessErr)
	{
		position_div8 = GeneralEnc_GetAngle((GeneralEnc_t *)Enc_Array[DEFAULT_ENC]) >>  8;
		position_div12=position_div8;
		Received=1;
		User_GetMultipleLaps(position_div12);
		
		EncState = (0x3 & (GeneralEnc_GetData(Enc_Array[DEFAULT_ENC], Enc_Cmd_User4) >> 6));

	}
	else if(DataProcessErr > 1000)
	{
		EncState = EncState | (1<<2);
	}
	
	EncRunCount++;

  /* USER CODE END DMA1_Channel3_IRQn 1 */
}
extern unsigned char Iq_data_save;
extern void IQ_save(void);
/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Channel4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */
  /* USER CODE END DMA1_Channel4_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_usart1_tx);

  /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

  /* USER CODE END DMA1_Channel4_IRQn 1 */
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

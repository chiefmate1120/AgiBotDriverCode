#include "TimerTask.h"
#include "main.h"
#include "regular_conversion_manager.h"
#include "Dwt_driver.h"

RegConv_t regHallAConv;
RegConv_t regHallBConv;

void HallAdc_RegisterConv(void)
{
	uint8_t Handle;
	
	regHallAConv.regADC = ADC1;
	regHallAConv.samplingTime = ADC_SAMPLETIME_2CYCLES_5;
	regHallAConv.channel = (uint8_t)3;
	Handle = RCM_RegisterRegConv(&regHallAConv);
	RCM_ExecRegularConv(Handle);
	
	regHallBConv.regADC = ADC1;
	regHallBConv.samplingTime = ADC_SAMPLETIME_2CYCLES_5;
	regHallBConv.channel = (uint8_t)4;
	Handle = RCM_RegisterRegConv(&regHallBConv);
	RCM_ExecRegularConv(Handle);
}

uint16_t ADC_ReadChannelStart(ADC_HandleTypeDef *hadc, uint32_t channel)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  
  // ADC转换通道
  sConfig.Channel = channel;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;  // �ɸ�����Ҫ��������ʱ��
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
  {
    return 0xFFFF;  // ����ʧ��
  }
  
  // ����ADCת��
  if (HAL_ADC_Start(hadc) != HAL_OK)
  {
    return 0xFFFF;  // ����ʧ��
  }
 
	return 0;
}

uint8_t ADC_ReadChannelEnd(ADC_HandleTypeDef *hadc, uint16_t *adc_value)
{

	if(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC) != RESET)
	{
		HAL_ADCEx_RegularStop(hadc);  // ��ʱ��ֹͣADC
		*adc_value = HAL_ADC_GetValue(hadc);  
		return 0x01;
	}
  return 0x00;
}


uint16_t Adc_Buff[2];
extern uint8_t motor_state;
extern ADC_HandleTypeDef hadc1;
uint8_t adcTaskFlag;


void Adc_ConvTask(void)
{
	static int count = 0;
	static int conv_flag = 0;
	static uint8_t pre_motor_state = 0;
	uint16_t *tmp16_t;
	
	typedef enum
	{
		Conv_End,
		Conv_Run
	}ConvFlag;
	
	if(pre_motor_state != motor_state && pre_motor_state == 0)
	{
	  ADC_InjectionConfTypeDef sConfigInjected = {0};

		HAL_ADCEx_RegularStop(&hadc1);
		HAL_ADC_DeInit(&hadc1);
 /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_LEFT;
  hadc1.Init.GainCompensation = 0;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Injected Channel
  */
  sConfigInjected.InjectedChannel = ADC_CHANNEL_1;
  sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
  sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_6CYCLES_5;
  sConfigInjected.InjectedSingleDiff = ADC_SINGLE_ENDED;
  sConfigInjected.InjectedOffsetNumber = ADC_OFFSET_NONE;
  sConfigInjected.InjectedOffset = 0;
  sConfigInjected.InjectedNbrOfConversion = 1;
  sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
  sConfigInjected.AutoInjectedConv = DISABLE;
  sConfigInjected.QueueInjectedContext = DISABLE;
  sConfigInjected.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJEC_T1_TRGO;
  sConfigInjected.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
  sConfigInjected.InjecOversamplingMode = DISABLE;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
	}
	
	pre_motor_state = motor_state;
	if(motor_state == 1)
	{
		return ;
	}
	
	if(conv_flag == 0)
	{
		if(count % 2 == 0)
		{
			ADC_ReadChannelStart(&hadc1, ADC_CHANNEL_3);
		}
		else 
		{
			ADC_ReadChannelStart(&hadc1, ADC_CHANNEL_4);
		}
		conv_flag = 1;
	}
	else
	{
		if(count % 2 == 0)
		{
			tmp16_t = &Adc_Buff[0];

		}
		else 
		{
			tmp16_t = &Adc_Buff[1];
		}
		
		if(ADC_ReadChannelEnd(&hadc1, tmp16_t))
		{
			conv_flag = 0;
			count++;
		}
	}
}


void Adc_RcmConvTask(void)
{
	static uint32_t DwtStartTick;
	static int conv_flag = 0;
	
	if(motor_state == 1)
	{
		return ;
	}
	
	if(conv_flag == 0)
	{
		DwtStartTick = DwtStartCount();
		RCM_ExecNextConv();
		conv_flag = 1;
	}
	else if(conv_flag == 1)
	{
			if(DwtEndCount(DwtStartTick) > 17000)
			{
				conv_flag = 2;
			}
	}
	else if(conv_flag == 2)
	{
		RCM_ReadOngoingConv();
		conv_flag = 0;
	}
	
}
uint32_t HallAdcGet(uint8_t channel)
{	
	return RCM_GetRegConvValue(channel)>>4;
}

extern unsigned int ActualKeyState;

void GpioReadTask(void)
{
	static uint32_t DwtStartTick;

	//Clutch signal
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5))
	{
		ActualKeyState |= 1<<0;
	}
	else 
	{
		ActualKeyState &= ~(1<<0);
	}
	
	//Capacitive signal
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))
	{
		ActualKeyState |= 1<<1;
		DwtStartTick = DwtStartCount();
	}
	else 
	{
		if(((ActualKeyState>>1) & 0x01))
		{
			if(DwtEndCount(DwtStartTick) > 17000000)
			{
				ActualKeyState &= ~(1<<1);
			}
		}
	}
	
	
	//Infrared signal
	if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0))
	{
		ActualKeyState |= 1<<2;
	}
	else 
	{
		ActualKeyState &= ~(1<<2);
	}
}

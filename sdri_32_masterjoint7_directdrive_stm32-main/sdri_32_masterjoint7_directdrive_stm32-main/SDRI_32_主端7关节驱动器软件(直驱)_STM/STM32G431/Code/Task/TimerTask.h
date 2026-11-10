#ifndef __TIMER_TASK_H__
#define __TIMER_TASK_H__

#include <stdint.h>
#include "main.h"

void HallAdc_RegisterConv(void);
uint16_t ADC_ReadChannelStart(ADC_HandleTypeDef *hadc, uint32_t channel);
uint8_t ADC_ReadChannelEnd(ADC_HandleTypeDef *hadc, uint16_t *adc_value);
void Adc_ConvTask(void);
uint32_t HallAdcGet(uint8_t channel);
void Adc_RcmConvTask(void);
void GpioReadTask(void);

#endif

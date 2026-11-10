/**
  ******************************************************************************
  * @file    mc_config.c 
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   Motor Control Subsystem components configuration and handler structures.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */ 
#include "main.h"
#include "mc_type.h"
#include "parameters_conversion.h"
#include "mc_parameters.h"
#include "mc_config.h"

/* USER CODE BEGIN Additional include */

/* USER CODE END Additional include */ 

#define FREQ_RATIO 1                /* Dummy value for single drive */
#define FREQ_RELATION HIGHEST_FREQ  /* Dummy value for single drive */

#define OFFCALIBRWAIT_MS     0
#define OFFCALIBRWAIT_MS2    0     
#include "pqd_motor_power_measurement.h"
/* USER CODE BEGIN Additional define */

/* USER CODE END Additional define */ 

PQD_MotorPowMeas_Handle_t PQD_MotorPowMeasM1 =
{
  .wConvFact = PQD_CONVERSION_FACTOR
};
PQD_MotorPowMeas_Handle_t *pPQD_MotorPowMeasM1 = &PQD_MotorPowMeasM1; 



/**
  * @brief  Current sensor parameters Dual Drive Motor 1 - ICS, STM32G4xx
  */
PWMC_ICS_Handle_t PWM_Handle_M1 = {
  {
    .pFctGetPhaseCurrents              = &ICS_GetPhaseCurrents,    
    .pFctSwitchOffPwm                  = &ICS_SwitchOffPWM,             
    .pFctSwitchOnPwm                   = &ICS_SwitchOnPWM,              
    .pFctCurrReadingCalib              = &ICS_CurrentReadingPolarization,
    .pFctTurnOnLowSides                = &ICS_TurnOnLowSides,         
    .pFctSetADCSampPointSectX          = &ICS_WriteTIMRegisters,            
    .pFctIsOverCurrentOccurred         = &ICS_IsOverCurrentOccurred,    
    .pFctOCPSetReferenceVoltage        = MC_NULL,
    .pFctRLDetectionModeEnable         = MC_NULL,    
    .pFctRLDetectionModeDisable        = MC_NULL,   
    .pFctRLDetectionModeSetDuty        = MC_NULL,   
    .hT_Sqrt3 = (PWM_PERIOD_CYCLES*SQRT3FACTOR)/16384u,  
    .Sector = 0,   
    .CntPhA = 0,
    .CntPhB = 0,
    .CntPhC = 0,
    .SWerror = 0,
    .TurnOnLowSidesAction = false, 
    .OffCalibrWaitTimeCounter = 0, 
    .Motor = M1,     
    .RLDetectionMode = false, 
    .Ia = 0,
    .Ib = 0,
    .Ic = 0,
    .DTTest = 0,   
    .DTCompCnt = DTCOMPCNT,
    .PWMperiod          = PWM_PERIOD_CYCLES,                                                  
    .OffCalibrWaitTicks = (uint16_t)((SYS_TICK_FREQUENCY * OFFCALIBRWAIT_MS)/ 1000),                                                                                                                                                       
    .Ton                 = TON,                             
    .Toff                = TOFF                             
  
  },
  .PhaseAOffset = 0,   
  .PhaseBOffset = 0,  
  .Half_PWMPeriod = PWM_PERIOD_CYCLES/2u,
  .PolarizationCounter = 0, 
  .OverCurrentFlag = false,    
  .OverVoltageFlag = false,    
  .BrakeActionLock = false,    
                                                            
  .pParams_str = &ICS_ParamsM1                               
};
  
  
  
/**
  * @brief  SpeedNPosition sensor parameters Motor 1 - Base Class
  */
VirtualSpeedSensor_Handle_t VirtualSpeedSensorM1 =
{
  
  ._Super = {
    .bElToMecRatio                     =	POLE_PAIR_NUM, 
    .hMaxReliableMecSpeedUnit          =	(uint16_t)(1.15*MAX_APPLICATION_SPEED_UNIT),
    .hMinReliableMecSpeedUnit          =	(uint16_t)(MIN_APPLICATION_SPEED_UNIT),
    .bMaximumSpeedErrorsNumber         =	MEAS_ERRORS_BEFORE_FAULTS,      
    .hMaxReliableMecAccelUnitP         =	65535,                             
    .hMeasurementFrequency             =	TF_REGULATION_RATE_SCALED,
    .DPPConvFactor                     =  DPP_CONV_FACTOR,       
    },
  .hSpeedSamplingFreqHz =	MEDIUM_FREQUENCY_TASK_RATE, 
  .hTransitionSteps     =	(int16_t)(TF_REGULATION_RATE * TRANSITION_DURATION/ 1000.0),
                           
};

/**
  * @brief  SpeedNPosition sensor parameters Motor 1 - Encoder
  */
ENCODER_Handle_t ENCODER_M1 =
{
  ._Super = {
    .bElToMecRatio                     =	POLE_PAIR_NUM,              
    .hMaxReliableMecSpeedUnit          =	(uint16_t)(1.15*MAX_APPLICATION_SPEED_UNIT),
    .hMinReliableMecSpeedUnit          =	(uint16_t)(MIN_APPLICATION_SPEED_UNIT),
    .bMaximumSpeedErrorsNumber         =	MEAS_ERRORS_BEFORE_FAULTS,         
    .hMaxReliableMecAccelUnitP         =	65535,                               
    .hMeasurementFrequency             =	TF_REGULATION_RATE_SCALED,
    .DPPConvFactor                     =  DPP_CONV_FACTOR,       
  },  
  .PulseNumber           =	M1_ENCODER_PPR*4,		 
  .RevertSignal           =	(FunctionalState)ENC_INVERT_SPEED,
  .SpeedSamplingFreqHz   =	MEDIUM_FREQUENCY_TASK_RATE, 
  .SpeedBufferSize       =	ENC_AVERAGING_FIFO_DEPTH,  
  .TIMx                  =	TIM4,	    
  .ICx_Filter            =  M1_ENC_IC_FILTER,

};

/**
  * @brief  Encoder Alignment Controller parameters Motor 1
  */
EncAlign_Handle_t EncAlignCtrlM1 =
{
  .hEACFrequencyHz =	MEDIUM_FREQUENCY_TASK_RATE,
  .hFinalTorque    =	FINAL_I_ALIGNMENT,            
  .hElAngle        =	ALIGNMENT_ANGLE_S16,      
  .hDurationms     =	ALIGNMENT_DURATION,              
  .bElToMecRatio   =	POLE_PAIR_NUM,            
};

/**
  * Virtual temperature sensor parameters Motor 1
  */
NTC_Handle_t TempSensorParamsM1 =
{
  .bSensorType = VIRTUAL_SENSOR,
  .hExpectedTemp_d = 555, 
  .hExpectedTemp_C = M1_VIRTUAL_HEAT_SINK_TEMPERATURE_VALUE,
};

/**
  * Virtual bus voltage sensor parameters Motor 1
  */
VirtualBusVoltageSensor_Handle_t VirtualBusVoltageSensorParamsM1 =
{
  ._Super =
  {
    .SensorType       = VIRTUAL_SENSOR,                 
    .ConversionFactor = 500,                             
  },
  .ExpectedVbus_d = 1 + (NOMINAL_BUS_VOLTAGE_V * 65536) / 500,                                                      
};

UI_Handle_t UI_Params =
{
  .bDriveNum = 0,
};

/** RAMP for Motor1.
  *
  */
RampExtMngr_Handle_t RampExtMngrHFParamsM1 =
{
  .FrequencyHz = TF_REGULATION_RATE 
};

/**
  * @brief  CircleLimitation Component parameters Motor 1 - Base Component
  */
CircleLimitation_Handle_t CircleLimitationM1 =
{
  .MaxModule          = MAX_MODULE,
  .MaxVd          	  = (uint16_t)(MAX_MODULE * 950 / 1000),
  .Circle_limit_table = MMITABLE,        	
  .Start_index        = START_INDEX, 		
};

UFCP_Handle_t pUSART =
{
  ._Super.RxTimeout = 0,
  .USARTx = USART2,
       
};
		

/* USER CODE BEGIN Additional configuration */

/* USER CODE END Additional configuration */ 

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/


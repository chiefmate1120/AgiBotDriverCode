#include "MotorInterface.h"
#include "parameters_conversion.h"
/* Pid Init  */
/*************************************************************************************************************************************/
PID_Handle_t PIDIqHandle_M1 =
{
  .hDefKpGain          = (int16_t)CURRENT_KP,
  .hDefKiGain          = (int16_t)CURRENT_KI,
  .wUpperIntegralLimit = (int32_t)CURRENT_MAXOUTPUT * TF_KIDIV,
  .wLowerIntegralLimit = (int32_t)-CURRENT_MAXOUTPUT * TF_KIDIV,   
  .hUpperOutputLimit       = CURRENT_INTERGRAL_LIMIT_MAX,     
  .hLowerOutputLimit       = -CURRENT_INTERGRAL_LIMIT_MAX,           
  .hKpDivisor          = (uint16_t)TF_KPDIV,       
  .hKiDivisor          = (uint16_t)TF_KIDIV,       
  .hKpDivisorPOW2      = (uint16_t)TF_KPDIV_LOG,       
  .hKiDivisorPOW2      = (uint16_t)TF_KIDIV_LOG,        
  .hDefKdGain           = 0x0000U,
  .hKdDivisor           = 0x0000U,
  .hKdDivisorPOW2       = 0x0000U,
};

/**
  * @brief  PI / PID Id loop parameters Motor 1
  */
PID_Handle_t PIDIdHandle_M1 =
{
  .hDefKpGain          = (int16_t)PID_FLUX_KP_DEFAULT, 
  .hDefKiGain          = (int16_t)PID_FLUX_KI_DEFAULT, 
  .wUpperIntegralLimit = (int32_t)INT16_MAX * TF_KIDIV, 
  .wLowerIntegralLimit = (int32_t)-INT16_MAX * TF_KIDIV,
  .hUpperOutputLimit       = INT16_MAX,                 
  .hLowerOutputLimit       = -INT16_MAX,                
  .hKpDivisor          = (uint16_t)TF_KPDIV,          
  .hKiDivisor          = (uint16_t)TF_KIDIV,          
  .hKpDivisorPOW2      = (uint16_t)TF_KPDIV_LOG,       
  .hKiDivisorPOW2      = (uint16_t)TF_KIDIV_LOG,       
  .hDefKdGain           = 0x0000U,
  .hKdDivisor           = 0x0000U,
  .hKdDivisorPOW2       = 0x0000U,
};

PID_Handle_t PIDSpeedHandle_M1 =
{
  .hDefKpGain          = (int16_t)SPEED_KP,
  .hDefKiGain          = (int16_t)SPEED_KI, 
  .wUpperIntegralLimit = (int32_t)SPEED_INTERGRAL_LIMIT_MAX * (int32_t)SP_KIDIV,
  .wLowerIntegralLimit = -(int32_t)SPEED_INTERGRAL_LIMIT_MAX * (int32_t)SP_KIDIV,
  .hUpperOutputLimit       = (int16_t)SPEED_MAXOUTPUT, 
  .hLowerOutputLimit       = -(int16_t)SPEED_MAXOUTPUT,
  .hKpDivisor          = (uint16_t)SP_KPDIV,
  .hKiDivisor          = (uint16_t)SP_KIDIV,
  .hKpDivisorPOW2      = (uint16_t)SP_KIDIV_LOG,
  .hKiDivisorPOW2      = (uint16_t)SP_KIDIV_LOG,
  .hDefKdGain           = SPEED_KD,
  .hKdDivisor           = (uint16_t)SP_KPDIV,
  .hKdDivisorPOW2       = (uint16_t)SP_KPDIV_LOG,
};

//definition Position Pid
PID_Handle_t PIDPositionHandle_M1 =
{
  .hKpGain          = (int16_t)POSITION_KP,
  .hKiGain          = (int16_t)POSITION_KI, 
  .wUpperIntegralLimit = (int32_t)((POSITION_INTERGRAL_LIMIT_MAX) * (int32_t)P_KIDIV),
  .wLowerIntegralLimit = -(int32_t)((POSITION_INTERGRAL_LIMIT_MAX) * (int32_t)P_KIDIV),
  .hUpperOutputLimit       = (int16_t)(POSITION_MAXOUTPUT), 
  .hLowerOutputLimit       = -(int16_t)(POSITION_MAXOUTPUT),	
  .hKpDivisor          = (uint16_t)P_KPDIV,
  .hKiDivisor          = (uint16_t)P_KIDIV,
  .hKpDivisorPOW2      = (uint16_t)P_KPDIV_LOG,
  .hKiDivisorPOW2      = (uint16_t)P_KIDIV_LOG,
  .hKdGain           		= POSITION_KD,
  .hKdDivisor           = P_KDDIV,
  .hKdDivisorPOW2       = P_KDDIV_LOG,
};

/**
  * @brief  SpeednTorque Controller parameters Motor 1
  */
SpeednTorqCtrl_Handle_t SpeednTorqCtrlM1 =
{
  .STCFrequencyHz =           		MEDIUM_FREQUENCY_TASK_RATE, 	 
  .MaxAppPositiveMecSpeedUnit =	(uint16_t)(MAX_APPLICATION_SPEED_UNIT), 
  .MinAppPositiveMecSpeedUnit =	(uint16_t)(MIN_APPLICATION_SPEED_UNIT), 
  .MaxAppNegativeMecSpeedUnit =	(int16_t)(-MIN_APPLICATION_SPEED_UNIT), 
  .MinAppNegativeMecSpeedUnit =	(int16_t)(-MAX_APPLICATION_SPEED_UNIT),
  .MaxPositiveTorque =				(int16_t)SPEED_MAXOUTPUT,		 
  .MinNegativeTorque =				-(int16_t)SPEED_MAXOUTPUT,       
  .ModeDefault =					DEFAULT_CONTROL_MODE,            
  .MecSpeedRefUnitDefault =		(int16_t)(DEFAULT_TARGET_SPEED_UNIT),
  .TorqueRefDefault =				(int16_t)DEFAULT_TORQUE_COMPONENT,
  .IdrefDefault =					(int16_t)DEFAULT_FLUX_COMPONENT,                                                                     
};

/*************************************************************************************************************************************/

/* Position loop function */
/*************************************************************************************************************************************/
int16_t Position_PI_Controller( PID_Handle_t * pHandle, int32_t wProcessVarError )
{
  int32_t wProportional_Term, wIntegral_Term, wIntegral_sum_temp;
  int64_t wOutput_32;
  int32_t wDischarge = 0;
  int16_t hUpperOutputLimit = pHandle->hUpperOutputLimit;
  int16_t hLowerOutputLimit = pHandle->hLowerOutputLimit;

  /* Proportional term computation*/
  wProportional_Term = pHandle->hKpGain * wProcessVarError;

  /* Integral term computation */
  if ( pHandle->hKiGain == 0 )
  {
    pHandle->wIntegralTerm = 0;
  }
  else
  {
    wIntegral_Term = pHandle->hKiGain * wProcessVarError;
    wIntegral_sum_temp = pHandle->wIntegralTerm + wIntegral_Term;

    if ( wIntegral_sum_temp < 0 )
    {
      if ( pHandle->wIntegralTerm > 0 )
      {
        if ( wIntegral_Term > 0 )
        {
          wIntegral_sum_temp = INT32_MAX;
        }
      }
    }
    else
    {
      if ( pHandle->wIntegralTerm < 0 )
      {
        if ( wIntegral_Term < 0 )
        {
          wIntegral_sum_temp = -INT32_MAX;
        }
      }
    }

    if ( wIntegral_sum_temp > pHandle->wUpperIntegralLimit )
    {
      pHandle->wIntegralTerm = pHandle->wUpperIntegralLimit;
    }
    else if ( wIntegral_sum_temp < pHandle->wLowerIntegralLimit )
    {
      pHandle->wIntegralTerm = pHandle->wLowerIntegralLimit;
    }
    else
    {
      pHandle->wIntegralTerm = wIntegral_sum_temp;
    }
  }

#ifdef FULL_MISRA_C_COMPLIANCY
  wOutput_32 = ( wProportional_Term / ( int32_t )pHandle->hKpDivisor ) + ( pHandle->wIntegralTerm /
               ( int32_t )pHandle->hKiDivisor );
#else
  /* WARNING: the below instruction is not MISRA compliant, user should verify
             that Cortex-M3 assembly instruction ASR (arithmetic shift right)
             is used by the compiler to perform the shifts (instead of LSR
             logical shift right)*/
	int64_t KpValue,KiValue;
	KpValue = ( wProportional_Term >> pHandle->hKpDivisorPOW2 );
	KiValue = ( pHandle->wIntegralTerm >> pHandle->hKiDivisorPOW2 );
  wOutput_32 = KpValue + KiValue;
#endif
  // 判断KpValue, KiValue, wOutput_32是否溢出
  if (KpValue > INT32_MAX || KpValue < INT32_MIN)
  {
    // KpValue溢出处理
    KpValue = (KpValue > 0) ? INT32_MAX : INT32_MIN;
  }
  if (KiValue > INT32_MAX || KiValue < INT32_MIN)
  {
    // KiValue溢出处理
    KiValue = (KiValue > 0) ? INT32_MAX : INT32_MIN;
  }
  if (wOutput_32 > INT32_MAX || wOutput_32 < INT32_MIN)
  {
    // wOutput_32溢出处理
    wOutput_32 = (wOutput_32 > 0) ? INT32_MAX : INT32_MIN;
  }


  if ( wOutput_32 > hUpperOutputLimit )
  {

    wDischarge = hUpperOutputLimit - wOutput_32;
    wOutput_32 = hUpperOutputLimit;
  }
  else if ( wOutput_32 < hLowerOutputLimit )
  {

    wDischarge = hLowerOutputLimit - wOutput_32;
    wOutput_32 = hLowerOutputLimit;
  }
  else { /* Nothing to do here */ }

	if(KpValue > 0 && ((KpValue) > pHandle->wUpperIntegralLimit))
	{	
		wDischarge = 0;
		pHandle->wIntegralTerm = 0;
		wOutput_32 = (KpValue );
	}
	else if(KpValue < 0 && ((KiValue) < pHandle->wLowerIntegralLimit))
	{	
		wDischarge = 0;
		pHandle->wIntegralTerm = 0;
		wOutput_32 = (KpValue );
	}
	
  pHandle->wIntegralTerm += wDischarge;
	
  return ( ( int16_t )( wOutput_32 ) );
}


extern int TargetPosition1;
extern int ActualPosition;
extern int TargetPosition;
extern unsigned char PositionIqStart;
int PositionVelocity(PID_Handle_t *pHandle)
{
	int wPositionError;
	int hTargetSpeed;
	int ErrLimit;
	
	ErrLimit = 65536 * 2;
	if(PositionIqStart==0)
		wPositionError=TargetPosition-ActualPosition;
	else
		wPositionError=(TargetPosition1<<4)-ActualPosition;
	
	if(wPositionError > ErrLimit)
	{
		wPositionError = ErrLimit;
	}
	else if(wPositionError < -ErrLimit)
	{
		wPositionError = -ErrLimit;
	}
	
	hTargetSpeed=PID_Controller( pHandle, wPositionError );
	
	return hTargetSpeed;
}

//abstract PositionLoop
extern int TargetTorque;

void PositionLoop_Contorl(void)
{
	int PositionTorque;
	
	PositionTorque = PositionVelocity(&PIDPositionHandle_M1);
	PositionTorque = PositionTorque * CURRENT_GAIN;
	if(PositionTorque > INT16_MAX)
	{
		PositionTorque = INT16_MAX;
	}
	else if(PositionTorque < -INT16_MAX)
	{
		PositionTorque = -INT16_MAX;
	}
	
	MC_ProgramTorqueRampMotor1(PositionTorque,0);

}

extern int position_div12;
extern unsigned int position_div8;
int GetEncoderPosition(void)
{
  return position_div12;
}
extern uint8_t AlignmentCalibrationFlag;

int16_t User_GetElAngle( void )
{
    #if MOTOR_CURRENT_BANDWIDTH_ELANGLE_ZERO == MOTOR_TEST_ENABLE
    return 0;
    #else
    uint32_t rawAngle;
    #if CURRENT_DIR == DIR_POSITIVE
        rawAngle = (uint32_t)(GetEncoderPosition() * MOTOR_POLE_PAIR_NUM) & 0xFFFF;
    #else
        rawAngle = (uint32_t)(-GetEncoderPosition() * MOTOR_POLE_PAIR_NUM) & 0xFFFF;
    #endif
    /* 先加 16384 再取模，保证结果在 0–65535，再转为 int16_t，避免溢出跳变 */
    rawAngle = (rawAngle + 16384u) & 0xFFFF;
    return (int16_t)rawAngle;
    #endif
}


extern unsigned char usart_rx[10];
extern unsigned char SetZeroFlag,TxLength,Rxlength,Received,SetZeroValue;
extern int ENCCount;
extern int PositionOffset;
extern unsigned int position_div8;
void User_GetMultipleLaps(int32_t CurrentPos)
{
	int pos;
	int temp;
	static unsigned int position_pre;
	
	#if POSITION_DIR == DIR_POSITIVE
	pos=CurrentPos;
	#else
	pos=-CurrentPos;
	#endif

	
	temp=pos-position_pre;
	if(temp>32767)
	{
		ENCCount-=1;
	}
	else if(temp<-32767)
	{
		ENCCount+=1;
	}
	ActualPosition=65536*ENCCount+pos-PositionOffset;
	position_pre=pos;
}


/*************************************************************************************************************************************/
//abstract GetEncoderPosition To Calculate Speed

//overwrite ENC_Init Function
void ENC_Init( ENCODER_Handle_t * pHandle )
{

  uint8_t BufferSize;
  uint8_t Index;
	
  /*Calculations of convenience*/
  pHandle->U32MAXdivPulseNumber = UINT32_MAX / ( uint32_t )( pHandle->PulseNumber );
  pHandle->SpeedSamplingFreqUnit = pHandle->SpeedSamplingFreqHz * SPEED_UNIT;
	
//  /* Erase speed buffer */
  BufferSize = pHandle->SpeedBufferSize;

  for ( Index = 0u; Index < BufferSize; Index++ )
  {
    pHandle->DeltaCapturesBuffer[Index] = 0;
  }
}

//overwrite ENC_CalcAvrgMecSpeedUnit Function
bool ENC_CalcAvrgMecSpeedUnit( ENCODER_Handle_t * pHandle, int16_t * pMecSpeedUnit )
{
  int32_t wOverallAngleVariation = 0;
  int32_t wtemp1,temp;
  int32_t wtemp2;
  uint8_t bBufferIndex = 0u;
  bool bReliability = true;
  uint8_t bBufferSize = pHandle->SpeedBufferSize;
  uint32_t CntCapture;
  uint32_t directionSample;
  uint8_t OFbit = 0;

	__disable_irq();
  CntCapture =  GetEncoderPosition();
	__enable_irq();
	
#ifdef TIM_CNT_UIFCPY
  temp=CntCapture-pHandle->PreviousCapture;
	
  if(temp<-ENCODER_PULSE_RESOLUTION * 0.8 || temp>ENCODER_PULSE_RESOLUTION * 0.8)
  {
	  OFbit=1;
  }
  else
  {
      OFbit=0;
  }
  if(((temp>=0)&&(OFbit==0))||((temp<0)&&(OFbit==1)))
  {
	  directionSample=1;
  }
  else
  {
	  directionSample=0;
  }
	  
#endif

  /*Calculation of delta angle*/
  if ( directionSample == 0 )
  {
    /* encoder timer down-counting*/
    /* if UIFCPY not present Overflow counter can not be safely used -> limitation to 1 OF. */
    pHandle->DeltaCapturesBuffer[pHandle->DeltaCapturesIndex] = temp - OFbit  * ENCODER_PULSE_RESOLUTION;
  }
  else
  {
    /* encoder timer up-counting*/
    /* if UIFCPY not present Overflow counter can not be safely used -> limitation to 1 OF. */
    pHandle->DeltaCapturesBuffer[pHandle->DeltaCapturesIndex] = temp + OFbit * ENCODER_PULSE_RESOLUTION;

  }


  /*Computes & returns average mechanical speed */
  for ( bBufferIndex = 0u; bBufferIndex < bBufferSize; bBufferIndex++ )
  {
    wOverallAngleVariation += pHandle->DeltaCapturesBuffer[bBufferIndex];
  }
  wtemp1 = wOverallAngleVariation * ( int32_t )( pHandle->SpeedSamplingFreqUnit );
  wtemp2 = ENCODER_PULSE_RESOLUTION *
           ( int32_t )( pHandle->SpeedBufferSize );
  wtemp1 /= wtemp2;

  /*Computes & stores average mechanical acceleration */


  /*Stores average mechanical speed */
	#if SPEED_DIR == DIR_POSITIVE
	*pMecSpeedUnit = ( int16_t )( wtemp1 );
	pHandle->_Super.hMecAccelUnitP = ( int16_t )( wtemp1 -
																 pHandle->_Super.hAvrMecSpeedUnit );
	pHandle->_Super.hAvrMecSpeedUnit = ( int16_t )wtemp1;
	#else
	*pMecSpeedUnit = -( int16_t )( wtemp1 );
	pHandle->_Super.hMecAccelUnitP = -( int16_t )( wtemp1 -
																 pHandle->_Super.hAvrMecSpeedUnit );
	pHandle->_Super.hAvrMecSpeedUnit = -( int16_t )wtemp1;
	#endif
	
  /*Computes & stores the instantaneous electrical speed [dpp], var wtemp1*/
  wtemp1 = pHandle->DeltaCapturesBuffer[pHandle->DeltaCapturesIndex] *
           ( int32_t )( pHandle->SpeedSamplingFreqHz ) *
           ( int32_t )pHandle->_Super.bElToMecRatio;
  wtemp1 /= ENCODER_PULSE_RESOLUTION;//( int32_t )( pHandle->PulseNumber );
  wtemp1 *= ( int32_t )( pHandle->_Super.DPPConvFactor);
  wtemp1 /= ( int32_t )( pHandle->_Super.hMeasurementFrequency );

	#if SPEED_DIR == DIR_POSITIVE
	pHandle->_Super.hElSpeedDpp = ( int16_t )wtemp1;
	
	#else
	pHandle->_Super.hElSpeedDpp = -( int16_t )wtemp1;
	#endif

  /*last captured value update*/
  pHandle->PreviousCapture = CntCapture;
  /*Buffer index update*/
  pHandle->DeltaCapturesIndex++;

  if ( pHandle->DeltaCapturesIndex == pHandle->SpeedBufferSize )
  {
    pHandle->DeltaCapturesIndex = 0u;
  }
	 
		
  return ( bReliability );
}



//overwrite PI_Controller Function
int16_t PI_Controller( PID_Handle_t * pHandle, int32_t wProcessVarError )
{
  int32_t wProportional_Term, wIntegral_Term, wOutput_32, wIntegral_sum_temp;
  int16_t hUpperOutputLimit = pHandle->hUpperOutputLimit;
  int16_t hLowerOutputLimit = pHandle->hLowerOutputLimit;

  /* Proportional term computation*/
  wProportional_Term = pHandle->hKpGain * wProcessVarError;

  /* Integral term computation */
  if ( pHandle->hKiGain == 0 )
  {
    pHandle->wIntegralTerm = 0;
  }
  else
  {
    wIntegral_Term = pHandle->hKiGain * wProcessVarError;
    wIntegral_sum_temp = pHandle->wIntegralTerm + wIntegral_Term;

    if ( wIntegral_sum_temp < 0 )
    {
      if ( pHandle->wIntegralTerm > 0 )
      {
        if ( wIntegral_Term > 0 )
        {
          wIntegral_sum_temp = INT32_MAX;
        }
      }
    }
    else
    {
      if ( pHandle->wIntegralTerm < 0 )
      {
        if ( wIntegral_Term < 0 )
        {
          wIntegral_sum_temp = -INT32_MAX;
        }
      }
    }

    if ( wIntegral_sum_temp > pHandle->wUpperIntegralLimit )
    {
      pHandle->wIntegralTerm = pHandle->wUpperIntegralLimit;
			
    }
    else if ( wIntegral_sum_temp < pHandle->wLowerIntegralLimit )
    {
      pHandle->wIntegralTerm = pHandle->wLowerIntegralLimit;
    }
    else
    {
      pHandle->wIntegralTerm = wIntegral_sum_temp;
    }
  }

#ifdef FULL_MISRA_C_COMPLIANCY
  wOutput_32 = ( wProportional_Term / ( int32_t )pHandle->hKpDivisor ) + ( pHandle->wIntegralTerm /
               ( int32_t )pHandle->hKiDivisor );
#else
  /* WARNING: the below instruction is not MISRA compliant, user should verify
             that Cortex-M3 assembly instruction ASR (arithmetic shift right)
             is used by the compiler to perform the shifts (instead of LSR
             logical shift right)*/
  wOutput_32 = ( wProportional_Term >> pHandle->hKpDivisorPOW2 ) + ( pHandle->wIntegralTerm >> pHandle->hKiDivisorPOW2 );
#endif

  if ( wOutput_32 > hUpperOutputLimit )
  {
    /* Output overflow detected - apply anti-windup by setting integral term */
    /* Integral term = (Limited Output - Proportional Output) * KiDivisor */
    int32_t wProportional_Output;
#ifdef FULL_MISRA_C_COMPLIANCY
    wProportional_Output = wProportional_Term / ( int32_t )pHandle->hKpDivisor;
#else
    wProportional_Output = wProportional_Term >> pHandle->hKpDivisorPOW2;
#endif
    /* Calculate the required integral term to make output = limit */
    int32_t wRequired_Integral_Output = ( int32_t )hUpperOutputLimit - wProportional_Output;
    /* Convert back to integral term value (before division) */
#ifdef FULL_MISRA_C_COMPLIANCY
    pHandle->wIntegralTerm = wRequired_Integral_Output * ( int32_t )pHandle->hKiDivisor;
#else
    pHandle->wIntegralTerm = wRequired_Integral_Output << pHandle->hKiDivisorPOW2;
#endif
    /* Clamp integral term to limits */
		if(pHandle->wIntegralTerm < 0)
		{
			pHandle->wIntegralTerm = 0;
		}
		
    if ( pHandle->wIntegralTerm > pHandle->wUpperIntegralLimit )
    {
      pHandle->wIntegralTerm = pHandle->wUpperIntegralLimit;
    }
    else if ( pHandle->wIntegralTerm < pHandle->wLowerIntegralLimit )
    {
      pHandle->wIntegralTerm = pHandle->wLowerIntegralLimit;
    }
    wOutput_32 = hUpperOutputLimit;
  }
  else if ( wOutput_32 < hLowerOutputLimit )
  {
    /* Output underflow detected - apply anti-windup by setting integral term */
    /* Integral term = (Limited Output - Proportional Output) * KiDivisor */
    int32_t wProportional_Output;
#ifdef FULL_MISRA_C_COMPLIANCY
    wProportional_Output = wProportional_Term / ( int32_t )pHandle->hKpDivisor;
#else
    wProportional_Output = wProportional_Term >> pHandle->hKpDivisorPOW2;
#endif
    /* Calculate the required integral term to make output = limit */
    int32_t wRequired_Integral_Output = ( int32_t )hLowerOutputLimit - wProportional_Output;
    /* Convert back to integral term value (before division) */
#ifdef FULL_MISRA_C_COMPLIANCY
    pHandle->wIntegralTerm = wRequired_Integral_Output * ( int32_t )pHandle->hKiDivisor;
#else
    pHandle->wIntegralTerm = wRequired_Integral_Output << pHandle->hKiDivisorPOW2;
#endif
    /* Clamp integral term to limits */
		if(pHandle->wIntegralTerm > 0)
		{
			pHandle->wIntegralTerm = 0;
		}
		
    if ( pHandle->wIntegralTerm > pHandle->wUpperIntegralLimit )
    {
      pHandle->wIntegralTerm = pHandle->wUpperIntegralLimit;
    }
    else if ( pHandle->wIntegralTerm < pHandle->wLowerIntegralLimit )
    {
      pHandle->wIntegralTerm = pHandle->wLowerIntegralLimit;
    }
    wOutput_32 = hLowerOutputLimit;
  }
  else { /* Nothing to do here */ }

  return ( ( int16_t )( wOutput_32 ) );
}


extern signed char IQ[4100];
extern unsigned int position_div8;
float ForwardSpeed = 10;
int16_t STC_CalcTorqueReference( SpeednTorqCtrl_Handle_t * pHandle )
{
  int32_t wCurrentReference;
  int16_t hTorqueReference = 0;
  int16_t hMeasuredSpeed;
  int16_t hTargetSpeed;
  int16_t hError;

  if ( pHandle->Mode == STC_TORQUE_MODE )
  {
    wCurrentReference = pHandle->TorqueRef;
  }
  else
  {
    wCurrentReference = pHandle->SpeedRefUnitExt;
  }

  /* Update the speed reference or the torque reference according to the mode
     and terminates the ramp if needed. */
  if ( pHandle->RampRemainingStep > 1u )
  {
    /* Increment/decrement the reference value. */
    wCurrentReference += pHandle->IncDecAmount;

    /* Decrement the number of remaining steps */
    pHandle->RampRemainingStep--;
  }
  else if ( pHandle->RampRemainingStep == 1u )
  {
    /* Set the backup value of hTargetFinal. */
    wCurrentReference = ( int32_t )pHandle->TargetFinal * 65536;
    pHandle->RampRemainingStep = 0u;
  }
  else
  {
    /* Do nothing. */
  }

  if ( pHandle->Mode == STC_SPEED_MODE )
  {
    /* Run the speed control loop */

    /* Compute speed error */
    hTargetSpeed = ( int16_t )( wCurrentReference / 65536 );
    hMeasuredSpeed = SPD_GetAvrgMecSpeedUnit( pHandle->SPD );
	  
	  
    hError = hTargetSpeed - hMeasuredSpeed;
    #if COGGING_FORCE_COMPENSATION_ENABLE == 1
    hTorqueReference = PI_Controller( pHandle->PISpeed, ( int32_t )hError )+(IQ[position_div8>>4]-40) * CURRENT_GAIN ;
    #else
    hTorqueReference = PID_Controller( pHandle->PISpeed, ( int32_t )hError ) + ForwardSpeed * hTargetSpeed;
    #endif

    pHandle->SpeedRefUnitExt = wCurrentReference;
    pHandle->TorqueRef = ( int32_t )hTorqueReference * 65536;
  }
  else
  {
    pHandle->TorqueRef = wCurrentReference;
    hTorqueReference = ( int16_t )( wCurrentReference / 65536 );
  }

  return hTorqueReference;
}

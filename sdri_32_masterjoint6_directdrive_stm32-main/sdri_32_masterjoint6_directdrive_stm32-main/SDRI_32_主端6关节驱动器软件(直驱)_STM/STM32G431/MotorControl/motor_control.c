#include "stm32g4xx_hal.h"
#include "motor_control.h"
#include "mc_type.h"
#include "mc_math.h"
#include "motorcontrol.h"
#include "regular_conversion_manager.h"
#include "mc_interface.h"
#include "mc_tuning.h"
#include "digital_output.h"
#include "state_machine.h"
#include "pwm_common.h"

#include "mc_tasks.h"
#include "parameters_conversion.h"
#include "ax58200_protocol.h"
#include "MotorInterface.h"

#include "MotorInterface.h"
#include "Dwt_driver.h"
#include <string.h>
#include "Enc_Config.h"
#include "Motor_Test.h"

uint8_t StMotorState;
uint8_t NaSingle;


extern UART_HandleTypeDef huart2;
extern unsigned char usart_rx[10];
unsigned char usart_cmd;
int targetspeed,position_error;

extern EncAlign_Handle_t EncAlignCtrlM1;
extern UART_HandleTypeDef huart1;

unsigned char ENCAlignment;
unsigned char PositionIqStart;
unsigned char PositionIqEn;
short TargetSpeed,TargetSpeed_pre;
int velocityreference;
int TargetRampPosition;
int ENCOffset;

extern signed int Iq;
extern signed char IQ[4100];
extern void IQ_save(void);

unsigned char Rxdata[5];
unsigned char Txdata[5];
unsigned char RxFlag;
unsigned char RxTemp;

unsigned char TxBuf[50];
unsigned char RxBuf[50];

int ENCCount=0;

unsigned int RxBufTemp;

int Iq_temp;

unsigned char PositionMode;
unsigned char AlignmentFlag;

unsigned short int motormode=0;


int TargetPosition;
int TargetVelocity;
int TargetTorque;
int ActualPosition;
int ActualVelocity;
int ActualTorque;
int ActualHall;

unsigned int PID_Current_P;
unsigned int PID_Current_I;
unsigned int PID_Velocity_P;
unsigned int PID_Velocity_I;
unsigned int PID_Position_P;
unsigned int PID_Position_I;

unsigned int MotorMode=0;
unsigned int ActualState;

int PIDPositionKp,PIDPositionKi;

extern FOCVars_t FOCVars[NBR_OF_MOTORS];
extern MCI_Handle_t* pMCI[NBR_OF_MOTORS];
extern SpeednTorqCtrl_Handle_t *pSTC[NBR_OF_MOTORS];
extern PID_Handle_t *pPIDSpeed[NBR_OF_MOTORS];
extern PID_Handle_t *pPIDIq[NBR_OF_MOTORS];

int PositionOffset;

void delay_us(unsigned int n)
{
	unsigned char i;
	i=250;
	while(n--)
	{
		while(i--);
	}
}


unsigned char encoderupdate;
int i3;
extern unsigned int p1;
unsigned char SetZeroFlag,TxLength,Rxlength,Received;
unsigned char SetZeroValue;
unsigned char Aligned;
void control_value_task(void)
{

	Ax58200_MsgHandle();
	Ax58200_MsgSend();   
}


extern void FOC_CalcCurrRef(uint8_t bMotor);
extern MCI_Handle_t * oMCInterface[NBR_OF_MOTORS];

uint32_t JscopeUsCount;
uint32_t FOC_RunFrep = 1000000 / PWM_FREQUENCY;
	
void MotorData_Updata(void)
{
	

	// Updata Current
	if(STC_GetControlMode(oMCInterface[M1]->pSTC) == STC_TORQUE_MODE && PositionMode == 0)
	{
		MC_ProgramTorqueRampMotor1(TargetTorque * CURRENT_GAIN,0);
		MCI_ExecBufferedCommands( oMCInterface[M1] );
		FOC_CalcCurrRef( M1 );		
	}
	
	JscopeUsCount += FOC_RunFrep;

	#if (MOTOR_CURRENT_BANDWIDTH_SINWAVE_MODE == MOTOR_TEST_ENABLE) || \
			(MOTOR_SPEED_BANDWIDTH_SINWAVE_MODE == MOTOR_TEST_ENABLE) || \
			(MOTOR_POSITION_BANDWIDTH_SINWAVE_MODE == MOTOR_TEST_ENABLE)
	if(SineWaveGenerator.enable_flag == 1)
	{
			#if MOTOR_TEST_SINWAVE_COUNT_TICK == MOTOR_TEST_ENABLE
			SinWaveRunTickStart = DwtStartCount();
			#endif
			
			#if MOTOR_CURRENT_BANDWIDTH_SINWAVE_MODE == MOTOR_TEST_ENABLE
			TargetTorque = sine_wave_get_sample(&SineWaveGenerator, JscopeUsCount);
			#endif
			
			#if MOTOR_SPEED_BANDWIDTH_SINWAVE_MODE == MOTOR_TEST_ENABLE
			TargetVelocity = sine_wave_get_sample(&SineWaveGenerator, JscopeUsCount);
			#endif
			
			#if MOTOR_POSITION_BANDWIDTH_SINWAVE_MODE == MOTOR_TEST_ENABLE
			TargetPosition = sine_wave_get_sample(&SineWaveGenerator, JscopeUsCount);
			#endif
		
			#if MOTOR_TEST_SINWAVE_COUNT_TICK == MOTOR_TEST_ENABLE
			SinWaveRunTick = DwtEndCount(SinWaveRunTickStart);
			#endif
	}
	#endif
	
}


uint32_t SetZeroDwtTick;
uint8_t SetZeroStart;

uint8_t AlignmentCalibrationStart;
uint32_t AlignmentCalibrationTick;
int32_t AlignmentCalibrationArr[16];
uint32_t AlignmentCalibrationAddValue = 65536/12;
uint8_t AlignmentCalibrationCount = 0;
int32_t AlignmentAverageErr;

void EncAlignmentCalibration(void)
{
	static uint8_t AlignmentMaxCount = 12;
	static uint8_t AlignmentCalibrationFlag = 0;

	if(AlignmentCalibrationStart == 1)
	{
		if(AlignmentCalibrationCount < AlignmentMaxCount)
		{
			if(AlignmentCalibrationCount == 0)
			{
				AlignmentAverageErr = 0;
			}
			
			if(AlignmentCalibrationFlag == 0)
			{
				AlignmentCalibrationTick = DwtStartCount();
				AlignmentCalibrationFlag++;
			}
			else if(AlignmentCalibrationFlag == 1)
			{
				if(DwtEndCount(AlignmentCalibrationTick) > 170000000)
				{	
					AlignmentCalibrationArr[AlignmentCalibrationCount] = User_GetElAngle();
					AlignmentAverageErr += AlignmentCalibrationCount * AlignmentCalibrationAddValue - AlignmentCalibrationArr[AlignmentCalibrationCount];
					AlignmentCalibrationCount++;
					AlignmentCalibrationFlag = 0;
				}
			}
		}
		else
		{
			AlignmentCalibrationStart = 0;
			AlignmentCalibrationCount = 0;
			AlignmentCalibrationFlag = 0;
			AlignmentAverageErr = AlignmentAverageErr / AlignmentMaxCount;
		}
	}
	
}
	
int positionindex;
int actual_current,k;
extern short Eangle1,E;
extern unsigned int position_div8;
unsigned char Iq_data_save;
int RealSpeed;
void control_value_update(void)
{
	static unsigned short int motorcontrol=0;
	static unsigned char motorrun;
	static unsigned char IQ_save_flag;

	if(motormode!=(MotorMode&0xFF))
	{
		motormode=MotorMode&0xFF;
		switch(motormode)
		{
			case 0:
			{
				PositionMode=1;
				PositionIqStart=0;
				pSTC[M1]->Mode=STC_TORQUE_MODE;
				FOCVars[M1].bDriveInput = INTERNAL;
				MC_ProgramTorqueRampMotor1(0,0);
				PIDPositionHandle_M1.wIntegralTerm=0;
				break;
			}
			case 1:
			{
				PositionMode=0;
				PositionIqStart=0;
				pSTC[M1]->Mode=STC_TORQUE_MODE;
				FOCVars[M1].bDriveInput = INTERNAL;
				MC_ProgramTorqueRampMotor1(0,0);
				break;
			}
			case 2:
			{
				PositionMode=0;
				PositionIqStart=0;
				pSTC[M1]->Mode=STC_SPEED_MODE;
				FOCVars[M1].bDriveInput = INTERNAL;
				MC_ProgramSpeedRampMotor1(0,0);
				break;
			}
			
			case 3:
			{
				PositionOffset=65536*ENCCount+position_div8;
				break;
			}
			case 4:
			{              
				SetZeroDwtTick = DwtStartCount();
				SetZeroStart = 1;
				
				PositionMode=1;
				pSTC[M1]->Mode=STC_TORQUE_MODE;
				FOCVars[M1].bDriveInput = INTERNAL;
				MC_ProgramTorqueRampMotor1(0,0);
				PIDPositionHandle_M1.wIntegralTerm=0;
			
				EncAlignCtrlM1.EncAligned=1;
				TargetRampPosition=0;
				MC_StartMotor1();

				
				break;
			}
			case 5:
			{
				PositionMode=1;
				pSTC[M1]->Mode=STC_SPEED_MODE;
				FOCVars[M1].bDriveInput = INTERNAL;
				MC_ProgramSpeedRampMotor1(0,0);
				break;
			}
			case 6:
			{
				AlignmentCalibrationStart = 1;
				break;
			}
			case 9:
			{
				PositionMode=1;
				pSTC[M1]->Mode=STC_SPEED_MODE;
				FOCVars[M1].bDriveInput = INTERNAL;
				MC_ProgramSpeedRampMotor1(0,0);
				PIDPositionHandle_M1.wIntegralTerm=0;
				pPIDSpeed[M1]->wIntegralTerm=0;
				PositionIqStart=1;
			}
			default:break;
		}
	}
	if(motorcontrol!=MotorMode>>16)
	{
		motorcontrol=MotorMode>>16;
		if(motorcontrol==1)
		{
			EncAlignCtrlM1.EncAligned=1;
			TargetRampPosition=0;
			MC_StartMotor1();
			motorrun=1;
			if(PositionIqStart)
				PositionIqEn=1;
		}
		else if(motormode!=4)
		{
			MC_ProgramTorqueRampMotor1(0,0);
			MC_StopMotor1();
			motorrun=0;
			TargetSpeed=0;
			TargetSpeed_pre=0;
			if(PositionIqEn==1)
			{
				PositionIqEn=0;
				Iq_data_save=1;
			}
			if(motormode==14)
			{
				if(IQ_save_flag==0)
				{
					IQ_save_flag=1;
					Iq_data_save=1;
				}
			}
			else
			{
				IQ_save_flag=0;
			}
		}
	}
	
	if(motorrun)
	{
		switch(motormode)
		{
			case 0:
			{
				PositionMode=1;
				break;
			}
			case 1:
			{
				#if COGGING_FORCE_COMPENSATION_ENABLE == 1
				MC_ProgramTorqueRampMotor1(TargetTorque * CURRENT_GAIN+((IQ[position_div8>>4]<<2)-40)*CURRENT_GAIN,0);
				#else
				MC_ProgramTorqueRampMotor1(TargetTorque * CURRENT_GAIN,0);
				#endif				
				PositionMode=0;
				break;
			}
			case 2:
			{
				MC_ProgramSpeedRampMotor1( TargetVelocity/6, 100 );
				PositionMode=0;
				break;
			}
			default:break;
		}
	}
	else
	{
		MCI_FaultAcknowledged(pMCI[M1]);
	}
	if(PositionIqStart==0)
		ActualTorque=MCI_GetIqd( pMCI[M1] ).q / CURRENT_GAIN;
	else
		ActualTorque=Iq;
	

	ActualVelocity=((ENCODER_M1._Super.hAvrMecSpeedUnit*6)<<16) | (GeneralEnc_GetData(Enc_Array[DEFAULT_ENC], Enc_Cmd_User3) >> 8);
	RealSpeed = (ENCODER_M1._Super.hAvrMecSpeedUnit*6);
//	ActualPosition=4096*ENCCount+TIM2->CNT-PositionOffset;
	EncAlignmentCalibration();
}

int16_t PID( PID_Handle_t * pHandle, int32_t wProcessVarError )
{
	int32_t wProportional_Term,wOutput_32;
	
  int16_t hUpperOutputLimit = pHandle->hUpperOutputLimit;
  int16_t hLowerOutputLimit = pHandle->hLowerOutputLimit;

  /* Proportional term computation*/
  wProportional_Term = pHandle->hKpGain * wProcessVarError;

  /* Integral term computation */

  wOutput_32 = ( wProportional_Term / pHandle->hKpDivisor) ;

  if ( wOutput_32 > hUpperOutputLimit )
  {
    wOutput_32 = hUpperOutputLimit;
  }
  else if ( wOutput_32 < hLowerOutputLimit )
  {
    wOutput_32 = hLowerOutputLimit;
  }

  return ( ( int16_t )( wOutput_32 ) );
}

int TargetPosition1;

extern SPI_HandleTypeDef hspi3;
//void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
//{
////	HAL_SPI_DMAStop(hspi);
//	control_value_task();
//	HAL_SPI_TransmitReceive_DMA(&hspi3, TxBuf, RxBuf, 20);
//}

//void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
//{
//	control_value_task();
//	HAL_SPI_TransmitReceive_DMA(hspi, TxBuf, RxBuf, 20);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,1);
//}

unsigned char loop=0;
unsigned int count=0;

void Position_Iq_task(void)
{
	int temp1;

	switch(loop)
	{
		case 0:
		{
			if(PositionIqStart)
			{
				loop=1;
				count=0;
				TargetPosition1=0;
			}
			break;
		}
		case 1:
		{
			if(PositionIqEn)
			{
				TargetPosition1=0;
				count++;
				if(count>=8000)
				{
					count=0;
					loop=2;
				}
			}
			break;
		}
		case 2:
		{
			count++;
			if(count>2500)
			{
				loop=3;
				temp1=Iq;
				if(temp1>127)
				{
					temp1=127;
				}
				else if(temp1<-127)
				{
					temp1=-127;
				}
				if(TargetPosition1<=4095)
					IQ[TargetPosition1]=temp1;
				else
					IQ[4095]=temp1;
			}
			if(PositionMode==0)
			{
				loop=0;
			}
			break;
		}
		case 3:
		{
			if(TargetPosition1<4095)
			{
				TargetPosition1++;
				count=0;
				loop=2;
			}
			else
			{
				loop=4;
			}
			break;
		}
		case 4:
		{
			if(!PositionIqStart)
			{
				loop=0;
			}
		}
		default :break;
	}
}

void SetZeroAngle(void)
{
	static unsigned char loop=0;
	switch(loop)
	{
		case 0:
		{
			if(SetZeroFlag==1)
			{
				usart_cmd=0x31;
				TxLength=1;
				Rxlength=4;
				Received=0;
				SetZeroValue=0;
				loop=1;
				Tx_485;
				HAL_UART_Transmit_DMA(&huart1,&usart_cmd,TxLength);
			}
			break;
		}
		case 1:
		{
			if(Received==1)
			{
				usart_cmd=0x29;
				TxLength=1;
				Rxlength=2;
				Received=0;
				loop=2;
				Tx_485;
				HAL_UART_Transmit_DMA(&huart1,&usart_cmd,TxLength);
			}
			break;
		}
		case 2:
		{
			if(Received==1)
			{
				usart_cmd=0x31;
				TxLength=1;
				Rxlength=4;
				Received=0;
				if(SetZeroValue==10)
					loop=3;
				else
					loop=1;
				Tx_485;
				HAL_UART_Transmit_DMA(&huart1,&usart_cmd,TxLength);
			}
			break;
		}
		case 3:
		{
			SetZeroFlag=0;
			Aligned=0;
			loop=0;
			break;
		}
		default:break;
	}
}

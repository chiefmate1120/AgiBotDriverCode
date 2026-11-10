#include "ax58200_protocol.h"
#include "motor_control.h"
#include "motorcontrol.h"
#include "MotorInterface.h"
#include "TimerTask.h"


extern FOCVars_t FOCVars[NBR_OF_MOTORS];
extern MCI_Handle_t* pMCI[NBR_OF_MOTORS];
extern SpeednTorqCtrl_Handle_t *pSTC[NBR_OF_MOTORS];
extern PID_Handle_t *pPIDSpeed[NBR_OF_MOTORS];
extern PID_Handle_t *pPIDIq[NBR_OF_MOTORS];
extern PID_Handle_t PIDPositionHandle_M1;

extern int TargetPosition;
extern int TargetVelocity;
extern int TargetTorque;
extern int ActualPosition;
extern int ActualVelocity;
extern int ActualTorque;
extern int ActualHall;
extern uint16_t ActualHallA;
extern uint16_t ActualHallB;

extern unsigned int PID_Current_P;
extern unsigned int PID_Current_I;
extern unsigned int PID_Velocity_P;
extern unsigned int PID_Velocity_I;
extern unsigned int PID_Position_P;
extern unsigned int PID_Position_I;

extern unsigned int MotorMode;
extern unsigned int ActualState;
extern unsigned int ActualKeyState;

extern unsigned char TxBuf[50];
extern unsigned char RxBuf[50];

extern uint8_t StMotorState;
extern uint8_t NaSingle;


/**
 * @brief Limits a float value to the given maximum and minimum bounds.
 *
 * If max_num < min_num, the function swaps their values to ensure correct limit.
 * Returns max_num if num is greater than max_num,
 *         min_num if num is less than min_num,
 *         otherwise returns num unchanged.
 *
 * @param num      The input floating-point number to be limited.
 * @param max_num  The upper bound for the input number.
 * @param min_num  The lower bound for the input number.
 * @return         The limited value within [min_num, max_num].
 */
float float_num_limit(float num, float max_num, float min_num)
{
	float temp_num;
	// Swap if max_num is less than min_num to ensure proper bounds
	if(max_num < min_num)
	{
		temp_num = max_num;
		max_num = min_num;
		min_num = temp_num;
	}
	
	// Clamp num to the range [min_num, max_num]
	if(num > max_num)
	{
		return max_num;
	}
	
	if(num < min_num)
	{
		return min_num;
	}
	
	// If within range, return num itself
	return num;
}

/**
 * @brief Extracts the high 16 bits from a 4-byte array as a 16-bit unsigned integer.
 *
 * This function takes a pointer to an array of uint8_t and returns the value constructed
 * from Data[0] (high byte) and Data[1] (low byte) as a 16-bit big-endian number.
 *
 * @param Data Pointer to a uint8_t array (at least 2 bytes).
 * @return The combined 16-bit value from Data[0] and Data[1].
 */
uint32_t U32GetHighU16(uint8_t *Data)
{
	return Data[0]<<8 | Data[1]<<0 ;
}

uint32_t U32GetLowU16(uint8_t *Data)
{
	return Data[2]<<8 | Data[3]<<0 ;
}

uint32_t VerifyCalculation(uint8_t Data[], uint16_t Size)
{
	uint32_t VerifyData = 0;
	
	for(int i=0; i<Size; i++)
	{
		VerifyData += Data[i];
	}
	return VerifyData;
}

uint8_t VerifyGetData(uint8_t Data[], uint16_t Size, uint32_t *VerifyData)
{
	if(Size < sizeof(Ax52800Protocol_t))
	{
		return 0;
	}
	
	*VerifyData = LittletoBig32(((Ax52800Protocol_t *)Data)->BaseProtocol[5].Data);
	return 1;
}

uint8_t VerifyCheck(uint8_t Data[], uint16_t Size)
{
	uint32_t VerifyData;
	uint32_t CalculationVerifyData;
	if(!VerifyGetData(Data, Size, &VerifyData))
	{
		return 0;
	}
	
	CalculationVerifyData = VerifyCalculation(Data, Size-4);
	if(VerifyData == CalculationVerifyData)
	{
		return 1;
	}
	
	return 0;

}
void Ax58200_MsgHandle(void)
{
	Ax52800Protocol_t RxProtocol = {0};
    

    
	if(VerifyCheck(RxBuf, sizeof(RxProtocol)))
	{
		memcpy(&RxProtocol, RxBuf, sizeof(RxProtocol));
		if(RxProtocol.BaseProtocol[0].CmdType == TARGET_POSITION)
		{
			TargetPosition = LittletoBig32(RxProtocol.BaseProtocol[0].Data);
			TargetTorque = LittletoBig32(RxProtocol.BaseProtocol[1].Data);
			TargetVelocity = LittletoBig32(RxProtocol.BaseProtocol[2].Data);
			MotorMode = LittletoBig32(RxProtocol.BaseProtocol[3].Data);
		}
		else 
		{
			PID_Current_P = U32GetHighU16((uint8_t *)&RxProtocol.BaseProtocol[0].Data);
			PID_Current_I = U32GetLowU16((uint8_t *)&RxProtocol.BaseProtocol[0].Data);
			
			PID_Velocity_P = U32GetHighU16((uint8_t *)&RxProtocol.BaseProtocol[2].Data);
			PID_Velocity_I = U32GetLowU16((uint8_t *)&RxProtocol.BaseProtocol[2].Data);
			
			PID_Position_P = U32GetHighU16((uint8_t *)&RxProtocol.BaseProtocol[3].Data);
			PID_Position_I = U32GetLowU16((uint8_t *)&RxProtocol.BaseProtocol[3].Data);
		}
		
		if(RxProtocol.BaseProtocol[0].CmdType == TARGET_POSITION)
		{
			//check Torque
			float TmpTargetTorque;
			TargetTorque=RxBuf[6]<<24|RxBuf[7]<<16|RxBuf[8]<<8|RxBuf[9];
			TmpTargetTorque = float_num_limit(TargetTorque, 32767 / CURRENT_GAIN, -32767 / CURRENT_GAIN);		
			TmpTargetTorque = float_num_limit(TmpTargetTorque * CURRENT_GAIN, pSTC[M1]->MaxPositiveTorque, pSTC[M1]->MinNegativeTorque);			
			TargetTorque = TmpTargetTorque / CURRENT_GAIN;
		}
		else 
		{
			//Set Cur PI
			PID_SetKP(pPIDIq[M1],(int16_t)PID_Current_P);
			PID_SetKI(pPIDIq[M1],(int16_t)PID_Current_I);
			
			//Set Velocity PI
			PID_SetKP(pPIDSpeed[M1],(int16_t)PID_Velocity_P);
			PID_SetKI(pPIDSpeed[M1],(int16_t)PID_Velocity_I);
			
			//Set Position PI
			PIDPositionHandle_M1.hKpGain=PID_Position_P;
//			if(PID_Position_I > 32767 / CURRENT_GAIN)
//			{
//				STC_SetNominalCurrent( pSTC[M1], 32767);
//			}
//			else
//			{
//				STC_SetNominalCurrent( pSTC[M1], PID_Position_I * CURRENT_GAIN);
//			}
//			
//			if(PID_Position_I<100)
//			{
//				PID_Position_I=100;
//			}
//			pPIDSpeed[M1]->hUpperOutputLimit=(int32_t)PID_Position_I*CURRENT_GAIN;
//			pPIDSpeed[M1]->hLowerOutputLimit=-(int32_t)PID_Position_I*CURRENT_GAIN;
//			pPIDSpeed[M1]->wUpperIntegralLimit=(int32_t)PID_Position_I*CURRENT_GAIN*(int32_t)SP_KIDIV;
//			pPIDSpeed[M1]->wLowerIntegralLimit=-(int32_t)PID_Position_I*CURRENT_GAIN*(int32_t)SP_KIDIV;
		}
	}
	

}

extern int EncState;
extern uint32_t EncCommunicationCount;

void Ax58200_MsgSend(void)
{
	static uint8_t HeadSingle=0;
	static uint8_t EncHeadSigle = 0;
	Ax52800Protocol_t TxProtocol = {0};

	//updata data	
	if(EncCommunicationCount <= 100)
	{
		EncHeadSigle = 0;
	}
	else
	{
		EncHeadSigle = 1;
	}
	
	HeadSingle = !HeadSingle;
	NaSingle = ActualKeyState;
	ActualState = (STM32_MOTOR_VERSION & 0xFFFF) | ((EncHeadSigle & 0x01) << 20) | ((EncState & 0x7) << 21)  | ((NaSingle & 0xF) << 24) | ((HeadSingle & 0x1) << 28);
	
	ActualHallA =  HallAdcGet(0);
	ActualHallB =  HallAdcGet(1);
	ActualHall = ActualHallA<<16 | ActualHallB;
    
	TxProtocol.BaseProtocol[0].CmdType = ACTUAL_POSITION;
	TxProtocol.BaseProtocol[1].CmdType = ACTUAL_TORQUE;
	TxProtocol.BaseProtocol[2].CmdType = ACTUAL_VELOCITY;
	TxProtocol.BaseProtocol[3].CmdType = ACTUAL_STATE;
	TxProtocol.BaseProtocol[4].CmdType = ACTUAL_HALL;    
	TxProtocol.BaseProtocol[5].CmdType = ACTUAL_VERIFY;
	
	TxProtocol.BaseProtocol[0].Data = LittletoBig32(ActualPosition);
	TxProtocol.BaseProtocol[1].Data = LittletoBig32(ActualTorque);
	TxProtocol.BaseProtocol[2].Data = LittletoBig32(ActualVelocity);
	TxProtocol.BaseProtocol[3].Data = LittletoBig32(ActualState);
	TxProtocol.BaseProtocol[4].Data = LittletoBig32(ActualHall);
	TxProtocol.BaseProtocol[5].Data = LittletoBig32(VerifyCalculation((uint8_t *)&TxProtocol, sizeof(Ax52800Protocol_t)));
	
	memcpy(&TxBuf, (uint8_t *)&TxProtocol, sizeof(TxProtocol));
}

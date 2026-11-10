#include "main.h"
#include "motor_control.h"
#include "General_Tool.h"
#include "Dwt_driver.h"
#include "mc_api.h"
#include "General_Enc.h"
#include "MPT_Enc.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;


#define MPT_ENC_CMD_SET_ZERO_POSITION (0x30)
#define MPT_ENC_CMD_GET_POSITION (0x31)
#define MPT_ENC_CMD_GET_POSITION_AND_STATES (0x64)
#define MPT_ENC_CMD_GET_POSITION_AND_TEMPERATURE (0x74)

static uint8_t Mpt_Read(GeneralEnc_t *pEncoder, uint8_t Cmd,uint8_t *RxBuff, uint8_t *EncRxSize);
static uint8_t Mpt_DataProcess(GeneralEnc_t *pEncoder);
static uint32_t Mpt_GetAngle(GeneralEnc_t *pEncoder);
static uint32_t Mpt_GetTemperature(GeneralEnc_t *pEncoder);
static uint32_t Mpt_GetStates(GeneralEnc_t *pEncoder);
static uint8_t Mpt_WriteRead(GeneralEnc_t *pEncoder, uint8_t Cmd, uint8_t *RxBuff, uint8_t *EncRxSize);
static void EncErrCallback(GeneralEnc_t *pEncoder, Enc_ErrCode_e ErrCode);

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

MPT_Encoder_t Mpt_Encoder = {
	.Parent = {
		.EncName = "Mpt_Encoder",
		.CommunicationMode = Enc_SynchronizationMode,
		.Read = Mpt_Read,
		.WriteRead = Mpt_WriteRead,
		.DataProcess = Mpt_DataProcess,
		.GetAngle = Mpt_GetAngle,
		.GetTemperature = Mpt_GetTemperature,
		.GetStates = Mpt_GetStates,
		.ErrState = Enc_ErrNone,
		.EncErrCallback = EncErrCallback,
	},
	.InteriorAngle = 0,
	.States = 0,
	.Temperature = 0,
};

static uint32_t Mpt_GetAngle(GeneralEnc_t *pEncoder)
{
	MPT_Encoder_t *pMptEncoder = (MPT_Encoder_t *)pEncoder;
	return pMptEncoder->InteriorAngle;
}

static uint32_t Mpt_GetTemperature(GeneralEnc_t *pEncoder)
{
	MPT_Encoder_t *pMptEncoder = (MPT_Encoder_t *)pEncoder;
	return pMptEncoder->Temperature;
}

static uint32_t Mpt_GetStates(GeneralEnc_t *pEncoder)
{
	MPT_Encoder_t *pMptEncoder = (MPT_Encoder_t *)pEncoder;
	return pMptEncoder->States;
}
static uint8_t Mpt_CmdGet(uint8_t Cmd)
{
	uint8_t SendCmd;
	
	switch(Cmd)
	{
		case Enc_Cmd_SetZeroPosition:
		{	
			SendCmd = MPT_ENC_CMD_SET_ZERO_POSITION;
			break;
		}
		case Enc_Cmd_GetInteriorAngle:
		{
			SendCmd = MPT_ENC_CMD_GET_POSITION_AND_STATES;
			break;
		}
		case Enc_Cmd_GetStates:
		{
			SendCmd = MPT_ENC_CMD_GET_POSITION_AND_STATES;
			break;
		}
		case Enc_Cmd_GetTemperature:
		{
			SendCmd = MPT_ENC_CMD_GET_POSITION_AND_TEMPERATURE;
			break;
		}
		default:
		{
			SendCmd = 0;
			break;
		}
	}
	
	return SendCmd;
}

static uint8_t Mpt_GetReadSize(uint8_t Cmd)
{
	switch(Cmd)
	{
		case Enc_Cmd_SetZeroPosition:
		{	
			return 2;
		}
		case Enc_Cmd_GetInteriorAngle:
		{
			return 5;
		}
		case Enc_Cmd_GetStates:
		{
			return 5;
		}
		case Enc_Cmd_GetTemperature:
		{
			return 6;
		}
		default:
		{
			return 0;
		}
	}
}

static void Enc_fastWrite(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t us)
{
	uint32_t DwtStartTick;

	DwtStartTick = DwtStartCount();

	huart->Instance->TDR = (uint8_t)(*pData & 0xFFU);

	while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET)
	{
		if(DwtTimeOutCheck(DwtStartTick, us))
		{
			break;
		}
	}
	
}



static uint8_t Mpt_WriteRead(GeneralEnc_t *pEncoder, uint8_t Cmd, uint8_t *RxBuff, uint8_t *EncRxSize)
{
	uint8_t DataSize;
	static uint8_t SendCmd;
	SendCmd = Mpt_CmdGet(Cmd);

	DataSize = Mpt_GetReadSize(Cmd);
	*EncRxSize = DataSize;

	Tx_485;
	Enc_fastWrite(&huart1, &SendCmd, 10);
	
	HAL_UART_AbortReceive(&huart1);
	Rx_485;
	HAL_UART_Receive_DMA(&huart1, RxBuff, DataSize);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT|DMA_IT_TE);
	
	return Enc_ErrNone;
}

static uint8_t Mpt_Read(GeneralEnc_t *pEncoder, uint8_t Cmd,uint8_t *RxBuff, uint8_t *EncRxSize)
{
	uint8_t DataSize;
	DataSize = Mpt_GetReadSize(Cmd);
	*EncRxSize = DataSize;
	if(DataSize == 0)
	{
		return 0;
	}
	
	Rx_485;
	HAL_UART_Receive_DMA(&huart1, RxBuff, DataSize);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT|DMA_IT_TE);
	
	return Enc_ErrNone;
}

static uint8_t Mpt_DataProcess(GeneralEnc_t *pEncoder)
{
	uint8_t *RxBuff = pEncoder->EncRxBuff;
	uint8_t RxSize = pEncoder->EncRxSize;
	uint8_t Crc = RxBuff[RxSize - 1];
	uint8_t Cmd = pEncoder->SendCmd;
	MPT_Encoder_t *pMptEncoder = (MPT_Encoder_t *)pEncoder;

	if(Crc != calcCRC(RxBuff, RxSize - 1))
	{
		return Enc_DataProcess_Error;
	}
	

	switch(Cmd)
	{
		case Enc_Cmd_SetZeroPosition:
		{	
			pMptEncoder->ClearCount = RxBuff[0];
			break;
		}
		case Enc_Cmd_GetInteriorAngle:
		{
			pMptEncoder->InteriorAngle = RxBuff[0] << 16 | RxBuff[1] << 8 | RxBuff[2];
			pMptEncoder->States = RxBuff[3];
			break;
		}
		case Enc_Cmd_GetStates:
		{
			pMptEncoder->InteriorAngle = RxBuff[0] << 16 | RxBuff[1] << 8 | RxBuff[2];
			pMptEncoder->States = RxBuff[3];
			break;
		}
		case Enc_Cmd_GetTemperature:
		{
			pMptEncoder->InteriorAngle = RxBuff[0] << 16 | RxBuff[1] << 8 | RxBuff[2];
			pMptEncoder->Temperature = RxBuff[3] << 8 | RxBuff[4];
			break;
		}
		default:
		{
			return Enc_DataProcess_Error;
		}
	}
	
	GeneralEnc_ResetErrorCount(pEncoder);
	
	return Enc_ErrNone;

}

static void EncErrCallback(GeneralEnc_t *pEncoder, Enc_ErrCode_e ErrCode)
{
	if(ErrCode == Enc_DataProcess_Error)
	{
		if(pEncoder->DataProcessErrorCount > 100)
		{
			MC_ProgramTorqueRampMotor1(0,0);
			MC_StopMotor1();
		}
	}
}


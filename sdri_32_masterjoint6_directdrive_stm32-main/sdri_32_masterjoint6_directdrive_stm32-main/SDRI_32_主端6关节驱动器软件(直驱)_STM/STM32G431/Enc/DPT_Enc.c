#include "DPT_Enc.h"
#include "main.h"
#include "motor_control.h"
#include "General_Tool.h"
#include "Dwt_driver.h"
#include "mc_api.h"

#define DPT_ENC_CMD_SET_ZERO_INTERIOR_POSITION (0x29)
#define DPT_ENC_CMD_SET_ZERO_EXTERNAL_POSITION (0x30)
#define DPT_ENC_CMD_GET_INTERIOR_POSITION (0x31)
#define DPT_ENC_CMD_GET_EXTERNAL_POSITION (0x32)
#define DPT_ENC_CMD_GET_INTERIOR_AND_EXTERNAL_POSITION (0x33)

#define DPT_ENC_CMD_GET_INTERIOR_POSITION_AND_STATES (0x41)
#define DPT_ENC_CMD_GET_EXTERNAL_POSITION_AND_STATES (0x42)
#define DPT_ENC_CMD_GET_INTERIOR_POSITION_AND_EXTERNAL_POSITION_AND_STATES (0x43)
#define DPT_ENC_CMD_GET_TEMPERATURE (0x74)


static uint8_t Dpt_DataProcess(GeneralEnc_t *pEncoder);
static uint32_t Dpt_GetAngle(GeneralEnc_t *pEncoder);
static uint32_t Dpt_GetTemperature(GeneralEnc_t *pEncoder);
static uint32_t Dpt_GetStates(GeneralEnc_t *pEncoder);
static uint8_t Dpt_WriteRead(GeneralEnc_t *pEncoder, uint8_t Cmd, uint8_t *RxBuff, uint8_t *EncRxSize);
static void Dpt_SetData(GeneralEnc_t *pEncoder, uint8_t cmd, uint32_t data);
static uint32_t Dpt_GetData(GeneralEnc_t *pEncoder, uint8_t cmd);
static void EncErrCallback(GeneralEnc_t *pEncoder, Enc_ErrCode_e ErrCode);

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

DPT_Encoder_t Dpt_Encoder = {
	.Parent = {
		.EncName = "DPT_Encoder",
		.CommunicationMode = Enc_SynchronizationMode,
		.WriteRead = Dpt_WriteRead,
		.DataProcess = Dpt_DataProcess,
		.GetAngle = Dpt_GetAngle,
		.GetTemperature = Dpt_GetTemperature,
		.SetData = Dpt_SetData,
		.GetData = Dpt_GetData,
		.GetStates = Dpt_GetStates,
		.ErrState = Enc_ErrNone,
		.EncErrCallback = EncErrCallback,
	},
	.InteriorAngle = 0,
	.ExternalAngle = 0,
	.States = 0,
	.Temperature = 0,
	.ClearFlag = 0,
	.ClearCount = 0,
};

static uint32_t Dpt_GetAngle(GeneralEnc_t *pEncoder)
{
	DPT_Encoder_t *pDptEncoder = (DPT_Encoder_t *)pEncoder;
	return pDptEncoder->InteriorAngle;
}

static uint32_t Dpt_GetTemperature(GeneralEnc_t *pEncoder)
{
	DPT_Encoder_t *pDptEncoder = (DPT_Encoder_t *)pEncoder;
	return pDptEncoder->Temperature;
}

static uint32_t Dpt_GetStates(GeneralEnc_t *pEncoder)
{
	DPT_Encoder_t *pDptEncoder = (DPT_Encoder_t *)pEncoder;
	return pDptEncoder->States;
}


static void Dpt_SetData(GeneralEnc_t *pEncoder, uint8_t cmd, uint32_t data)
{
	DPT_Encoder_t *pDptEncoder = (DPT_Encoder_t *)pEncoder;

	if(cmd == Enc_Cmd_User1)
	{
		
	}
	else if(cmd == Enc_Cmd_User2)
	{
		pDptEncoder->ClearCount = data;
	}
	else if(cmd == Enc_Cmd_User3)
	{
	
	}
}

static uint32_t Dpt_GetData(GeneralEnc_t *pEncoder, uint8_t cmd)
{
	DPT_Encoder_t *pDptEncoder = (DPT_Encoder_t *)pEncoder;

	if(cmd == Enc_Cmd_User1)
	{
		
	}
	else if(cmd == Enc_Cmd_User2)
	{
		return pDptEncoder->ClearCount;
	}
	else if(cmd == Enc_Cmd_User3)
	{
		return pDptEncoder->ExternalAngle;
	}
	else if(cmd == Enc_Cmd_User4)
	{
		return pDptEncoder->States;
	}
	
	return 0;
}


static uint8_t Dpt_CmdGet(uint8_t Cmd)
{
	uint8_t SendCmd;
	
	switch(Cmd)
	{
		case Enc_Cmd_SetZeroPosition:
		{	
			SendCmd = DPT_ENC_CMD_SET_ZERO_INTERIOR_POSITION;
			break;
		}
		case Enc_Cmd_GetInteriorAngle:
		{
			SendCmd = DPT_ENC_CMD_GET_INTERIOR_POSITION_AND_EXTERNAL_POSITION_AND_STATES;
			break;
		}
		case Enc_Cmd_GetStates:
		{
			SendCmd = DPT_ENC_CMD_GET_INTERIOR_POSITION_AND_STATES;
			break;
		}
		case Enc_Cmd_GetTemperature:
		{
			SendCmd = DPT_ENC_CMD_GET_TEMPERATURE;
			break;
		}
		case Enc_Cmd_User1:
		{
			SendCmd = DPT_ENC_CMD_GET_INTERIOR_POSITION;
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

static uint8_t DPT_GetReadSize(uint8_t Cmd)
{
	switch(Cmd)
	{
		case Enc_Cmd_SetZeroPosition:
		{	
			return 2;
		}
		case Enc_Cmd_GetInteriorAngle:
		{
			return 8;
		}
		case Enc_Cmd_GetStates:
		{
			return 5;
		}
		case Enc_Cmd_GetTemperature:
		{
			return 3;
		}
		case Enc_Cmd_User1:
		{
			return 4;
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




static uint8_t Dpt_WriteRead(GeneralEnc_t *pEncoder, uint8_t Cmd, uint8_t *RxBuff, uint8_t *EncRxSize)
{
	uint8_t DataSize;
	static uint8_t SendCmd;

	SendCmd = Dpt_CmdGet(Cmd);
	DataSize = DPT_GetReadSize(Cmd);
	*EncRxSize = DataSize;

	Tx_485;
	Enc_fastWrite(&huart1, &SendCmd, 10);
	
	HAL_UART_AbortReceive(&huart1);
	Rx_485;
	HAL_UART_Receive_DMA(&huart1, RxBuff, DataSize);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT|DMA_IT_TE);
	
	return Enc_ErrNone;
}


static uint8_t Dpt_DataProcess(GeneralEnc_t *pEncoder)
{
	uint8_t *RxBuff = pEncoder->EncRxBuff;
	uint8_t RxSize = pEncoder->EncRxSize;
	uint8_t Crc = RxBuff[RxSize - 1];
	uint8_t Cmd = pEncoder->SendCmd;
	DPT_Encoder_t *pDptEncoder = (DPT_Encoder_t *)pEncoder;

	if(Crc != calcCRC(RxBuff, RxSize - 1))
	{
		return Enc_DataProcess_Error;
	}
	

	switch(Cmd)
	{
		case Enc_Cmd_SetZeroPosition:
			pDptEncoder->ClearCount = RxBuff[0];
			break;
		case Enc_Cmd_GetInteriorAngle:
		{
			pDptEncoder->InteriorAngle = RxBuff[0] | RxBuff[1] << 8 | RxBuff[2] << 16;
			pDptEncoder->ExternalAngle = RxBuff[3] | RxBuff[4] << 8 | RxBuff[5] << 16;
			pDptEncoder->States = RxBuff[6];
			break;
		}
		case Enc_Cmd_GetStates:
		{
			pDptEncoder->InteriorAngle = RxBuff[0] | RxBuff[1] << 8 | RxBuff[2] << 16;
			pDptEncoder->States = RxBuff[3];
			break;
		}
		case Enc_Cmd_GetTemperature:
		{
			pDptEncoder->Temperature = RxBuff[0] | RxBuff[1] << 8;
			break;
		}
		case Enc_Cmd_User1:
		{
			pDptEncoder->InteriorAngle = RxBuff[0] | RxBuff[1] << 8 | RxBuff[2] << 16;
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

	}
}

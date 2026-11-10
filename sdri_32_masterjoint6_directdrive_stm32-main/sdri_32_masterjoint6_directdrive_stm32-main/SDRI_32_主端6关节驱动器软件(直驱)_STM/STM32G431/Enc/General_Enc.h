#ifndef __GENERAL_ENC_H
#define __GENERAL_ENC_H

#include <stdint.h>

typedef enum
{
	ENC_STATE_NO_ERR = 0,
	ENC_STATE_CMD_LENGTH_ERR,
	ENC_STATE_CMD_CRC_ERR,
}ENC_STATE_CODE;


typedef enum
{
	Enc_ErrNone = 0,
	Enc_Err,
	Enc_Init_Error,
	Enc_WriteRead_Error,
	Enc_Write_Error,
	Enc_Read_Error,
	Enc_DataProcess_Error,
}Enc_ErrCode_e;

typedef enum
{
	Enc_AsynchronizationMode = 0,
	Enc_SynchronizationMode,
}Enc_CommunicationMode_e;

typedef enum
{
	Enc_Cmd_SetZeroPosition,
	Enc_Cmd_GetInteriorAngle,
	Enc_Cmd_GetTemperature,
	Enc_Cmd_GetStates,

	Enc_Cmd_User1, 
	Enc_Cmd_User2,
	Enc_Cmd_User3,
	Enc_Cmd_User4,
	Enc_Cmd_User5,
	Enc_Cmd_User6,

}Enc_Cmd_e;

typedef struct _GeneralEnc_t GeneralEnc_t;

typedef struct _GeneralEnc_t
{
	char 	EncName[16];
	uint8_t SendCmd;
	uint8_t EncRxBuff[32];
	uint8_t EncRxSize;
	uint8_t InitFlag;

	Enc_ErrCode_e ErrState;
	uint32_t WriteErrorCount;
	uint32_t ReadErrorCount;
	uint32_t WriteReadErrorCount;
	uint32_t DataProcessErrorCount;

	//接口函数
	Enc_CommunicationMode_e CommunicationMode;
	uint8_t (*WriteRead)(GeneralEnc_t *pEncoder, uint8_t Cmd,uint8_t *RxBuff, uint8_t *EncRxSize);
	uint8_t (*Write)(GeneralEnc_t *pEncoder, uint8_t Cmd);
	uint8_t (*Read)(GeneralEnc_t *pEncoder, uint8_t Cmd,uint8_t *RxBuff, uint8_t *EncRxSize);
	uint8_t (*DataProcess)(GeneralEnc_t *pEncoder);
	
	uint32_t (*GetAngle)(GeneralEnc_t *pEncoder);
	uint32_t (*GetTemperature)(GeneralEnc_t *pEncoder);
	uint32_t (*GetStates)(GeneralEnc_t *pEncoder);
	uint32_t (*GetData)(GeneralEnc_t *pEncoder, uint8_t cmd);
	void (*SetData)(GeneralEnc_t *pEncoder, uint8_t cmd, uint32_t data);

	void (*EncErrCallback)(GeneralEnc_t *pEncoder, Enc_ErrCode_e eErr);
	
}GeneralEnc_t;


void GeneralEnc_Write(GeneralEnc_t *pEncoder, uint8_t Cmd);
void GeneralEnc_Read(GeneralEnc_t *pEncoder);
uint32_t GeneralEnc_DataProcess(GeneralEnc_t *pEncoder);
uint32_t GeneralEnc_GetAngle(GeneralEnc_t *pEncoder);
uint32_t GeneralEnc_GetTemperature(GeneralEnc_t *pEncoder);
uint32_t GeneralEnc_GetStates(GeneralEnc_t *pEncoder);
void GeneralEnc_ResetErrorCount(GeneralEnc_t *pEncoder);

uint32_t GeneralEnc_GetData(GeneralEnc_t *pEncoder, uint8_t cmd);
void GeneralEnc_SetData(GeneralEnc_t *pEncoder, uint8_t cmd, uint32_t data);

#endif

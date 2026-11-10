#include "General_Enc.h"
#include <stdlib.h>

uint8_t GeneralEnc_Check(GeneralEnc_t *pEncoder)
{
	if(pEncoder->CommunicationMode == Enc_AsynchronizationMode)
  {
    if(pEncoder->Write == NULL)
    {
      pEncoder->ErrState = Enc_Init_Error;
      return 0;
    }
    if(pEncoder->Read == NULL)
    {
      pEncoder->ErrState = Enc_Init_Error;
      return 0;
    }
  }
  else if(pEncoder->CommunicationMode == Enc_SynchronizationMode)
  {
    if(pEncoder->WriteRead == NULL)
    {
      pEncoder->ErrState = Enc_Init_Error;
      return 0;
    }
  }
  return 1;
}

void GeneralEnc_Write(GeneralEnc_t *pEncoder, uint8_t Cmd)
{
  if(pEncoder->InitFlag == 0)
  {
    if(GeneralEnc_Check(pEncoder))
    {
      pEncoder->InitFlag = 1;
    }
    else
    {
      return;
    }
  }

	pEncoder->SendCmd = Cmd;

	if(pEncoder->CommunicationMode == Enc_AsynchronizationMode)
	{
		if(pEncoder->Write(pEncoder, Cmd) != Enc_ErrNone)
		{
			pEncoder->ErrState = Enc_Write_Error;
			pEncoder->WriteErrorCount++;
	
			if(pEncoder->EncErrCallback != 0)
			{
				pEncoder->EncErrCallback(pEncoder, Enc_Write_Error);
			}
		}
	}
	else if(pEncoder->CommunicationMode == Enc_SynchronizationMode)
	{
		if(pEncoder->WriteRead(pEncoder, Cmd, pEncoder->EncRxBuff, &pEncoder->EncRxSize) != Enc_ErrNone)
		{
			pEncoder->ErrState = Enc_WriteRead_Error;
			pEncoder->WriteReadErrorCount++;
			
			if(pEncoder->EncErrCallback != 0)
			{
				pEncoder->EncErrCallback(pEncoder, Enc_WriteRead_Error);
			}
		}
	}


}


void GeneralEnc_Read(GeneralEnc_t *pEncoder)
{
	if(pEncoder->InitFlag == 0)
	{
		if(GeneralEnc_Check(pEncoder))
		{
			pEncoder->InitFlag = 1;
		}
		else
		{
			return;
		}
	}

	if(pEncoder->CommunicationMode == Enc_AsynchronizationMode)
	{
		if(pEncoder->Read(pEncoder, pEncoder->SendCmd, pEncoder->EncRxBuff, &pEncoder->EncRxSize) != Enc_ErrNone)
		{
			pEncoder->ErrState = Enc_Read_Error;
			pEncoder->ReadErrorCount++;
			if(pEncoder->EncErrCallback != 0)
			{
				pEncoder->EncErrCallback(pEncoder, Enc_Read_Error);
			}
		}
	}
	else if(pEncoder->CommunicationMode == Enc_SynchronizationMode)
	{
		if(pEncoder->WriteRead(pEncoder, pEncoder->SendCmd, pEncoder->EncRxBuff, &pEncoder->EncRxSize) != Enc_ErrNone)
		{
			pEncoder->ErrState = Enc_WriteRead_Error;
			pEncoder->WriteReadErrorCount++;
			if(pEncoder->EncErrCallback != 0)
			{
				pEncoder->EncErrCallback(pEncoder, Enc_WriteRead_Error);
			}
		}
	}
}

uint32_t GeneralEnc_DataProcess(GeneralEnc_t *pEncoder)
{
	if(pEncoder->InitFlag == 0)
	{
		if(GeneralEnc_Check(pEncoder))
		{
			pEncoder->InitFlag = 1;
		}
		else
		{
			return 0xFFFF;
		}
	}

	if(pEncoder->DataProcess(pEncoder) != Enc_ErrNone)
	{
		pEncoder->ErrState = Enc_DataProcess_Error;
		pEncoder->DataProcessErrorCount++;
		if(pEncoder->EncErrCallback != 0)
		{
			pEncoder->EncErrCallback(pEncoder, Enc_DataProcess_Error);
		}
		return pEncoder->DataProcessErrorCount;
	}
	
	return 0;


}

uint32_t GeneralEnc_GetAngle(GeneralEnc_t *pEncoder)
{
  return pEncoder->GetAngle(pEncoder);
}

uint32_t GeneralEnc_GetTemperature(GeneralEnc_t *pEncoder)
{
	return pEncoder->GetTemperature(pEncoder);
}

uint32_t GeneralEnc_GetStates(GeneralEnc_t *pEncoder)
{
	return pEncoder->GetStates(pEncoder);
}

uint32_t GeneralEnc_GetData(GeneralEnc_t *pEncoder, uint8_t cmd)
{
	return pEncoder->GetData(pEncoder, cmd);
}

void GeneralEnc_SetData(GeneralEnc_t *pEncoder, uint8_t cmd, uint32_t data)
{
	pEncoder->SetData(pEncoder, cmd, data);
}

void GeneralEnc_ResetErrorCount(GeneralEnc_t *pEncoder)
{
	pEncoder->WriteErrorCount = 0;
	pEncoder->ReadErrorCount = 0;
	pEncoder->WriteReadErrorCount = 0;
	pEncoder->DataProcessErrorCount = 0;
}

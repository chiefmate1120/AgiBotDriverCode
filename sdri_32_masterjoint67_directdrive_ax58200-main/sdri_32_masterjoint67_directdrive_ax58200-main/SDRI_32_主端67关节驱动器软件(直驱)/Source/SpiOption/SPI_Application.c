#include "SPI_Application.h"

void SPI_Init(void)
{
    SPIx_ConfigInit(SPI1);
    SPIx_ConfigInit(SPI3);
}

void SPI_CsControl(SPI_T* Instance, SPIx_CS_STATE State)
{
	if(Instance == SPI0)
	{
		if(State == SPI_LOW)
		{
			SPI0_CS_L;
		}
		else
		{
			SPI0_CS_H;
		}
	}
	
	if(Instance == SPI1)
	{
		if(State == SPI_LOW)
		{
			SPI1_CS_L;
		}
		else
		{
			SPI1_CS_H;
		}
	}
	
	if(Instance == SPI3)
	{
		if(State == SPI_LOW)
		{
			SPI3_CS_L;
		}
		else
		{
			SPI3_CS_H;
		}
	}
	
}


void STM_WriteRead(SPI_T* Instance, uint8_t cmd,int32_t value, UINT8 *pRxData)
{
		uint8_t TxData[5];  

    
		SPI_CsControl(Instance, SPI_LOW);
	
    TxData[0] = cmd;
    TxData[1] = 0xFF & (value>>24); 
    TxData[2] = 0xFF & (value>>16);
    TxData[3] = 0xFF & (value>>8);
    TxData[4] = 0xFF & (value>>0);  
    SPIx_TransmitReceive(Instance,TxData,pRxData,5,1000);

		SPI_CsControl(Instance, SPI_HIGH);
}






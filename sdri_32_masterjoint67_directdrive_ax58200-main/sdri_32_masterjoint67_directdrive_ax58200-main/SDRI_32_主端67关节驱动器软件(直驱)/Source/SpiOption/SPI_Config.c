#include "SPI_Config.h"

void SPIx_ConfigInit(SPI_T* Instance)
{
    if(Instance == SPI0)
    {
        HW_SPI_ClkSource(SPI0,ENABLE);
//CS
        HW_MultiFuncPins(SPI0_CS_PORT, SPI0_CS_PIN, 0);		
        GPIO_SetMode(SPI0_CS_PORT, SPI0_CS_PIN, GPIO_MODE_OUTPUT);
        GPIO_SetSlewCtl(SPI0_CS_PORT, SPI0_CS_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI0_CS_PORT, SPI0_CS_PIN, GPIO_PUSEL_PULL_UP);      
//CLK	
        GPIO_SetMode(SPI0_CLK_PORT, SPI0_CLK_PIN, GPIO_MODE_OUTPUT);
        GPIO_SetSlewCtl(SPI0_CLK_PORT, SPI0_CLK_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI0_CLK_PORT, SPI0_CLK_PIN, GPIO_PUSEL_PULL_UP);   
//MISO        	
        GPIO_SetMode(SPI0_MISO_PORT, SPI0_MISO_PIN, GPIO_MODE_INPUT);
        GPIO_SetSlewCtl(SPI0_MISO_PORT, SPI0_MISO_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI0_MISO_PORT, SPI0_MISO_PIN, GPIO_PUSEL_PULL_UP);   
//MOSI	
        GPIO_SetMode(SPI0_MOSI_PORT, SPI0_MOSI_PIN, GPIO_MODE_OUTPUT);
        GPIO_SetSlewCtl(SPI0_MOSI_PORT, SPI0_MOSI_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI0_MOSI_PORT, SPI0_MOSI_PIN, GPIO_PUSEL_PULL_UP);   
        
        SPI0_Select;
        SPI0_CS_H;
    } 
    else if(Instance == SPI1)
    {
        HW_SPI_ClkSource(SPI1,ENABLE);
//CS
        HW_MultiFuncPins(SPI1_CS_PORT, SPI1_CS_PIN, 0);		
        GPIO_SetMode(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_MODE_OUTPUT);
        GPIO_SetSlewCtl(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_PUSEL_PULL_UP);      
//CLK	
        GPIO_SetMode(SPI1_CLK_PORT, SPI1_CLK_PIN, GPIO_MODE_OUTPUT);
        GPIO_SetSlewCtl(SPI1_CLK_PORT, SPI1_CLK_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI1_CLK_PORT, SPI1_CLK_PIN, GPIO_PUSEL_PULL_UP);   
//MISO        	
        GPIO_SetMode(SPI1_MISO_PORT, SPI1_MISO_PIN, GPIO_MODE_INPUT);
        GPIO_SetSlewCtl(SPI1_MISO_PORT, SPI1_MISO_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI1_MISO_PORT, SPI1_MISO_PIN, GPIO_PUSEL_PULL_UP);   
//MOSI	
        GPIO_SetMode(SPI1_MOSI_PORT, SPI1_MOSI_PIN, GPIO_MODE_OUTPUT);
        GPIO_SetSlewCtl(SPI1_MOSI_PORT, SPI1_MOSI_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI1_MOSI_PORT, SPI1_MOSI_PIN, GPIO_PUSEL_PULL_UP);   
             
        SPI1_Select;
        SPI1_CS_H;
    }
    else if(Instance == SPI3)
    {
        HW_SPI_ClkSource(SPI3,ENABLE);
//CS
        HW_MultiFuncPins(SPI3_CS_PORT, SPI3_CS_PIN, 0);		
        GPIO_SetMode(SPI3_CS_PORT, SPI3_CS_PIN, GPIO_MODE_OUTPUT);
        GPIO_SetSlewCtl(SPI3_CS_PORT, SPI3_CS_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI3_CS_PORT, SPI3_CS_PIN, GPIO_PUSEL_PULL_UP);      
//CLK	
        GPIO_SetMode(SPI3_CLK_PORT, SPI3_CLK_PIN, GPIO_MODE_OUTPUT);
        GPIO_SetSlewCtl(SPI3_CLK_PORT, SPI3_CLK_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI3_CLK_PORT, SPI3_CLK_PIN, GPIO_PUSEL_PULL_UP);   
//MISO        	
        GPIO_SetMode(SPI3_MISO_PORT, SPI3_MISO_PIN, GPIO_MODE_INPUT);
        GPIO_SetSlewCtl(SPI3_MISO_PORT, SPI3_MISO_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI3_MISO_PORT, SPI3_MISO_PIN, GPIO_PUSEL_PULL_UP);   
//MOSI	
        GPIO_SetMode(SPI3_MOSI_PORT, SPI3_MOSI_PIN, GPIO_MODE_OUTPUT);
        GPIO_SetSlewCtl(SPI3_MOSI_PORT, SPI3_MOSI_PIN, GPIO_SLEWCTL_FAST);
        GPIO_SetPullCtl(SPI3_MOSI_PORT, SPI3_MOSI_PIN, GPIO_PUSEL_PULL_UP);   
        
        SPI3_Select;
        SPI3_CS_H;
    }    
    else
    {
        
    }
	/* Enable SPI PDI */
	SPI_Open(Instance, SPI_MASTER, SPI_MODE_3, 8, SPI_BAUDRATE);	
}

INT32 SPIx_WaitFlagStateUntilTimeout(SPI_T* Instance,UINT32 Flag, UINT32 State, UINT32 Timeout, UINT32 Tickstart)
{
	while((((Instance->STATUS & Flag) == (Flag)) ? 1 : 0) != State)
	{
		if(HW_CheckTimeout(Tickstart, Timeout))
		{
			SPI_DisableInt(Instance, SPI_FIFO_TXTH_INT_MASK|SPI_FIFO_RXTH_INT_MASK|SPI_SLVBE_INT_MASK);				
			SPI_DISABLE(Instance);
			DBG_PRINT("SPI wait flag timeout!\r\n");
			return -1;
		}
	}

	return 0;
}

INT32 SPIx_TransmitReceive(SPI_T* Instance,UINT8 *pTxData, UINT8 *pRxData, UINT16 Size, UINT32 Timeout)
{
	UINT32 tickstart = 0U, TxXferCount=0, RxXferCount=0;
	INT16 errorcode = 0;

	/* Init tickstart for timeout management*/
	tickstart = HW_GetTimer();
		  
	TxXferCount = Size;
	RxXferCount = Size;
	
	/* Check if the SPI is already enabled */
	

	/* CS = low */

	/* Transmit and Receive data in 8 Bit mode */
	if(TxXferCount == 0x01)
	{
		*((__IO uint8_t*)&(Instance->TX)) = (*pTxData);
		pTxData += sizeof(uint8_t);
		TxXferCount--;
	}
	
	while((TxXferCount > 0U) || (RxXferCount > 0U))
	{
		/* check TXE flag */
		if((TxXferCount > 0U) && (Instance->STATUS & SPI_STATUS_TXEMPTY_Msk))
		{
			*(__IO uint8_t *)&(Instance->TX) = (*pTxData++);
			TxXferCount--;
		}

		/* Wait until RXNE flag is reset */
		if((RxXferCount > 0U) && (Instance->STATUS & SPI_STATUS_RXEMPTY_Msk)==0)
		{
			(*(uint8_t *)pRxData++) = Instance->RX;
			RxXferCount--;
		}

		if(HW_CheckTimeout(tickstart, Timeout))
		{
			DBG_PRINT("SPI xfer timeout!(%04x->%04x > %04x)\r\n", (UINT16)tickstart, HW_GetTimer(), (UINT16)Timeout);
			errorcode = -2;
			goto HW_SPI_XferError;
		}
	}

	/* Wait until TXE flag */
	if(SPIx_WaitFlagStateUntilTimeout(Instance,SPI_STATUS_TXEMPTY_Msk, 1, Timeout, tickstart) < 0)
	{
		DBG_PRINT("SPI wait TXE timeout!\r\n");		
		errorcode = -3;
		goto HW_SPI_XferError;
	}
  
	/* Check Busy flag */
	if(SPIx_WaitFlagStateUntilTimeout(Instance,SPI_STATUS_BUSY_Msk, 0, Timeout, tickstart) < 0)
	{
		DBG_PRINT("SPI wait BSY timeout!\r\n");
		errorcode = -4;
		goto HW_SPI_XferError;
	}

	/* Clear overrun flag in 2 Lines communication mode because received is not read */
	Instance->STATUS = SPI_STATUS_RXOVIF_Msk; 
	Instance->STATUS = SPI_STATUS_RXTOIF_Msk;

HW_SPI_XferError:
	/* CS = high */	
	
	return errorcode;

} /* End of HW_SPI_TransmitReceive() */



unsigned char SPIx_WriteReadByte(SPI_T* Instance,unsigned char byte)
{
	unsigned char temp;
	
	Instance->TX=byte;
	while((Instance->STATUS & SPI_STATUS_RXEMPTY_Msk));
	temp=Instance->RX;
	
	return temp;
}




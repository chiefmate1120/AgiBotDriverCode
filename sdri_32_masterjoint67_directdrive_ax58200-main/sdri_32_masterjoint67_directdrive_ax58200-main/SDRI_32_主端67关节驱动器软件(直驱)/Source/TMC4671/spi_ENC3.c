#include "AX58200_Hw.h"
#include "task_tmc4671.h"
#include "spi_4671.h"
#include "spi_ENC3.h"
static INT32 SPI0_WaitFlagStateUntilTimeout( UINT32 Flag, UINT32 State, UINT32 Timeout, UINT32 Tickstart)
{
	while((((SPI0->STATUS & Flag) == (Flag)) ? 1 : 0) != State)
	{
		if(HW_CheckTimeout(Tickstart, Timeout))
		{
			SPI_DisableInt(SPI0, SPI_FIFO_TXTH_INT_MASK|SPI_FIFO_RXTH_INT_MASK|SPI_SLVBE_INT_MASK);				
			SPI_DISABLE(SPI0);
			DBG_PRINT("SPI wait flag timeout!\r\n");
			return -1;
		}
	}

	return 0;
}
static INT32 SPI0_TransmitReceive(UINT8 *pTxData, UINT8 *pRxData, UINT16 Size, UINT32 Timeout)
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
		*((__IO uint8_t*)&(SPI0->TX)) = (*pTxData);
		pTxData += sizeof(uint8_t);
		TxXferCount--;
	}
	
	while((TxXferCount > 0U) || (RxXferCount > 0U))
	{
		/* check TXE flag */
		if((TxXferCount > 0U) && (SPI0->STATUS & SPI_STATUS_TXEMPTY_Msk))
		{
			*(__IO uint8_t *)&(SPI0->TX) = (*pTxData++);
			TxXferCount--;
		}

		/* Wait until RXNE flag is reset */
		if((RxXferCount > 0U) && (SPI0->STATUS & SPI_STATUS_RXEMPTY_Msk)==0)
		{
			(*(uint8_t *)pRxData++) = SPI0->RX;
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
	if(SPI0_WaitFlagStateUntilTimeout(SPI_STATUS_TXEMPTY_Msk, 1, Timeout, tickstart) < 0)
	{
		DBG_PRINT("SPI wait TXE timeout!\r\n");		
		errorcode = -3;
		goto HW_SPI_XferError;
	}
  
	/* Check Busy flag */
	if(SPI0_WaitFlagStateUntilTimeout(SPI_STATUS_BUSY_Msk, 0, Timeout, tickstart) < 0)
	{
		DBG_PRINT("SPI wait BSY timeout!\r\n");
		errorcode = -4;
		goto HW_SPI_XferError;
	}

	/* Clear overrun flag in 2 Lines communication mode because received is not read */
	SPI0->STATUS = SPI_STATUS_RXOVIF_Msk; 
	SPI0->STATUS = SPI_STATUS_RXTOIF_Msk;

HW_SPI_XferError:
	/* CS = high */	
	
	return errorcode;

} /* End of HW_SPI_TransmitReceive() */

unsigned char enc3_readwriteByte(uint8_t motor, uint8_t data, uint8_t lastTransfer)
{
	uint8_t Rxdata;
	SPI0_TransmitReceive(&data,&Rxdata, 1, 1000);
	return Rxdata;
}


void ENC3_spi_init(void)
{
	SYS->GPA_MFPL |= SYS_GPA_MFPL_PA2MFP_SPI0_CLK|SYS_GPA_MFPL_PA1MFP_SPI0_MISO;
	SYS->GPB_MFPH |= SYS_GPB_MFPH_PB12MFP_SPI0_MOSI;
	PA->SMTEN |= BIT_2;
	
	HW_MultiFuncPins(PB, BIT_13, 0);
	GPIO_SetMode(PB, BIT_13, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PB, BIT_13, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PB, BIT_13, GPIO_PUSEL_PULL_UP);
	ENC3_spi_cs_H;
//	SPI_DisableAutoSS(SPI0);
	SPI_Open(SPI0, SPI_MASTER, SPI_MODE_1, 8, HW_SPI_ESC_BAUDRATE/30);
}

unsigned char enc3_writereadbyte(unsigned char byte)
{
	unsigned char temp;
	
	SPI0->TX=byte;
	while((SPI0->STATUS & SPI_STATUS_RXEMPTY_Msk));
	temp=SPI0->RX;
	
	return temp;
}

unsigned int ENC3_read(void)
{
	unsigned int res;
	
	SPI_ENABLE(SPI0);
	ENC3_spi_cs_L;
	res=0;
	res|=enc3_writereadbyte(0);
	res<<=8;
	res|=enc3_writereadbyte(0);
	res<<=8;
	res|=enc3_writereadbyte(0);
	res<<=8;
	res|=enc3_writereadbyte(0);
	res>>=6;
	ENC3_spi_cs_H;
	SPI_DISABLE(SPI0);
	
	return res;
}
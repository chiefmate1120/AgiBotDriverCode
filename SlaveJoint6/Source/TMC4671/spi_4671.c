#include "spi_4671.h"


#define		true	1
#define 	false   0 
	
extern void delay_us(unsigned int us);

void TMC4671_SPI_Init(void)
{
	/* Enable SPI clock source */
	HW_SPI_ClkSource(SPI1, ENABLE);
	SYS->GPC_MFPL = SYS_GPC_MFPL_PC3MFP_SPI1_MISO | SYS_GPC_MFPL_PC2MFP_SPI1_MOSI | SYS_GPC_MFPL_PC1MFP_SPI1_CLK;

	//SPI CLK
	GPIO_SetMode(PC, BIT_1, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PC, BIT_1, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_1, GPIO_PUSEL_PULL_UP);
	//Enable clock pin schmitt trigger	
	PC->SMTEN |= BIT_1;
	//SPI MISO
	GPIO_SetMode(PC, BIT_3, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PC, BIT_3, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_3, GPIO_PUSEL_DISABLE);
	//SPI MOSI 
	GPIO_SetMode(PC, BIT_2, GPIO_MODE_OUTPUT);	
	GPIO_SetSlewCtl(PC, BIT_2, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_2, GPIO_PUSEL_PULL_UP);
    
	// SPI CS 
	HW_MultiFuncPins(PC, BIT_0, 0);		
	GPIO_SetMode(PC, BIT_0, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PC, BIT_0, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_0, GPIO_PUSEL_PULL_UP);	   
    //TMC4671_EN
	GPIO_SetMode(PB, BIT_6, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PB, BIT_6, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PB, BIT_6, GPIO_PUSEL_PULL_UP);
    
	PB->SMTEN |= BIT_1;	
    //Enable SPI
	SPI_Open(SPI1, SPI_MASTER, SPI_MODE_3, 8, SPI1_SPEED);	
	SPI_SET_MSB_FIRST(SPI1);
	
    HW_GPIO_WritePin(PB,BIT_6, 0);
	SPI1_CS1_H; 
	SPI_ENABLE(SPI1);
} /* End of HW_SPI_Init() */

static INT32 SPI_WaitFlagStateUntilTimeout( UINT32 Flag, UINT32 State, UINT32 Timeout, UINT32 Tickstart)
{
	
	while((((SPI1->STATUS & Flag) == (Flag)) ? 1 : 0) != State)
	{
		if(HW_CheckTimeout(Tickstart, Timeout))
		{
			SPI_DisableInt(SPI1, SPI_FIFO_TXTH_INT_MASK|SPI_FIFO_RXTH_INT_MASK|SPI_SLVBE_INT_MASK);				
			SPI_DISABLE(SPI1);
			DBG_PRINT("SPI wait flag timeout!\r\n");
			return -1;
		}
	}

	return 0;
	
} /* End of HW_SPI_WaitFlagStateUntilTimeout() */


static INT32 SPI_TransmitReceive(UINT8 *pTxData, UINT8 *pRxData, UINT16 Size, UINT32 Timeout)
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
		*((__IO uint8_t*)&(SPI1->TX)) = (*pTxData);
		pTxData += sizeof(uint8_t);
		TxXferCount--;
	}
	
	while((TxXferCount > 0U) || (RxXferCount > 0U))
	{
		/* check TXE flag */
		if((TxXferCount > 0U) && (SPI1->STATUS & SPI_STATUS_TXEMPTY_Msk))
		{
			*(__IO uint8_t *)&(SPI1->TX) = (*pTxData++);
			TxXferCount--;
		}

		/* Wait until RXNE flag is reset */
		if((RxXferCount > 0U) && (SPI1->STATUS & SPI_STATUS_RXEMPTY_Msk)==0)
		{
			(*(uint8_t *)pRxData++) = SPI1->RX;
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
//	if(SPI_WaitFlagStateUntilTimeout(SPI_STATUS_TXEMPTY_Msk, 1, Timeout, tickstart) < 0)
//	{
//		DBG_PRINT("SPI wait TXE timeout!\r\n");		
//		errorcode = -3;
//		goto HW_SPI_XferError;
//	}
//  
//	/* Check Busy flag */
//	if(SPI_WaitFlagStateUntilTimeout(SPI_STATUS_BUSY_Msk, 0, Timeout, tickstart) < 0)
//	{
//		DBG_PRINT("SPI wait BSY timeout!\r\n");
//		errorcode = -4;
//		goto HW_SPI_XferError;
//	}

	/* Clear overrun flag in 2 Lines communication mode because received is not read */
	SPI1->STATUS = SPI_STATUS_RXOVIF_Msk; 
	SPI1->STATUS = SPI_STATUS_RXTOIF_Msk;

HW_SPI_XferError:
	/* CS = high */	
	
	return errorcode;

} /* End of HW_SPI_TransmitReceive() */


/*
 first data in pTxData is address
*/
INT32 SPI_TransmitReceive_WithAddress(UINT8 *pTxData, UINT8 *pRxData, UINT16 Size, UINT32 Timeout)
{
	UINT32 tickstart = 0U, TxXferCount=0, RxXferCount=0;
	INT16 errorcode = 0;
	BOOL addressData =true;

	/* Init tickstart for timeout management*/
	tickstart = HW_GetTimer();

	TxXferCount = Size;
	RxXferCount = Size;

	/* Check if the SPI is already enabled */


	/* CS = low */

	/* Transmit and Receive data in 8 Bit mode */
	if(TxXferCount >= 0x01)
	{
		*((__IO uint8_t*)&(SPI1->TX)) = (*pTxData);
		pTxData += sizeof(uint8_t);
		TxXferCount--;
	}

	while((TxXferCount > 0U) || (RxXferCount > 0U))
	{
		/* check TXE flag */
		if((TxXferCount > 0U) && (SPI1->STATUS & SPI_STATUS_TXEMPTY_Msk))
		{
			// wait 1us after address sent
			if (addressData){
				delay_us(2);
				addressData = false;
			}
			*(__IO uint8_t *)&(SPI1->TX) = (*pTxData++);
			TxXferCount--;
		}

		/* Wait until RXNE flag is reset */
		if((RxXferCount > 0U) && (SPI1->STATUS & SPI_STATUS_RXEMPTY_Msk)==0)
		{
			(*(uint8_t *)pRxData++) = SPI1->RX;
			RxXferCount--;
		}

		if(HW_CheckTimeout(tickstart, Timeout))
		{
			DBG_PRINT("SPI xfer timeout!(%04x->%04x > %04x)\r\n", (UINT16)tickstart, HW_GetTimer(), (UINT16)Timeout);
			errorcode = -2;
			goto HW_SPI_XferError;
		}
	}

//	/* Wait until TXE flag */
//	if(SPI_WaitFlagStateUntilTimeout(SPI_STATUS_TXEMPTY_Msk, 1, Timeout, tickstart) < 0)
//	{
//		DBG_PRINT("SPI wait TXE timeout!\r\n");		
//		errorcode = -3;
//		goto HW_SPI_XferError;
//	}

//	/* Check Busy flag */
//	if(SPI_WaitFlagStateUntilTimeout(SPI_STATUS_BUSY_Msk, 0, Timeout, tickstart) < 0)
//	{
//		DBG_PRINT("SPI wait BSY timeout!\r\n");
//		errorcode = -4;
//		goto HW_SPI_XferError;
//	}

	/* Clear overrun flag in 2 Lines communication mode because received is not read */
	SPI1->STATUS = SPI_STATUS_RXOVIF_Msk; 
	SPI1->STATUS = SPI_STATUS_RXTOIF_Msk;

HW_SPI_XferError:
	/* CS = high */	

	return errorcode;

}

uint8_t tmc4671_readwriteByte(uint8_t motor, uint8_t data, uint8_t lastTransfer)
{
	uint8_t Rxdata;
	SPI_TransmitReceive(&data,&Rxdata, 1, 1000);
	return Rxdata;
}

void tmc4671_writeInt(uint8_t motor, uint8_t address, int32_t value)
{
	uint8_t RxData[5];
	uint8_t TxData[5];	
	
	SPI_ENABLE(SPI1);
	SPI1_CS1_L;

	TxData[0]=address|0x80;
    
    TxData[1]=0xFF & (value>>24);
	TxData[2]=0xFF & (value>>16);
	TxData[3]=0xFF & (value>>8);
	TxData[4]=0xFF & (value>>0);
	SPI_TransmitReceive_WithAddress(TxData,RxData, 5, 1000);

	SPI1_CS1_H;
}
int32_t tmc4671_readInt(uint8_t motor, uint8_t address)
{
	uint8_t RxData[5];
	uint8_t TxData[5];	
	int32_t value;	
	
	SPI_ENABLE(SPI1);
	SPI1_CS1_L;

	TxData[0]=address&0x7F;

	SPI_TransmitReceive_WithAddress(TxData,RxData, 5, 1000);

	value=RxData[1];
	value<<=8;
	value |= RxData[2];
	value<<=8;
	value |= RxData[3];
	value<<=8;
	value |= RxData[4];

	SPI1_CS1_H;
	return value;
}

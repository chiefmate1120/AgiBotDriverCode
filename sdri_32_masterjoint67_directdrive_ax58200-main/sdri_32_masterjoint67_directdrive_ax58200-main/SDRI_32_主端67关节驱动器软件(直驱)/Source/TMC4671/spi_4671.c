#include "AX58200_Hw.h"

#define		SPI1_CS_L	HW_GPIO_WritePin(PC, BIT_0, 0)
#define 	SPI1_CS_H	HW_GPIO_WritePin(PC, BIT_0, 1)

#define		SCK_H	HW_GPIO_WritePin(PC, BIT_1, 1)
#define		SCK_L	HW_GPIO_WritePin(PC, BIT_1, 0)

#define		MOSI_H	HW_GPIO_WritePin(PC, BIT_2, 1)
#define		MOSI_L	HW_GPIO_WritePin(PC, BIT_2, 0)

#define		MISO	HW_GPIO_ReadPin(PC,BIT_3)

#define		SPI2_CS_L	HW_GPIO_WritePin(PE, BIT_5, 0)
#define 	SPI2_CS_H	HW_GPIO_WritePin(PE, BIT_5, 1)

#define		SCK2_H	HW_GPIO_WritePin(PE, BIT_4, 1)
#define		SCK2_L	HW_GPIO_WritePin(PE, BIT_4, 0)

#define		MOSI2_H	HW_GPIO_WritePin(PE, BIT_2, 1)
#define		MOSI2_L	HW_GPIO_WritePin(PE, BIT_2, 0)

#define		MISO2	HW_GPIO_ReadPin(PE,BIT_3)

#define		true	1
#define 	false   0 

unsigned char Rx_data[5];

extern void delay_us(unsigned int us);
extern unsigned char slave;
void TMC4671_SPI_Init(void)
{
	HW_MultiFuncPins(PC, BIT_0, 0);		
	GPIO_SetMode(PC, BIT_0, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PC, BIT_0, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_0, GPIO_PUSEL_PULL_UP);
	
	GPIO_SetMode(PC, BIT_1, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PC, BIT_1, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_1, GPIO_PUSEL_PULL_UP);
		
	
	GPIO_SetMode(PC, BIT_3, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PC, BIT_3, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_3, GPIO_PUSEL_PULL_DOWN);

	
	GPIO_SetMode(PC, BIT_2, GPIO_MODE_OUTPUT);	
	GPIO_SetSlewCtl(PC, BIT_2, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PC, BIT_2, GPIO_PUSEL_PULL_UP);

			
	GPIO_SetMode(PE, BIT_4, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PE, BIT_4, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_4, GPIO_PUSEL_PULL_DOWN);
	
	GPIO_SetMode(PE, BIT_6, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PE, BIT_6, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_6, GPIO_PUSEL_PULL_DOWN);
	
	GPIO_SetMode(PE, BIT_5, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PE, BIT_5, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_5, GPIO_PUSEL_PULL_UP);
	
	GPIO_SetMode(PE, BIT_4, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PE, BIT_4, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_4, GPIO_PUSEL_PULL_DOWN);
	
	GPIO_SetMode(PE, BIT_2, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PE, BIT_2, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_2, GPIO_PUSEL_PULL_DOWN);
	
	GPIO_SetMode(PE, BIT_3, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PE, BIT_3, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_3, GPIO_PUSEL_PULL_DOWN);
	
	
	HW_GPIO_WritePin(PC, BIT_0, 1);
	SCK_H;
	HW_GPIO_WritePin(PE, BIT_5, 1);
	SCK2_H;
}

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



static void SPI_TransmitReceive(UINT8 *pTxData, UINT8 *pRxData)
{
//	UINT32 tickstart = 0U, TxXferCount=0, RxXferCount=0;
//	INT16 errorcode = 0;

//	TxXferCount = Size;
//	RxXferCount = Size;
//	
//	if(TxXferCount == 0x01)
//	{
//		*((__IO uint8_t*)&(SPI1->TX)) = (*pTxData);
//		pTxData += sizeof(uint8_t);
//		TxXferCount--;
//	}
//	
//	while( RxXferCount > 0U)
//	{
//		if((SPI1->STATUS & SPI_STATUS_RXEMPTY_Msk)==0)
//		{
//			(*(uint8_t *)pRxData++) = SPI1->RX;
//			RxXferCount--;
//		}

//	}

//	/* Wait until TXE flag */
//	if(SPI_WaitFlagStateUntilTimeout(SPI_STATUS_TXEMPTY_Msk, 1, Timeout, tickstart) < 0)
//	{	
//		errorcode = -3;
//	}
//  
//	/* Check Busy flag */
//	if(SPI_WaitFlagStateUntilTimeout(SPI_STATUS_BUSY_Msk, 0, Timeout, tickstart) < 0)
//	{
//		errorcode = -4;
//	}

//	/* Clear overrun flag in 2 Lines communication mode because received is not read */
//	SPI1->STATUS = SPI_STATUS_RXOVIF_Msk; 
//	SPI1->STATUS = SPI_STATUS_RXTOIF_Msk;

//	/* CS = high */	
//	
//	SPI1->TX=*pTxData;
//	while((SPI1->STATUS & SPI_STATUS_RXEMPTY_Msk));
//	*pRxData=SPI1->RX;
	
	unsigned char tx,rx,count=0;
	if(slave==1)
	{
		tx=*pTxData;
		rx=0;
		for(count=0;count<8;count++)
		{
			SCK_L;
			rx<<=1;
			if(tx&0x80)
			{
				MOSI_H;
			}
			else
			{
				MOSI_L;
			}
			tx<<=1;
			SCK_H;
			if(MISO)
			rx|=1;
		}
		*pRxData=rx;
	}
	else if(slave==2)
	{
		tx=*pTxData;
		rx=0;
		for(count=0;count<8;count++)
		{
			SCK2_L;
			rx<<=1;
			if(tx&0x80)
			{
				MOSI2_H;
			}
			else
			{
				MOSI2_L;
			}
			tx<<=1;
			SCK2_H;
			if(MISO2)
			rx|=1;
		}
		*pRxData=rx;
	}
		
} /* End of HW_SPI_TransmitReceive() */

uint8_t tmc4671_readwriteByte(uint8_t motor, uint8_t data, uint8_t lastTransfer)
{
	uint8_t Rxdata;
//	SPI_TransmitReceive(&data,&Rxdata, 1, 10);
	return Rxdata;
}

void tmc4671_writeInt(uint8_t motor, uint8_t address, int32_t value)
{

}

void tmc4671_write_withbuff(uint8_t address, int32_t value, uint8_t *rx_data)
{
	unsigned char temp;
//	SPI1_CS_L;
	temp=address;
	SPI_TransmitReceive(&temp,&rx_data[0]);
	temp=0xFF & (value>>24);
	SPI_TransmitReceive(&temp,&rx_data[1]);
	temp=0xFF & (value>>16);
	SPI_TransmitReceive(&temp,&rx_data[2]);
	temp=0xFF & (value>>8);
	SPI_TransmitReceive(&temp,&rx_data[3]);
	temp=0xFF & (value>>0);
	SPI_TransmitReceive(&temp,&rx_data[4]);
//	SPI1_CS_H;
}


void tmc4671_write(uint8_t address, int32_t value)
{
	unsigned char temp;
//	SPI1_CS_L;
	temp=address;
	SPI_TransmitReceive(&temp,&Rx_data[0]);
	temp=0xFF & (value>>24);
	SPI_TransmitReceive(&temp,&Rx_data[1]);
	temp=0xFF & (value>>16);
	SPI_TransmitReceive(&temp,&Rx_data[2]);
	temp=0xFF & (value>>8);
	SPI_TransmitReceive(&temp,&Rx_data[3]);
	temp=0xFF & (value>>0);
	SPI_TransmitReceive(&temp,&Rx_data[4]);
//	SPI1_CS_H;
}

int32_t tmc4671_readInt(uint8_t motor, uint8_t address)
{
	SPI_ENABLE(SPI1);
	// clear write bit
	address &= 0x7F;
	SPI1_CS_L;
	// write address
	delay_us(5);
	tmc4671_readwriteByte(motor, address, false);

	// read data
	int32_t value = tmc4671_readwriteByte(motor, 0, false);
	value <<= 8;
	value |= tmc4671_readwriteByte(motor, 0, false);
	value <<= 8;
	value |= tmc4671_readwriteByte(motor, 0, false);
	value <<= 8;
	value |= tmc4671_readwriteByte(motor, 0, true);
	delay_us(5);
	SPI1_CS_H;
	return value;
}

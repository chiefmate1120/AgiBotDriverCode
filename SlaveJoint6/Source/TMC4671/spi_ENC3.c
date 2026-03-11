#include "AX58200_Hw.h"
#include "task_tmc4671.h"
#include "spi_4671.h"
#include "spi_ENC3.h"
/*
PE2 ->  ENC_SPI_MOSI
PE3 ->  ENC_SPI_MISO
PE4 ->  ENC_SPI_SCK

PE5 ->  ENC_SPI_CS1
PE7 ->  ENC_SPI_CS2
使用SPI3  
*/
//FR_ENCClass FR_ENC;
//uint8_t biss_crc_table[64]={
//  0x00, 0x03, 0x06, 0x05, 0x0C, 0x0F, 0x0A, 0x09,
//  0x18, 0x1B, 0x1E, 0x1D, 0x14, 0x17, 0x12, 0x11,
//  0x30, 0x33, 0x36, 0x35, 0x3C, 0x3F, 0x3A, 0x39,
//  0x28, 0x2B, 0x2E, 0x2D, 0x24, 0x27, 0x22, 0x21,
//  0x23, 0x20, 0x25, 0x26, 0x2F, 0x2C, 0x29, 0x2A,
//  0x3B, 0x38, 0x3D, 0x3E, 0x37, 0x34, 0x31, 0x32,
//  0x13, 0x10, 0x15, 0x16, 0x1F, 0x1C, 0x19, 0x1A,
//  0x0B, 0x08, 0x0D, 0x0E, 0x07, 0x04, 0x01, 0x02
//};





void ENC3_spi_init(void)
{    
	/* Enable SPI clock source */
	HW_SPI_ClkSource(SPI3, ENABLE);

	SYS->GPE_MFPL = SYS_GPE_MFPL_PE3MFP_SPI3_MISO | SYS_GPE_MFPL_PE2MFP_SPI3_MOSI | SYS_GPE_MFPL_PE4MFP_SPI3_CLK;
	/* SPI ESC CS GPIO pin configuration  */
//	HW_MultiFuncPins(PE, BIT_2, 0);		
//	GPIO_SetMode(PE, BIT_2, GPIO_MODE_OUTPUT);
//	GPIO_SetSlewCtl(PE, BIT_2, GPIO_SLEWCTL_FAST);
//	GPIO_SetPullCtl(PE, BIT_2, GPIO_PUSEL_PULL_UP);
//	
//	HW_MultiFuncPins(PE, BIT_3, 0);		
//	GPIO_SetMode(PE, BIT_3, GPIO_MODE_INPUT);
//	GPIO_SetSlewCtl(PE, BIT_3, GPIO_SLEWCTL_FAST);
//	GPIO_SetPullCtl(PE, BIT_3, GPIO_PUSEL_DISABLE);
//	
//	HW_MultiFuncPins(PE, BIT_4, 0);		
//	GPIO_SetMode(PE, BIT_4, GPIO_MODE_OUTPUT);
//	GPIO_SetSlewCtl(PE, BIT_4, GPIO_SLEWCTL_FAST);
//	GPIO_SetPullCtl(PE, BIT_4, GPIO_PUSEL_PULL_UP);
	PE->SMTEN |= BIT_4;	
	HW_MultiFuncPins(PE, BIT_5, 0);		
	GPIO_SetMode(PE, BIT_5, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PE, BIT_5, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_5, GPIO_PUSEL_PULL_UP);
	

	SPI_Open(SPI3, SPI_MASTER, SPI_MODE_3, 8, 5000000);	
	SPI_ENABLE(SPI3);
	SPI3_CS1_H;   
   
}

static INT32 SPI3_WaitFlagStateUntilTimeout( UINT32 Flag, UINT32 State, UINT32 Timeout, UINT32 Tickstart)
{
	while((((SPI3->STATUS & Flag) == (Flag)) ? 1 : 0) != State)
	{
		if(HW_CheckTimeout(Tickstart, Timeout))
		{
			SPI_DisableInt(SPI3, SPI_FIFO_TXTH_INT_MASK|SPI_FIFO_RXTH_INT_MASK|SPI_SLVBE_INT_MASK);				
			SPI_DISABLE(SPI3);
			DBG_PRINT("SPI wait flag timeout!\r\n");
			return -1;
		}
	}

	return 0;
}
static INT32 ENC_SPI_TransmitReceive(UINT8 *pTxData, UINT8 *pRxData, UINT16 Size, UINT32 Timeout)
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
		*((__IO uint8_t*)&(SPI3->TX)) = (*pTxData);
		pTxData += sizeof(uint8_t);
		TxXferCount--;
	}
	
	while((TxXferCount > 0U) || (RxXferCount > 0U))
	{
		/* check TXE flag */
		if((TxXferCount > 0U) && (SPI3->STATUS & SPI_STATUS_TXEMPTY_Msk))
		{
			*(__IO uint8_t *)&(SPI3->TX) = (*pTxData++);
			TxXferCount--;
		}

		/* Wait until RXNE flag is reset */
		if((RxXferCount > 0U) && (SPI3->STATUS & SPI_STATUS_RXEMPTY_Msk)==0)
		{
			(*(uint8_t *)pRxData++) = SPI3->RX;
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
//	if(SPI3_WaitFlagStateUntilTimeout(SPI_STATUS_TXEMPTY_Msk, 1, Timeout, tickstart) < 0)
//	{
//		DBG_PRINT("SPI wait TXE timeout!\r\n");		
//		errorcode = -3;
//		goto HW_SPI_XferError;
//	}
  
	/* Check Busy flag */
//	if(SPI3_WaitFlagStateUntilTimeout(SPI_STATUS_BUSY_Msk, 0, Timeout, tickstart) < 0)
//	{
//		DBG_PRINT("SPI wait BSY timeout!\r\n");
//		errorcode = -4;
//		goto HW_SPI_XferError;
//	}

	/* Clear overrun flag in 2 Lines communication mode because received is not read */
	SPI3->STATUS = SPI_STATUS_RXOVIF_Msk; 
	SPI3->STATUS = SPI_STATUS_RXTOIF_Msk;

HW_SPI_XferError:
	/* CS = high */	
	
	return errorcode;

} /* End of HW_SPI_TransmitReceive() */


uint8_t ENC_readwriteByte(uint8_t motor, uint8_t data, uint8_t lastTransfer)
{
	uint8_t Rxdata;
	ENC_SPI_TransmitReceive(&data,&Rxdata, 1, 1000);
	return Rxdata;
}

unsigned char SPI3_TransmitReceive(unsigned char TxData)
{
	int RxData;
	
	SPI3->TX=TxData;
//	while(!((SPI3->STATUS & SPI_STATUS_RXEMPTY_Msk) == (SPI_STATUS_RXEMPTY_Msk)));
	while((SPI3->STATUS & SPI_STATUS_RXEMPTY_Msk));
	RxData=SPI3->RX;
	
	return RxData;
}

/************************数据读取*******************************/
int32_t FR_ENC1_readInt(uint8_t motor, uint8_t address)
{
  	uint8_t RxData[5];
	uint8_t TxData[5];    
	int32_t value;  
    int32_t CRC_DATA,CRC_Result;
//	SPI_ENABLE(SPI3);
    SPI3_CS1_L;

	ENC_SPI_TransmitReceive(TxData,RxData, 3, 1000);    
	value =  RxData[0];
	value<<=8;
	value |= RxData[1];
	value<<=8;
	value |= RxData[2];
    
    FR_ENC.FR_CRCData = value&0x3f;
    FR_ENC.FR_CRCVerify = biss_crc32_lookup(value>>6);
    FR_ENC.FR_ErrorFlag = (value>>6&0x01);    
    if((FR_ENC.FR_CRCData != FR_ENC.FR_CRCVerify) || (FR_ENC.FR_ErrorFlag==1))
    {
       FR_ENC.FR_CRC_Flag=1; 
        //数据报错
       if(FR_ENC.FR_ErrorFlag==1)
       {                        
           FR_ENC.FR_ErrorFlagTimes++; 
           if( FR_ENC.FR_ErrorFlagTimes>100)
           {
               FR_ENC.FR_ErrorFlagTimes_Flag=1;  //连续100次报错
           }
       }
       else //校验报错
       {
           //如果出现CRC报错，但是跳变不超过20时，认为没有报错
           if(abs((value>>7) - FR_ENC.FR_Motor1Data1) < 20)
           {
               FR_ENC.FR_CRC_Flag=0;   
           }
           
           //保持记录报错次数，连续超过100次时，认为异常
           FR_ENC.FR_CRCVerifyErrorTimes++;
           if(FR_ENC.FR_CRCVerifyErrorTimes>100)
           {
               FR_ENC.FR_CRCVerifyErrorTimes_Flag=1;  //连续100次报错             
           }
       }
    }
    else
    {
        FR_ENC.FR_CRC_Flag=0;
        FR_ENC.FR_CRCVerifyErrorTimes=0;
        FR_ENC.FR_CRCVerifyErrorTimes_Flag=0;
        FR_ENC.FR_ErrorFlagTimes=0;
        FR_ENC.FR_ErrorFlagTimes_Flag=0;
    }       

    value>>=7;
    
	SPI3_CS1_H;
//	SPI_DISABLE(SPI3);	
	
	return value;
}





//uint8_t biss_crc32_lookup(uint32_t in_data32)
//{
//  uint8_t crc_result;
//  
//  uint8_t crc[6];  
//  for(uint8_t i=0; i<6; i++)
//  {
//    crc[i]=(uint8_t)((in_data32>>(i*6))&0x3F);            //将32字节分解成6个bit的字节。
//  }
//  
//  crc_result=biss_crc_table[crc[0]^biss_crc_table[crc[1]^biss_crc_table[crc[2]^biss_crc_table[crc[3]^biss_crc_table[crc[4]^biss_crc_table[crc[5]]]]]]];
//  crc_result=(~crc_result)&0x3F;
//  return crc_result;
//}



//int FRReadVersion(void)
//{
// 	uint8_t RxData[4];
//	uint8_t TxData[4];	
//	int32_t value;    
//     
//    SPI3_CS1_L;
//    
//    TxData[0]=0xEA;
//    TxData[1]=0x55;    
//    TxData[2]=0xBF;   
//	ENC_SPI_TransmitReceive(TxData,RxData, 3, 1000);   
//  
//	SPI3_CS1_H;

//    
//    value = (RxData[0]<<16 | RxData[1]<<8 | RxData[2]) >> 7;
//    return value;   
//}


//void FRWriteJZ(void)
//{
// 	uint8_t RxData[4];
//	uint8_t TxData[4];	
//    
//	 SPI3_CS1_L;
//    
//    TxData[0]=0x7A;
//    TxData[1]=0x55;    
//    TxData[2]=0x2F;   
//    ENC_SPI_TransmitReceive(TxData,RxData, 3, 1000);  

//	SPI3_CS1_H;
//}










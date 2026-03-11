#include "SPI_Simulation.h"
#include "spi_ENC3.h"

FR_ENCClass FR_ENC;
uint8_t biss_crc_table[64]={
  0x00, 0x03, 0x06, 0x05, 0x0C, 0x0F, 0x0A, 0x09,
  0x18, 0x1B, 0x1E, 0x1D, 0x14, 0x17, 0x12, 0x11,
  0x30, 0x33, 0x36, 0x35, 0x3C, 0x3F, 0x3A, 0x39,
  0x28, 0x2B, 0x2E, 0x2D, 0x24, 0x27, 0x22, 0x21,
  0x23, 0x20, 0x25, 0x26, 0x2F, 0x2C, 0x29, 0x2A,
  0x3B, 0x38, 0x3D, 0x3E, 0x37, 0x34, 0x31, 0x32,
  0x13, 0x10, 0x15, 0x16, 0x1F, 0x1C, 0x19, 0x1A,
  0x0B, 0x08, 0x0D, 0x0E, 0x07, 0x04, 0x01, 0x02
};

void SimulationSPI_init(void)
{   
	//CS
	HW_MultiFuncPins(PE, BIT_5, 0);
	GPIO_SetMode(PE, BIT_5, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PE, BIT_5, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_5, GPIO_PUSEL_PULL_UP); 
    //MISO
	HW_MultiFuncPins(PE, BIT_3, 0);    
	GPIO_SetMode(PE, BIT_3, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PE, BIT_3, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_3, GPIO_PUSEL_PULL_UP);
	//CLK
	HW_MultiFuncPins(PE, BIT_4, 0);    
	GPIO_SetMode(PE, BIT_4, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PE, BIT_4, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_4, GPIO_PUSEL_PULL_UP);
	PE->SMTEN |= BIT_4;     
	//MOSI
	HW_MultiFuncPins(PE, BIT_2, 0);    
	GPIO_SetMode(PE, BIT_2, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PE, BIT_2, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_2, GPIO_PUSEL_PULL_UP);    

    SPI_CS_GPIO_H;   
}



void SimulationSPI_TransmitReceive(UINT8 *pTxData, UINT8 *pRxData)
{
	unsigned char tx,rx,count=0;
	
	tx=*pTxData;
	rx=0;
	for(count=0;count<8;count++)
	{
		SPI_SCLK_GPIO_L;
		rx<<=1;
		if(tx&0x80)
		{
			SPI_MOSI_GPIO_H;
		}
		else
		{
			SPI_MOSI_GPIO_L;
		}
		tx<<=1;
		SPI_SCLK_GPIO_H;
		if(SPI_MOSI_GPIO)
		rx|=1;
	}
	*pRxData=rx;    
}


unsigned int SimulationSPI_FRENCRead()
{
    uint8_t RxData[3];      
	uint32_t value=0;  
	unsigned char temp=0;        
    SPI_CS_GPIO_L; 
	SimulationSPI_TransmitReceive(0,&RxData[0]);
	SimulationSPI_TransmitReceive(0,&RxData[1]);
	SimulationSPI_TransmitReceive(0,&RxData[2]);
  
    SPI_CS_GPIO_H;
	value=RxData[0];
	value<<=8;
	value |= RxData[1];
	value<<=8;
	value |= RxData[2];
        
    FR_ENC.FR_CRCData = value&0x3f;
    FR_ENC.FR_CRCVerify = biss_crc32_lookup(value>>6);
    FR_ENC.FR_ErrorFlag = (value>>6&0x01);    

             
        //数据报错
       if(FR_ENC.FR_ErrorFlag==1)
       {                        
           FR_ENC.FR_ErrorFlagTimes++; 
           if( FR_ENC.FR_ErrorFlagTimes>100)
           {
               FR_ENC.FR_ErrorFlagTimes_Flag=1;  //连续100次报错
           }
       }
       else
       {
            FR_ENC.FR_ErrorFlagTimes=0;
            FR_ENC.FR_ErrorFlagTimes_Flag=0;           
       }
       
       if((FR_ENC.FR_CRCData != FR_ENC.FR_CRCVerify)) //校验报错
       {
           FR_ENC.FR_CRC_Flag=1; 
           
            FR_ENC.FR_CRCVerifyErrorTimes++;
           if(FR_ENC.FR_CRCVerifyErrorTimes>100)
           {
               FR_ENC.FR_CRCVerifyErrorTimes_Flag=1;  //连续100次报错             
           }
       }
       else
       {
            FR_ENC.FR_CRC_Flag=0;
            FR_ENC.FR_CRCVerifyErrorTimes=0;
            FR_ENC.FR_CRCVerifyErrorTimes_Flag=0;           
       }
  

	value >>=7;        
    
    return value;
}


uint8_t biss_crc32_lookup(uint32_t in_data32)
{
  uint8_t crc_result;
  
  uint8_t crc[6];  
  for(uint8_t i=0; i<6; i++)
  {
    crc[i]=(uint8_t)((in_data32>>(i*6))&0x3F);            //将32字节分解成6个bit的字节。
  }
  
  crc_result=biss_crc_table[crc[0]^biss_crc_table[crc[1]^biss_crc_table[crc[2]^biss_crc_table[crc[3]^biss_crc_table[crc[4]^biss_crc_table[crc[5]]]]]]];
  crc_result=(~crc_result)&0x3F;
  return crc_result;
}



int FRReadVersion(void)
{
 	uint8_t RxData[4];
	uint8_t TxData[4];	
	int32_t value;    
     

    
    TxData[0]=0xEA;
    TxData[1]=0x55;    
    TxData[2]=0xBF;  
    
    SPI_CS_GPIO_L; 
	SimulationSPI_TransmitReceive(&TxData[0],&RxData[0]);
	SimulationSPI_TransmitReceive(&TxData[1],&RxData[1]);
	SimulationSPI_TransmitReceive(&TxData[2],&RxData[2]);
  
    SPI_CS_GPIO_H;


    
    value = (RxData[0]<<16 | RxData[1]<<8 | RxData[2]) >> 7;
    return value;   
}


void FRWriteJZ(void)
{
 	uint8_t RxData[4];
	uint8_t TxData[4];	
    
   
    TxData[0]=0x7A;
    TxData[1]=0x55;    
    TxData[2]=0x2F;   
    
    SPI_CS_GPIO_L; 
    
	SimulationSPI_TransmitReceive(&TxData[0],&RxData[0]);
	SimulationSPI_TransmitReceive(&TxData[1],&RxData[1]);
	SimulationSPI_TransmitReceive(&TxData[2],&RxData[2]);
 
    SPI_CS_GPIO_H;

}






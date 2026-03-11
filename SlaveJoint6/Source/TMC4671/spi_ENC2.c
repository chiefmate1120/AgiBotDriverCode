#include "AX58200_Hw.h"
#include "task_tmc4671.h"
#include "spi_4671.h"

void ENC2_spi_init(void)
{
    //´Å±àCS
	HW_MultiFuncPins(PB, BIT_10, 0);
	GPIO_SetMode(PB, BIT_10, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PB, BIT_10, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PB, BIT_10, GPIO_PUSEL_PULL_UP);    
	PB->SMTEN |= BIT_1;
     
    //CLK
	HW_MultiFuncPins(PB, BIT_3, 0);
	GPIO_SetMode(PB, BIT_3, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PB, BIT_3, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PB, BIT_3, GPIO_PUSEL_PULL_UP);    
    //MOSI
 	HW_MultiFuncPins(PB, BIT_4, 0);
	GPIO_SetMode(PB, BIT_4, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PB, BIT_4, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PB, BIT_4, GPIO_PUSEL_PULL_UP);       
    //MISO
	HW_MultiFuncPins(PE, BIT_1, 0);
	GPIO_SetMode(PE, BIT_1, GPIO_MODE_INPUT);
	GPIO_SetSlewCtl(PE, BIT_1, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_1, GPIO_PUSEL_PULL_UP);

	SPI1_CS2_H;
}


int32_t ENC_readInt(uint8_t motor, uint8_t address)
{
  	uint8_t RxData[5];
	uint8_t TxData[5];    
	int32_t value;
	spi1_clr1;
	spi1_select2;	
	SPI_ENABLE(SPI1);
	SPI1_CS2_L;

	TxData[0]=address;	
	SPI_TransmitReceive_WithAddress(TxData,RxData, 5, 1000);
    
	value =  RxData[1];
	value<<=8;
	value |= RxData[2];
	value<<=8;
	value |= RxData[3];
	value >>=7;    

	
	SPI1_CS2_H;
	SPI_DISABLE(SPI1);
	spi1_clr2;
	spi1_select1;
	
	return value;
}

int32_t ICPZENC_readInt(uint8_t motor, uint8_t address)
{
  	uint8_t RxData[5];
	uint8_t TxData[5];    
	int32_t value;
	spi1_clr1;
	spi1_select2;	
	SPI_ENABLE(SPI1);
	SPI1_CS2_L;

	TxData[0]=address;	
	SPI_TransmitReceive_WithAddress(TxData,RxData, 5, 1000);
    
	value =  RxData[1];
	value<<=8;
	value |= RxData[2];
	value<<=8;
	value |= RxData[3];
	value<<=8;
 	value |= RxData[4];
	value>>=8;   
	value&=0x00FFFFFF;	


	SPI1_CS2_H;
	SPI_DISABLE(SPI1);
	spi1_clr2;
	spi1_select1;
	
	return value;
}

void ICPZENC_REBOOT(void)
{
	int32_t value;
	spi1_clr1;
	spi1_select2;	
	SPI_ENABLE(SPI1);

	// clear write bit
	//address &= 0x7F;
	SPI1_CS2_L;
	tmc4671_readwriteByte(0, 0xD9, false);
    tmc4671_readwriteByte(0, 0x10, true);
	SPI1_CS2_H;
	
	SPI_DISABLE(SPI1);
	spi1_clr2;
	spi1_select1;
}
void ENC_writeInt(uint8_t motor, uint8_t address, int32_t value)
{
	spi1_clr1;
	spi1_select2;
	SPI_ENABLE(SPI1);
	SPI1_CS2_L;
	// write address
	tmc4671_readwriteByte(motor, address|0x80, false);
  
	// write value
	tmc4671_readwriteByte(motor, 0xFF & (value>>24), false);
	tmc4671_readwriteByte(motor, 0xFF & (value>>16), false);
	tmc4671_readwriteByte(motor, 0xFF & (value>>8), false);
	tmc4671_readwriteByte(motor, 0xFF & (value>>0), true);
	SPI1_CS2_H;
	spi1_clr2;
	spi1_select1;
}

int32_t ENC_readRegister(uint8_t motor, uint8_t address)
{
	spi1_clr1;
	spi1_select2;
	SPI_ENABLE(SPI1);
	// clear write bit
	SPI1_CS2_L;
	// write address
	tmc4671_readwriteByte(motor, 0x81, false);
	tmc4671_readwriteByte(motor, address, false);
	tmc4671_readwriteByte(motor, 0x00, false);
	// read data
	int32_t value = tmc4671_readwriteByte(motor, 0, false);
	value <<= 8;
	value |= tmc4671_readwriteByte(motor, 0, false);
	value <<= 8;
	value |= tmc4671_readwriteByte(motor, 0, false);
	value <<= 8;
	value |= tmc4671_readwriteByte(motor, 0, true);
	SPI1_CS2_H;
	spi1_clr2;
	spi1_select1;
	SPI_DISABLE(SPI1);
	return value;
}
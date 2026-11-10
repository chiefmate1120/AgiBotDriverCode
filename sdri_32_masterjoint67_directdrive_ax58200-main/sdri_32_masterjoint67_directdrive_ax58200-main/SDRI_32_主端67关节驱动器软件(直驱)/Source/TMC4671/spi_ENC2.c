#include "AX58200_Hw.h"
#include "task_tmc4671.h"
#include "spi_4671.h"

#define		true	1
#define 	false   0 

#define SPI2_CS_L	HW_GPIO_WritePin(PE, BIT_5, 0)
#define SPI2_CS_H	HW_GPIO_WritePin(PE, BIT_5, 1)

#define spi1_select1	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC3MFP_SPI1_MISO | SYS_GPC_MFPL_PC2MFP_SPI1_MOSI | SYS_GPC_MFPL_PC1MFP_SPI1_CLK
#define spi1_select2	SYS->GPE_MFPL |= SYS_GPE_MFPL_PE1MFP_SPI1_MISO ;SYS->GPB_MFPL |=SYS_GPB_MFPL_PB4MFP_SPI1_MOSI | SYS_GPB_MFPL_PB3MFP_SPI1_CLK

#define spi1_clr1	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC3MFP_SPI1_MISO | SYS_GPC_MFPL_PC2MFP_SPI1_MOSI | SYS_GPC_MFPL_PC1MFP_SPI1_CLK)
#define spi1_clr2	SYS->GPE_MFPL &=~(SYS_GPE_MFPL_PE1MFP_SPI1_MISO) ;SYS->GPB_MFPL &=~(SYS_GPB_MFPL_PB4MFP_SPI1_MOSI | SYS_GPB_MFPL_PB3MFP_SPI1_CLK)
void ENC2_spi_init(void)
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
	
	HW_MultiFuncPins(PE, BIT_5, 0);		
	GPIO_SetMode(PE, BIT_5, GPIO_MODE_OUTPUT);
	GPIO_SetSlewCtl(PE, BIT_5, GPIO_SLEWCTL_FAST);
	GPIO_SetPullCtl(PE, BIT_5, GPIO_PUSEL_PULL_UP);
	
//	PE->SMTEN |= BIT_4;
	SPI_Open(SPI3, SPI_MASTER, SPI_MODE_3, 8, 6000000);	
	SPI_ENABLE(SPI3);
	SPI2_CS_H;
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

int32_t ENC_readInt(uint8_t motor, uint8_t address)
{
	int32_t value;

	SPI2_CS_L;
	// write address

//	SPI3_TransmitReceive(address);
	value = SPI3_TransmitReceive(0);
	value <<= 8;
	value |= SPI3_TransmitReceive(0);
//	value <<= 8;
//	value |= SPI3_TransmitReceive(0);
//	value <<= 8;
//	value |= SPI3_TransmitReceive(0);
//	value >>=8;
//	value&=0x00FFFFFF;
	SPI2_CS_H;
	
	
	return value;
}

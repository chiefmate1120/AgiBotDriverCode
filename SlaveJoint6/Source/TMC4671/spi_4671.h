#ifndef _spi_4671_h_
#define _spi_4671_h_
#include "AX58200_Hw.h"

#define		true	1
#define 	false   0 


//TMC4671_CS1
#define	SPI1_CS1_L	HW_GPIO_WritePin(PC, BIT_0, 0)
#define SPI1_CS1_H	HW_GPIO_WritePin(PC, BIT_0, 1)


//ENC_CS1
#define SPI3_CS1_L	HW_GPIO_WritePin(PE, BIT_5, 0)
#define SPI3_CS1_H	HW_GPIO_WritePin(PE, BIT_5, 1)


#define spi1_select1	SYS->GPC_MFPL |= SYS_GPC_MFPL_PC3MFP_SPI1_MISO | SYS_GPC_MFPL_PC2MFP_SPI1_MOSI | SYS_GPC_MFPL_PC1MFP_SPI1_CLK
#define spi1_select2	SYS->GPE_MFPL |= SYS_GPE_MFPL_PE1MFP_SPI1_MISO ;SYS->GPB_MFPL |=SYS_GPB_MFPL_PB4MFP_SPI1_MOSI | SYS_GPB_MFPL_PB3MFP_SPI1_CLK

#define spi1_clr1	SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC3MFP_SPI1_MISO | SYS_GPC_MFPL_PC2MFP_SPI1_MOSI | SYS_GPC_MFPL_PC1MFP_SPI1_CLK)
#define spi1_clr2	SYS->GPE_MFPL &=~(SYS_GPE_MFPL_PE1MFP_SPI1_MISO) ;SYS->GPB_MFPL &=~(SYS_GPB_MFPL_PB4MFP_SPI1_MOSI | SYS_GPB_MFPL_PB3MFP_SPI1_CLK)


extern void TMC4671_SPI_Init(void);
extern int32_t tmc4671_readInt(uint8_t motor, uint8_t address);
extern void tmc4671_writeInt(uint8_t motor, uint8_t address, int32_t value);
extern uint8_t tmc4671_readwriteByte(uint8_t motor, uint8_t data, uint8_t lastTransfer);
extern INT32 SPI_TransmitReceive_WithAddress(UINT8 *pTxData, UINT8 *pRxData, UINT16 Size, UINT32 Timeout);

#endif

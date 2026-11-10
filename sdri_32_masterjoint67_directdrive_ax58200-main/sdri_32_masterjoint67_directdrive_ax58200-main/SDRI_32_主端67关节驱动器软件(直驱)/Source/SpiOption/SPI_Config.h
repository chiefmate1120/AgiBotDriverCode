#ifndef SPI_CONFIG
#define SPI_CONFIG

#include "NuMicro.h"
#include "AX58200_HW.h"
/**
SPI0 :
CS      -       X
CLK     -       PA2
MISO    -       PA1
MOSI    -       PB12


SPI1 :
CS      -       PC0
CLK     -       PC1
MISO    -       PC3
MOSI    -       PC2


SPI3 :
CS      -       PE5
CLK     -       PE4
MISO    -       PE3
MOSI    -       PE2

*/

#define  SPI0_CS_H     HW_GPIO_WritePin(PB, BIT_13, 1);	      
#define  SPI0_CS_L     HW_GPIO_WritePin(PB, BIT_13, 0);	 

#define SPI0_CS_PORT                PB
#define SPI0_CS_PIN                 BIT_13
#define SPI0_CLK_PORT               PA
#define SPI0_CLK_PIN                BIT_2
#define SPI0_MISO_PORT              PA
#define SPI0_MISO_PIN               BIT_1
#define SPI0_MOSI_PORT              PB
#define SPI0_MOSI_PIN               BIT_12
#define SPI0_Select    \
do                     \
{                      \
    SYS->GPA_MFPL |= SYS_GPA_MFPL_PA2MFP_SPI0_CLK|SYS_GPA_MFPL_PA1MFP_SPI0_MISO; SYS->GPB_MFPH |= SYS_GPB_MFPH_PB12MFP_SPI0_MOSI;  PA->SMTEN |= BIT_2;  \
}while(0)


#define  SPI1_CS_H     HW_GPIO_WritePin(PC, BIT_0, 1);	      
#define  SPI1_CS_L     HW_GPIO_WritePin(PC, BIT_0, 0);	

#define SPI1_CS_PORT                PC
#define SPI1_CS_PIN                 BIT_0
#define SPI1_CLK_PORT               PC
#define SPI1_CLK_PIN                BIT_1
#define SPI1_MISO_PORT              PC
#define SPI1_MISO_PIN               BIT_3
#define SPI1_MOSI_PORT              PC
#define SPI1_MOSI_PIN               BIT_2
#define SPI1_Select    \
do                     \
{                      \
    SYS->GPC_MFPL |= SYS_GPC_MFPL_PC1MFP_SPI1_CLK|SYS_GPC_MFPL_PC2MFP_SPI1_MOSI|SYS_GPC_MFPL_PC3MFP_SPI1_MISO;  PC->SMTEN |= BIT_1;     \
}while(0)   




#define  SPI3_CS_H     HW_GPIO_WritePin(PE, BIT_5, 1);	      
#define  SPI3_CS_L     HW_GPIO_WritePin(PE, BIT_5, 0);

#define SPI3_CS_PORT                PE
#define SPI3_CS_PIN                 BIT_5
#define SPI3_CLK_PORT               PE
#define SPI3_CLK_PIN                BIT_4
#define SPI3_MISO_PORT              PE
#define SPI3_MISO_PIN               BIT_3
#define SPI3_MOSI_PORT              PE
#define SPI3_MOSI_PIN               BIT_2
#define  SPI3_Select   \
do                     \
{                      \
     SYS->GPE_MFPL |= SYS_GPE_MFPL_PE4MFP_SPI3_CLK|SYS_GPE_MFPL_PE2MFP_SPI3_MOSI|SYS_GPE_MFPL_PE3MFP_SPI3_MISO;  PE->SMTEN |= BIT_4;    \
}while(0)    





#define SPI_BAUDRATE    8000000
	


void SPIx_ConfigInit(SPI_T* Instance);
INT32 SPIx_WaitFlagStateUntilTimeout(SPI_T* Instance,UINT32 Flag, UINT32 State, UINT32 Timeout, UINT32 Tickstart);
INT32 SPIx_TransmitReceive(SPI_T* Instance,UINT8 *pTxData, UINT8 *pRxData, UINT16 Size, UINT32 Timeout);











#endif
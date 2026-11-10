#ifndef SPI_APPLICATION
#define SPI_APPLICATION

#include "NuMicro.h"
#include "AX58200_HW.h"
#include "SPI_Config.h"


typedef enum 
{
   SPI0_CS =0,
   SPI1_CS =1,  
   SPI3_CS =3,   
    
}SPIx_CS;

typedef enum 
{
   SPI_LOW =0,
   SPI_HIGH,      
}SPIx_CS_STATE;


void SPI_Init(void);
void STM_WriteRead(SPI_T* Instance, uint8_t cmd,int32_t value, UINT8 *pRxData);


#endif
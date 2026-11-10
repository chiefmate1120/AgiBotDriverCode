#ifndef _spi_encoder_h_
#define _spi_encoder_h_

#include "stm32g4xx_hal.h"

#define SPI1_CS_L HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,0)
#define SPI1_CS_H HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,1)

#define SPI2_CS_L HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,0)
#define SPI2_CS_H HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,1)

extern unsigned char EAngleSave;
extern short MA600Offsettemp;
extern short MA600Offset;

extern int AS5047P_Read(void);

extern void set_Mangle(void);

extern short enc_angle;
extern int MT6835_ANGLE(void);
extern short MT6835_EANGLE(void);

extern void MA600_Init(void);
extern short GetMA600EAngle(void);
extern void SetMA600EAngle90(void);
extern short MA600Offset,EangleOffset;
extern short GetMA600Angle(void);
extern void DataSave(void);

extern void M95640_WREN(void);
extern void M95640_WRDI(void);
extern void M95640_WRITE_BYTE(unsigned short addr,unsigned short data);
extern unsigned short M95640_READ_BYTE(unsigned short addr);
extern unsigned short M95640_STATUS(void);
extern void M95640_WRITE_DATA(unsigned short addr,unsigned short data);
extern unsigned short M95640_READ_BYTE(unsigned short addr);

#endif

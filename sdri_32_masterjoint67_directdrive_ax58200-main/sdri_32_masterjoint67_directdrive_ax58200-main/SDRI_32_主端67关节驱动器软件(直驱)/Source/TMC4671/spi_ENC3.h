#ifndef _spi_ENC3_h
#define _spi_ENC3_h

#define		ENC3_spi_cs_H	HW_GPIO_WritePin(PB, BIT_13, 1)
#define		ENC3_spi_cs_L	HW_GPIO_WritePin(PB, BIT_13, 0)


extern void ENC3_spi_init(void);
extern unsigned int ENC3_read(void);

#endif
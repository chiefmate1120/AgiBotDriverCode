#ifndef _spi_ENC2_h_
#define _spi_ENC2_h_
#include "AX58200_Hw.h"

extern void ENC2_spi_init(void);
extern void ENC_writeInt(uint8_t motor, uint8_t address, int32_t value);
extern int32_t ENC_readInt(uint8_t motor, uint8_t address);

#endif
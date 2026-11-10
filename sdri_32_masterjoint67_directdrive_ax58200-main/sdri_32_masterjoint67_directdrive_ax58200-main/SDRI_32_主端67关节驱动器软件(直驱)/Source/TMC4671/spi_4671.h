#ifndef _spi_4671_h_
#define _spi_4671_h_
#include "AX58200_Hw.h"
extern void TMC4671_SPI_Init(void);
extern int32_t tmc4671_readInt(uint8_t motor, uint8_t address);
extern void tmc4671_writeInt(uint8_t motor, uint8_t address, int32_t value);
extern uint8_t tmc4671_readwriteByte(uint8_t motor, uint8_t data, uint8_t lastTransfer);
extern void tmc4671_write(uint8_t address, int32_t value);
#endif

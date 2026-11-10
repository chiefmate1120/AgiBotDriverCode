#ifndef _STM_PROTOCOL_H
#define _STM_PROTOCOL_H

#include <stdint.h>

#include "SPI_Application.h"

#pragma pack(1)
typedef struct _tStmPrtocolRxBase
{
	uint8_t 	cmd;
	uint32_t  data;
}StmPrtocolBase_t;

typedef struct _tProtocol
{
	StmPrtocolBase_t tx_protocol;
	StmPrtocolBase_t rx_protocol;
}StmProtocol_t;
#pragma pack()

uint32_t U8ArrToU32(uint8_t *data);
uint32_t U8ArrToU16(uint8_t *data);
uint32_t RxVerifyCalculation(StmProtocol_t cmd_buff[], uint16_t cmd_size);
uint32_t TxVerifyCalculation(StmProtocol_t cmd_buff[], uint16_t cmd_size);
uint32_t RxVerifyCheck(StmProtocol_t cmd_buff[], uint16_t cmd_size);
void STM_SendMsg(StmProtocol_t protocol[], uint16_t size);


#endif
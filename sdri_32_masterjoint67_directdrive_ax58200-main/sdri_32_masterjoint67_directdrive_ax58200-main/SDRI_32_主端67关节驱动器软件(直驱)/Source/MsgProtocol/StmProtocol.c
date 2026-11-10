#include "StmProtocol.h"

uint32_t U8ArrToU32(uint8_t *data)
{
	return data[0]<<24 | data[1]<<16 | data[2]<<8 | data[3];
}

uint32_t U8ArrToU16(uint8_t *data)
{
	return data[0]<<8 | data[1]<<0;
}


uint32_t RxVerifyCalculation(StmProtocol_t cmd_buff[], uint16_t cmd_size)
{
	uint32_t sum;
	uint8_t *data;
	sum = 0;
	for(int i=0; i<cmd_size-1; i++)
	{
		data = (uint8_t *)&cmd_buff[i].rx_protocol.data;
		sum	+= cmd_buff[i].rx_protocol.cmd;
		sum	+= data[0];
		sum	+= data[1];
		sum	+= data[2];
		sum	+= data[3];
	}
	sum	+= cmd_buff[cmd_size-1].rx_protocol.cmd;
	
	return sum;
}


uint32_t TxVerifyCalculation(StmProtocol_t cmd_buff[], uint16_t cmd_size)
{
	uint32_t sum;
	uint8_t *data;
	sum = 0;
	for(int i=0; i<cmd_size-1; i++)
	{
		data = (uint8_t *)&cmd_buff[i].tx_protocol.data;
		sum	+= cmd_buff[i].tx_protocol.cmd;
		sum	+= data[0];
		sum	+= data[1];
		sum	+= data[2];
		sum	+= data[3];
	}
	sum	+= cmd_buff[cmd_size-1].tx_protocol.cmd;
	
	return sum;
}

uint32_t RxVerifyCheck(StmProtocol_t cmd_buff[], uint16_t cmd_size)
{
	uint32_t sum;
	uint32_t verity_data;
	
	sum	= RxVerifyCalculation(cmd_buff, cmd_size);
	verity_data = U8ArrToU32((uint8_t *)&cmd_buff[cmd_size-1].rx_protocol.data);
	if(sum == verity_data)
	{
		for(int i=0; i<cmd_size; i++)
		{
			cmd_buff[i].rx_protocol.data = U8ArrToU32((uint8_t *)&cmd_buff[i].rx_protocol.data);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}


void STM_SendMsg(StmProtocol_t protocol[], uint16_t size)
{
	for(int i=0; i<size; i++)
	{
			STM_WriteRead(SPI1, protocol[i].tx_protocol.cmd, protocol[i].tx_protocol.data, (uint8_t *)&protocol[i].rx_protocol);
	}
}




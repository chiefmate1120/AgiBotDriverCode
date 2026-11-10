#include "SEGGER_RTT_Output.h"


SEGGER_DATA_T g_tSeggerData = {0};
uint8_t SeggerBuff[2048] = {0};

void SEGGER_RTT_OuputInit(void)
{
	SEGGER_RTT_ConfigUpBuffer(1, "JScope_t4i2i2i2", SeggerBuff, sizeof(SeggerBuff), SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}

void SEGGER_RTT_Output(void)
{
	SEGGER_RTT_Write(1,&g_tSeggerData, sizeof(SEGGER_DATA_T));
}



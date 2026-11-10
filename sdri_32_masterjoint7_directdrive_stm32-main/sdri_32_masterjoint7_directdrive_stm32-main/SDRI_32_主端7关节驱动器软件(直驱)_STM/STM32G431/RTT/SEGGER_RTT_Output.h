#ifndef _SEGGER_DATA_RTT_H
#define _SEGGER_DATA_RTT_H

#include <stdint.h>
#include "SEGGER_RTT.h"

#pragma pack(1)

typedef struct 
{
	uint32_t 	UsCount;
	int16_t 	Data0;
	int16_t 	Data1;
	int16_t 	Data2;
}SEGGER_DATA_T;

#pragma pack()


void SEGGER_RTT_OuputInit(void);
void SEGGER_RTT_Output(void);

extern SEGGER_DATA_T g_tSeggerData;

#endif

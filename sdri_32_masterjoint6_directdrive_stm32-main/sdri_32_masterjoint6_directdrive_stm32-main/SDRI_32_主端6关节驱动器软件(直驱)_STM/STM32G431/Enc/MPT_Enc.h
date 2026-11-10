#ifndef __MPT_ENC_H
#define __MPT_ENC_H

#include <stdint.h>
#include "General_Enc.h"



int Mpt_PositionConversion(uint8_t data[], uint8_t size, uint32_t *position);
void Mpt_SetZeroPosition(void);

typedef struct _MPT_Encoder_t
{
	GeneralEnc_t Parent;
	uint8_t ClearCount;
	uint32_t InteriorAngle;
	uint8_t States;
	uint16_t Temperature;
}MPT_Encoder_t;

extern MPT_Encoder_t Mpt_Encoder;


#endif

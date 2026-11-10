#ifndef __DPT_ENC_H
#define __DPT_ENC_H

#include <stdint.h>
#include "General_Enc.h"

typedef  enum
{
	DPT_CMD_SET_INTERNAL_ZERO = 0,
	DPT_CMD_SET_EXTERNAL_ZERO,
	DPT_CMD_GET_INTERNAL_ANGLE,
	DPT_CMD_GET_EXTERNAL_ANGLE,
	DPT_CMD_GET_INTERNAL_EXTERNAL_ANGLE,
	DPT_CMD_GET_INTERNAL_STATE_ANGLE,
	DPT_CMD_GET_EXTERNAL_STATE_ANGLE,
	DPT_CMD_GET_INTERNAL_EXTERNAL_STATE_ANGLE,
	DPT_CMD_GET_TEMPERATUER,
}Dpt_Cmd_e;

typedef struct _DPT_Encoder_t
{
	GeneralEnc_t Parent;
	uint8_t ClearFlag;
	uint8_t ClearCount;
	uint32_t InteriorAngle;
	uint32_t ExternalAngle;
	uint8_t States;
	uint16_t Temperature;
}DPT_Encoder_t;

extern DPT_Encoder_t Dpt_Encoder;

#endif

#include "stm32g4xx_hal.h"
#include "LinearRamp.h"
#include "motor_control.h"

#define motor 0

unsigned char RampTemp;

int32_t tmc_limitInt(int32_t value, int32_t min, int32_t max)
{
	if (value > max)
		return max;
	else if (value < min)
		return min;
	else
		return value;
}

int64_t tmc_limitS64(int64_t value, int64_t min, int64_t max)
{
	if (value > max)
		return max;
	else if (value < min)
		return min;
	else
		return value;
}

/* lookup table for square root function */
static const unsigned char sqrttable[256] =
{
	0,   16,  22,  27,  32,  35,  39,  42,  45,  48,  50,  53,  55,  57,  59,  61,
	64,  65,  67,  69,  71,  73,  75,  76,  78,  80,  81,  83,  84,  86,  87,  89,
	90,  91,  93,  94,  96,  97,  98,  99,  101, 102, 103, 104, 106, 107, 108, 109,
	110, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
	128, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
	143, 144, 144, 145, 146, 147, 148, 149, 150, 150, 151, 152, 153, 154, 155, 155,
	156, 157, 158, 159, 160, 160, 161, 162, 163, 163, 164, 165, 166, 167, 167, 168,
	169, 170, 170, 171, 172, 173, 173, 174, 175, 176, 176, 177, 178, 178, 179, 180,
	181, 181, 182, 183, 183, 184, 185, 185, 186, 187, 187, 188, 189, 189, 190, 191,
	192, 192, 193, 193, 194, 195, 195, 196, 197, 197, 198, 199, 199, 200, 201, 201,
	202, 203, 203, 204, 204, 205, 206, 206, 207, 208, 208, 209, 209, 210, 211, 211,
	212, 212, 213, 214, 214, 215, 215, 216, 217, 217, 218, 218, 219, 219, 220, 221,
	221, 222, 222, 223, 224, 224, 225, 225, 226, 226, 227, 227, 228, 229, 229, 230,
	230, 231, 231, 232, 232, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238,
	239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247,
	247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255
};

int32_t tmc_sqrti(int32_t x)
{
	int32_t xn;

	// Negative parameter?
	if (x < 0)
		return -1;

	if (x < 0x0100)
		return (int) sqrttable[x] >> 4;

	if (x >= 0x00010000)
	{
		if (x >= 0x01000000)
		{
			if (x >= 0x10000000)
			{
				if (x >= 0x40000000)
				{
					// 0x40000000 <= x < 0x7FFFFFFF
					xn = (int) sqrttable[x >> 24] << 8;
				}
				else
				{
					// 0x10000000 <= x < 0x40000000
					xn = (int) sqrttable[x >> 22] << 7;
				}
			}
			else
			{
				if (x >= 0x04000000)
				{
					// 0x04000000 <= x < 0x10000000
					xn = (int) sqrttable[x >> 20] << 6;
				}
				else
				{
					// 0x01000000 <= x < 0x04000000
					xn = (int) sqrttable[x >> 18] << 5;
				}
			}

			// Two steps of the babylonian method
			xn = (xn + 1 + (x / xn)) >> 1;
			xn = (xn + 1 + (x / xn)) >> 1;
		}
		else
		{
			if (x >= 0x00100000)
			{
				if (x >= 0x00400000)
				{
					// 0x00400000 <= x < 0x01000000
					xn = (int) sqrttable[x >> 16] << 4;
				}
				else
				{
					// 0x00100000 <= x < 0x00400000
					xn = (int) sqrttable[x >> 14] << 3;
				}
			}
			else
			{
				if (x >= 0x00040000)
				{
					// 0x00040000 <= x < 0x00100000
					xn = (int) sqrttable[x >> 12] << 2;
				}
				else
				{
					// 0x00010000 <= x < 0x00040000
					xn = (int) sqrttable[x >> 10] << 1;
				}
			}

			// One step of the babylonian method
			xn = (xn + 1 + (x / xn)) >> 1;
		}
	}
	else
	{
		if (x >= 0x1000)
		{
			if (x >= 0x4000)
			{
				// 0x4000 <= x < 0x00010000
				xn = (int) (sqrttable[x >> 8] ) + 1;
			}
			else
			{
				// 0x1000 <= x < 0x4000
				xn = (int) (sqrttable[x >> 6] >> 1) + 1;
			}
		}
		else
		{
			if (x >= 0x0400)
			{
				// 0x0400 <= x < 0x1000
				xn = (int) (sqrttable[x >> 4] >> 2) + 1;
			}
			else
			{
				// 0x0100 <= x < 0x0400
				xn = (int) (sqrttable[x >> 2] >> 3) + 1;
			}
		}
	}

	// Make sure that our result is floored
	if ((xn * xn) > x)
		xn--;

	return xn;
}

int32_t tmc_filterPT1(int64_t *akku, int32_t newValue, int32_t lastValue, uint8_t actualFilter, uint8_t maxFilter)
{
	*akku += (newValue-lastValue) << (maxFilter-actualFilter);
	return *akku >> maxFilter;
}

int64_t abs1(int64_t input)
{
	if(input>0)
	{
		return input;
	}
	else input*=-1;
	return input;
}

typedef struct
{
	uint32_t maxVelocity;
	int32_t targetPosition;
	int32_t rampPosition;
	int32_t targetVelocity;
	int32_t rampVelocity;
	int32_t acceleration;
	uint16_t encoderSteps;
	int32_t lastdVRest;
	int32_t lastdXRest;
	uint8_t rampEnabled;
} TMC_LinearRamp;



void tmc_linearRamp_init(TMC_LinearRamp *linearRamp)
{
	linearRamp->maxVelocity     = 500;
	linearRamp->targetPosition  = 0;
	linearRamp->targetVelocity  = 0;
	linearRamp->rampVelocity    = 0;
	linearRamp->rampPosition    = 0;
	linearRamp->acceleration    = 10000;
	linearRamp->encoderSteps	= 32767;
	linearRamp->lastdVRest      = 0;
	linearRamp->lastdXRest      = 0;
	linearRamp->rampEnabled     = 1;
}
void tmc_linearRamp_computeRampPosition(TMC_LinearRamp *linearRamp)
{
//	if (linearRamp->rampEnabled)
	{
		// update target position according actual set acceleration and max velocity
		// (scaling pre-factor of 1000 used for 1ms position ramp handling)

		// limit position difference for further computations
		int32_t targetPositionsDifference = linearRamp->targetPosition-linearRamp->rampPosition;

		// limit the sqrti value in case of high position differences
		int64_t sqrtiValue = tmc_limitS64(((int64_t)120 * (int64_t)linearRamp->acceleration * (int64_t)(abs1(targetPositionsDifference))) / (int64_t)linearRamp->encoderSteps, 0, (int64_t)linearRamp->maxVelocity*(int64_t)linearRamp->maxVelocity);

		// compute max allowed ramp velocity to ramp down to target
		int32_t maxRampStop = tmc_sqrti(sqrtiValue);

		// compute max allowed ramp velocity
		int32_t maxRampTargetVelocity = 0;
		if (targetPositionsDifference > 0)
		{
			maxRampTargetVelocity = tmc_limitInt(maxRampStop, 0, (int32_t)linearRamp->maxVelocity);
		}
		else if (targetPositionsDifference < 0)
		{
			maxRampTargetVelocity = tmc_limitInt(-maxRampStop, -(int32_t)linearRamp->maxVelocity, 0);
		}
		else
		{
			//maxRampTargetVelocity = 0;
		}

		int dV = linearRamp->acceleration;  // pre-factor ~ 1/1000

		// to ensure that small velocity changes at high set acceleration are also possible
		int maxDTV = abs1(maxRampTargetVelocity - linearRamp->rampVelocity);
		if (maxDTV < (dV / 1000))
			dV = maxDTV * 1000;

		dV += linearRamp->lastdVRest;
		linearRamp->lastdVRest = dV % 1000;

		// do velocity ramping
		if (maxRampTargetVelocity > linearRamp->rampVelocity)
		{
			linearRamp->rampVelocity += dV / 1000;
		}
		else if (maxRampTargetVelocity < linearRamp->rampVelocity)
		{
			linearRamp->rampVelocity -= dV / 1000;
		}

		// limit positionRampTargetVelocity to maxRampTargetVelocity
		linearRamp->rampVelocity = tmc_limitInt(linearRamp->rampVelocity, -abs1(maxRampTargetVelocity), abs1(maxRampTargetVelocity));

		// do position ramping using actual ramp velocity to update dX
		int64_t dX = ((int64_t)linearRamp->rampVelocity * (int64_t)linearRamp->encoderSteps) / ((int64_t)60) + linearRamp->lastdXRest;

		// scale actual target position
		int64_t tempActualTargetPosition = (int64_t)linearRamp->rampPosition * 1000;

		// update actual target position
		tempActualTargetPosition += dX;

		if (tempActualTargetPosition >= 0)
			linearRamp->lastdXRest = (abs1(tempActualTargetPosition) % 1000);
		else if (tempActualTargetPosition < 0)
			linearRamp->lastdXRest = -(abs1(tempActualTargetPosition) % 1000);

		// scale actual target position back
		linearRamp->rampPosition = tempActualTargetPosition / 1000;
	}
//	else
//	{
//		// use target position directly
//		linearRamp->rampPosition = linearRamp->targetPosition;
//	}
}


TMC_LinearRamp rampGenerator[1];
uint8_t actualMotionMode[1];
int32_t lastRampTargetPosition[1];
int32_t lastRampTargetVelocity[1];



unsigned char PositionRamp;

void Task_ProfilePosition(void)
{
//	if(PositionMode)
	{
		if(!RampTemp)
		{
			RampTemp=1;
			linearRamp_init();
			rampGenerator[0].lastdXRest=ActualPosition;
			rampGenerator[0].rampPosition=ActualPosition;	
		}
		rampGenerator[0].targetPosition=TargetPosition;
		tmc_linearRamp_computeRampPosition(&rampGenerator[0]);
//		// set new target position (only if changed)
//		if (rampGenerator[0].rampPosition != lastRampTargetPosition[0])
//		{
//			TargetRampPosition=rampGenerator[0].rampPosition;
//			lastRampTargetPosition[0] = rampGenerator[0].rampPosition;
//		}
		TargetRampPosition=rampGenerator[0].rampPosition;
		PositionRamp++;
	}
//	else
	{
		RampTemp=0;
	}
}

void linearRamp_init(void)
{
	tmc_linearRamp_init(&rampGenerator[0]);
}



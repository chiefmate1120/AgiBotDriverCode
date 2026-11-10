#ifndef __AX58200_PROTOCOL_H
#define __AX58200_PROTOCOL_H

#include <stdint.h>
#include <string.h>

#define LittletoBig32(A) ((( (uint32_t)(A) &0xff000000)>>24) | (( (uint32_t)(A) &0x00ff0000) >>8) |  (( (uint32_t)(A) &0x0000ff00) <<8) | (( (uint32_t)(A) &0x000000ff) <<24))



#define TARGET_POSITION	0x02
#define TARGET_TORQUE	0x04
#define TARGET_VELOCITY 0x03
#define MOTOR_MODE	    0x01
#define TARGET_HALL	    0x05
#define CMD_VERITY		0x06


#define PID_FLUX	0x54
#define PID_TORQUE	0x56
#define PID_VELOCITY 0x58	
#define PID_POSITION 0x5A


#define ACTUAL_POSITION	0x22
#define ACTUAL_TORQUE	0x24
#define ACTUAL_VELOCITY	0x23
#define ACTUAL_STATE	0x21
#define ACTUAL_HALL	    0x25
#define ACTUAL_VERIFY	0x26

#define PROTOCO_SIZE	(30)


#pragma pack(1)
typedef struct _tAx52800BaseProtocol
{
	uint8_t CmdType;
	uint32_t Data;
}Ax52800BaseProtocol_t;

typedef struct _tAx52800Protocol
{
	Ax52800BaseProtocol_t BaseProtocol[6];
}Ax52800Protocol_t;
#pragma pack()


void Ax58200_MsgHandle(void);
void Ax58200_MsgSend(void);

#endif

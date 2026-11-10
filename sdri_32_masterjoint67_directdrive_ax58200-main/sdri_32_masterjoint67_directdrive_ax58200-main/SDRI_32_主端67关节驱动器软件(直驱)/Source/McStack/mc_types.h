/*
 ******************************************************************************
 *     Copyright (c) 2020	ASIX Electronic Corporation      All rights reserved.
 *
 *     This is unpublished proprietary source code of ASIX Electronic Corporation
 *
 *     The copyright notice above does not evidence any actual or intended
 *     publication of such source code.
 ******************************************************************************
 */
#ifndef __MC_TYPES_H__
#define __MC_TYPES_H__

/* INCLUDE FILE DECLARATIONS */
#include "NuMicro.h"

/* NAMING CONSTANT DECLARATIONS */
#define MCSTS_OK              0ul
#define MCSTS_ERR            -1ul

/* TYPE DECLARATIONS */
typedef enum
{
	MCFS_NO_ERR     = 0x0000,
  MCFS_OVER_VOLT  = 0x0001,
	MCFS_UNDER_VOLT = 0x0002,
	MCFS_OVER_TEMP  = 0x0004,
	MCFS_START_UP   = 0x0008,
	MCFS_SPEED_FBK  = 0x0010,
	MCFS_BREAK_IN   = 0x0020,
	MCFS_SW_ERR     = 0x0040,
} MC_FAULT_E;

typedef enum
{
	MC_STOP = 0,
  MC_START,
} MC_STSP_E;

typedef enum
{
	MC_DISABLE = 0,
  MC_ENABLE,	
} MC_EADA_E;

typedef enum
{
	MC_FORWARD = 0,
  MC_BACKWARD,
} MC_DIR_E;

/* MACRO DECLARATIONS */
#define MC_SWAP16(w)  (uint16_t)(((w & 0x00FF) << 8) | ((w & 0xFF00) >> 8))
#define MC_SWAP32(dw) (uint32_t)(((dw & 0x000000FF) << 24) | ((dw & 0xFF000000) >> 24) | ((dw & 0x0000FF00) << 8) | ((dw & 0x00FF0000) >> 8))

/* GLOBAL VARIABLES */

/* EXPORTED SUBPROGRAM SPECIFICATIONS */


#endif /* __MC_TYPES_H__ */

/* End of mc_types.h */

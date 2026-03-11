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

/**
 * \addtogroup AX58200 FoeAppl header file
 * @{
 */

/**
\file AX58200_FoeAppl.h
\brief AX58200_FoeAppl function prototypes and defines

\version 1.0.0.0
 */

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#ifndef _AX58200_FOE_APPL_H_
#define _AX58200_FOE_APPL_H_

#include "ecat_def.h"
#include <stdlib.h>
#include <string.h>
#include "esc.h"
#include "ecatslv.h"
#include "objdef.h"
#include "ecatappl.h"




/*ECATCHANGE_START(V5.12) FOE1*/
#include "foeappl.h"
/*ECATCHANGE_END(V5.12) FOE1*/

#endif /*#ifndef _AX58200_FOE_APPL_H_*/

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/
#ifndef MAX_FILE_SIZE
#define MAX_FILE_SIZE                             0x180
#endif

/*-----------------------------------------------------------------------------------------
------
------    Global Variables
------
-----------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------
------    Global Functions
------
-----------------------------------------------------------------------------------------*/
extern UINT16 AX58200_FoeRead(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password, UINT16 maxBlockSize, UINT16 *pData);
extern UINT16 AX58200_FoeReadData(UINT32 offset, UINT16 maxBlockSize, UINT16 *pData);
extern void AX58200_FoeError(UINT32 errorCode);
extern UINT16 AX58200_FoeWrite(UINT16 MBXMEM * pName, UINT16 nameSize, UINT32 password);
extern UINT16 AX58200_FoeWriteData(UINT16 MBXMEM * pData, UINT16 Size, BOOL bDataFollowing);
extern void AX58200_FoeInit(void);

/** @}*/

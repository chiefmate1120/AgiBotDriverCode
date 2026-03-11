/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
\addtogroup ESM EtherCAT State Machine
@{
*/

/**
\file bootmode.c
\author EthercatSSC@beckhoff.com
\brief Implementation

\version 5.12.1

<br>Changes to version V4.20:<br>
V5.12 BOOT2: call BL_Start() from Init to Boot<br>
<br>Changes to version - :<br>
V4.20: File created
*/

/*--------------------------------------------------------------------------------------
------
------    Includes
------
--------------------------------------------------------------------------------------*/
#include "ecat_def.h"
#include "ecatslv.h"
#define _BOOTMODE_ 1
#include "bootmode.h"
#undef _BOOTMODE_
#include "foeappl.h"
#include "ecatfoe.h"

#define FIRMWARE_UPDATE_BASE_ADDR		0x40000
#define FIRMWARE_UPDATE_MAX_PAGE_NUMBER	64
#define FIRMWARE_UPDATE_MAX_FILE_SIZE	0x40000
#define SIGNATURE_STR					"ASIX"
#define FIRMWARE_UPDATE_ENABLE			1
#define H2NL(x) ((x >> 24 & 0x000000FF) | (x >> 8 & 0x0000FF00) | ( x << 24 & 0xFF000000) | (x << 8 & 0x00FF0000))
#define TIMEOUT_FOR_JUMP_TO_LDROM		1000 //ms

typedef struct _UPGRADE_RUNTIME_HEADER
{
	unsigned char	Signature[4];
	unsigned short	ImageType;
	unsigned short	Compress;
	unsigned long	ContentOffset;
	unsigned long	FileLen;
	unsigned long	Reserved0;
	unsigned long	Checksum32;	
	unsigned long	Reserved1[2];
} UPGRADE_RT_HEAD;

typedef struct _UPGRADE_FIRMWARE
{
	unsigned char	State;
	unsigned long	FileOffset;
	unsigned long	FileLength;
	unsigned int	NumberPageErase;
	unsigned int 	PageBeErased;
} UPGRADE_FIRMWARE;

UPGRADE_FIRMWARE	firmwareObj;
UPGRADE_RT_HEAD		firmwareHeader;
UINT16				timerForJumpToLdrom;

static UINT32 FirmwareChecksum32(UINT32 length);

/*ECATCHANGE_START(V5.12) BOOT2*/
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    State        Current state

 \brief Called from INIT to BOOT
*////////////////////////////////////////////////////////////////////////////////////////
void BL_Start( UINT8 State)
{
	if (State == STATE_BOOT)
	{
		firmwareObj.State = BL_STATE_START;
	}
	else if (State == 0)//Polling
	{
	}
}
/*ECATCHANGE_END(V5.12) BOOT2*/

void BL_Init(void)
{
	firmwareObj.State = BL_STATE_IDLE;
	firmwareObj.FileOffset = 0;
	firmwareObj.FileLength = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

\brief Called in the state transition from BOOT to Init
*////////////////////////////////////////////////////////////////////////////////////////
void BL_Stop(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    password    download password

 \brief Dummy BL_StartDownload function
*////////////////////////////////////////////////////////////////////////////////////////
void BL_StartDownload(UINT32 password0)
{
	if (firmwareObj.State == BL_STATE_START)
	{
		firmwareObj.State = BL_STATE_START_DOWNLOAD;
		firmwareObj.FileOffset = 0;
		firmwareObj.FileLength = 0;
		SYS_UnlockReg();		/* Unlock register lock protect */
		FMC_Open();				/* Enable FMC ISP function */
		FMC_ENABLE_AP_UPDATE();	/* Enable FMC erase/program APROM */
		SYS_LockReg();			/* Lock protected registers */
	}
	printf("Start download firmware...\r\n");
}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pData    Data pointer
 \param    Size    Data Length


 \return    FoE error code

 \brief Dummy BL_Data function
*////////////////////////////////////////////////////////////////////////////////////////
unsigned char	IBOT_Signature[4];
UINT16 BL_Data(UINT16 *pData,UINT16 Size)
{
	unsigned long	dwordData, dataRead32;
	int				updateLen = 0;
	UINT16			writeSize = Size;
	unsigned char	*pSrcData = (unsigned char *)pData;
	
	if (firmwareObj.State < BL_STATE_START_DOWNLOAD)
	{
		return ECAT_FOE_ERRCODE_ACCESS;
	}
	else if (firmwareObj.State == BL_STATE_START_DOWNLOAD)
	{
		if (firmwareObj.FileOffset == 0)
		{
			memcpy((unsigned char *)&firmwareHeader, pSrcData, sizeof(UPGRADE_RT_HEAD));
//			printf("Signature %c%c%c%c\r\n", firmwareHeader.Signature[0],
//											 firmwareHeader.Signature[1],
//											 firmwareHeader.Signature[2],
//											 firmwareHeader.Signature[3]);
//			printf("Size %d\r\n", Size);
			/* Check the signature */
			if ((firmwareHeader.Signature[0] != 'A') | (firmwareHeader.Signature[1] != 'S') |
				(firmwareHeader.Signature[2] != 'I') | (firmwareHeader.Signature[3] != 'X'))
			{
				SYS_UnlockReg();
				FMC_Close();	/* Disable FMC ISP function */
				SYS_LockReg();
				return ECAT_FOE_ERRCODE_ILLEGAL;
			}
            
            IBOT_Signature[0] = (firmwareHeader.Reserved1[0]>>0) &0XFF;
            IBOT_Signature[1] = (firmwareHeader.Reserved1[0]>>8) &0XFF;
            IBOT_Signature[2] = (firmwareHeader.Reserved1[0]>>16) &0XFF;
            IBOT_Signature[3] = (firmwareHeader.Reserved1[0]>>24) &0XFF;            
			if ((IBOT_Signature[0]!= 'I') | (IBOT_Signature[1] != 'B') | 
				(IBOT_Signature[2] != 'O') | (IBOT_Signature[3] != 'T'))
			{
				SYS_UnlockReg();
				FMC_Close();	/* Disable FMC ISP function */
				SYS_LockReg();
				return ECAT_FOE_ERRCODE_ILLEGAL;
			}

			/* Check the file size */
			firmwareObj.FileLength = H2NL(firmwareHeader.FileLen);
//			printf("File length %x\r\n", firmwareObj.FileLength);
			if (firmwareObj.FileLength > FIRMWARE_UPDATE_MAX_FILE_SIZE)
			{
				SYS_UnlockReg();
				FMC_Close();	/* Disable FMC ISP function */
				SYS_LockReg();
				return ECAT_FOE_ERRCODE_ILLEGAL;
			}
			
			firmwareObj.NumberPageErase = firmwareObj.FileLength/4096;
			if (firmwareObj.FileLength % 4096)
			{
				firmwareObj.NumberPageErase += 1;
			}
//			printf("Page to be Erased %d\r\n", firmwareObj.NumberPageErase);
			firmwareObj.PageBeErased = 0;
		}
		firmwareObj.State = BL_STATE_DATA;
	}
	
	/* Erase the flash for firmware update */
	if (firmwareObj.NumberPageErase >  0)
	{
		SYS_UnlockReg();
		if (FMC_Erase(FIRMWARE_UPDATE_BASE_ADDR+(firmwareObj.PageBeErased*4096)) < 0)
		{
				FMC_Erase(FIRMWARE_UPDATE_BASE_ADDR);
				FMC_Close();                       /* Disable FMC ISP function */
				SYS_LockReg();
				return ECAT_FOE_ERRCODE_DISKFULL;
		}
		SYS_LockReg();
		firmwareObj.NumberPageErase--;
		firmwareObj.PageBeErased++;
//		printf("Erased %d\r\n", firmwareObj.PageBeErased);
	}
	
	if ((firmwareObj.FileOffset+Size) > firmwareObj.FileLength)
	{
			SYS_UnlockReg();
			FMC_Erase(FIRMWARE_UPDATE_BASE_ADDR);	/* Erase the signature page */
			FMC_Close();							/* Disable FMC ISP function */
			SYS_LockReg();
			return ECAT_FOE_ERRCODE_DISKFULL;
	}
	
	SYS_UnlockReg();
	writeSize = Size;
	while (writeSize > 0)
	{
		if (writeSize >= 4)
		{
			dwordData = *(unsigned long *)pSrcData;
			updateLen = 4;
       }
		else
		{
			dwordData = 0xFFFFFFFF;
			updateLen = writeSize;
			memcpy((unsigned char *)&dwordData, pSrcData, updateLen);
//			printf("dwordData: writeSize %d: %lx\r\n", writeSize, dwordData);
		}
		
		/* Write data to flash, FMC support 4 bytes write */
//		printf("FMC_Write: offset %x: %x\r\n", FIRMWARE_UPDATE_BASE_ADDR+firmwareObj.FileOffset, dwordData);
		FMC_Write(FIRMWARE_UPDATE_BASE_ADDR+firmwareObj.FileOffset, dwordData);
		dataRead32 = FMC_Read(FIRMWARE_UPDATE_BASE_ADDR+firmwareObj.FileOffset);
		if (dataRead32 != dwordData)
		{
			printf("Wrtie %8lx, Readback: %8lx\r\n", dwordData, dataRead32);
			FMC_Erase(FIRMWARE_UPDATE_BASE_ADDR);	/* Erase the signature page */
			FMC_Close();							/* Disable FMC ISP function */
			SYS_LockReg();
			return ECAT_FOE_ERRCODE_ILLEGAL;
		}
		firmwareObj.FileOffset += updateLen;
		writeSize -= updateLen;
		pSrcData += updateLen;
	}
	SYS_LockReg();

	if (firmwareObj.FileOffset >= firmwareObj.FileLength)
	{
		unsigned long checksum;
		checksum = FirmwareChecksum32(firmwareObj.FileLength-sizeof(UPGRADE_RT_HEAD));
//		printf("Compute checksum %8lx Header checksum %8lx\r\n", checksum, firmwareHeader.Checksum32);
		printf("Received completely, ");
		if (checksum != ~firmwareHeader.Checksum32)
		{
			printf("but CRC error!!!\r\n\r\n");
			SYS_UnlockReg();
			FMC_Erase(FIRMWARE_UPDATE_BASE_ADDR);	/* Erase the signature page */
			FMC_Close();							/* Disable FMC ISP function */
			SYS_LockReg();
			return ECAT_FOE_ERRCODE_ILLEGAL;
		}
		printf("jump to LDROM after 1 second.\r\n");
		printf("LDROM will move received firmware to APROM later.\r\n\r\n");
		timerForJumpToLdrom = HW_GetTimer();
		firmwareObj.State = BL_STATE_FW_TRANSMIT_DONE;
	}

	return 0;
}

static UINT32 FirmwareChecksum32(UINT32 length)
{
	UINT32 i;
	UINT32 count = (length >> 2);
	UINT32 cksum = 0, data32;
	UINT32 firmwareAddr = FIRMWARE_UPDATE_BASE_ADDR+sizeof(UPGRADE_RT_HEAD);
	
	SYS_UnlockReg();
	for (i = 0; i < count; i++)
	{
		data32 = FMC_Read(firmwareAddr);
		
		cksum += data32;
		firmwareAddr += 4;
		if (cksum < data32)
		{
			cksum++;
		}
	}
	
	if (length % 4)
	{
		data32 = FMC_Read(firmwareAddr);
//		printf("last long word %8lx\r\n", data32);
		count = length % 4;
		if (count == 3)
		{
			data32 &= 0x00FFFFFF;
		}
		else if (count == 2)
		{
			data32 &= 0x0000FFFF;
		}
		else
		{
			data32 &= 0x000000FF;
		}
//		printf("convert long word %8lx\r\n", data32);
		cksum += data32;
		if (cksum < data32)
		{
			cksum++;
		}
	}
	SYS_LockReg();
	
	return cksum;
} /* End of FirmwareChecksum32() */

void BL_JumpToLdrom(void)
{
	UINT16	tmp16;
	
	if (firmwareObj.State == BL_STATE_FW_TRANSMIT_DONE)
	{
		tmp16 = HW_GetTimer();
		if (tmp16 >= timerForJumpToLdrom)
		{
			if ((tmp16 - timerForJumpToLdrom) >= TIMEOUT_FOR_JUMP_TO_LDROM)
			{
				firmwareObj.State = BL_STATE_IDLE;
				SYS_UnlockReg();                //解锁
				FMC_Open();                     //开启FMC
				FMC_SetBootSource(1);           //选择启动源 0：从APROM启动  1：从LDROM启动
				FMC_Close();                    //关闭FMC
				SYS_LockReg();                  //锁存
				NVIC_SystemReset();             //复位
			}
		}
		else
		{
			timerForJumpToLdrom = tmp16;
		}
	}
}

/** @} */

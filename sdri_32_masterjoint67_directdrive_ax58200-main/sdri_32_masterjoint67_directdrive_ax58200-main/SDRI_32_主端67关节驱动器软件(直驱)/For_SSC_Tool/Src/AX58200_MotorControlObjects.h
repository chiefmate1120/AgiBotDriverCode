/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
* \addtogroup AX58200_MotorControl AX58200_MotorControl
* @{
*/

/**
\file AX58200_MotorControlObjects
\author ET9300Utilities.ApplicationHandler (Version 1.5.0.0) | EthercatSSC@beckhoff.com

\brief AX58200_MotorControl specific objects<br>
\brief NOTE : This file will be overwritten if a new object dictionary is generated!<br>
*/

#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
#define PROTO
#else
#define PROTO extern
#endif
/******************************************************************************
*                    Object 0x1600 : RxPdoMappingCspCsv
******************************************************************************/
/**
* \addtogroup 0x1600 0x1600 | RxPdoMappingCspCsv
* @{
* \brief Object 0x1600 (RxPdoMappingCspCsv) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - map control word<br>
* SubIndex 2<br>
* SubIndex 3 - map taget position<br>
* SubIndex 4 - map target velocity<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1600[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - map control word */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 - map taget position */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 - map target velocity */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex8 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1600[] = "RxPdoMappingCspCsv\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - map control word */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 - map taget position */
UINT32 SI4; /* Subindex4 - map target velocity */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1600;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1600 RxPdoMappingCspCsv0x1600
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={8,0x60400010,0x607A0020,0x60FF0020,0x60710020,0x68BA0020,0x68BB0020,0x60600008,0x00000018}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1601 : RxPdoMappingCsp
******************************************************************************/
/**
* \addtogroup 0x1601 0x1601 | RxPdoMappingCsp
* @{
* \brief Object 0x1601 (RxPdoMappingCsp) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - map control word<br>
* SubIndex 2<br>
* SubIndex 3 - map taget position<br>
* SubIndex 4 - map target velocity<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1601[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - map control word */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 - map taget position */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 - map target velocity */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex13 */
/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1601[] = "RxPdoMappingCsp\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000"
"SubIndex 011\000"
"SubIndex 012\000"
"SubIndex 013\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - map control word */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 - map taget position */
UINT32 SI4; /* Subindex4 - map target velocity */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */ 
UINT32 SI13; /* Subindex13 -  */    
} OBJ_STRUCT_PACKED_END
TOBJ1601;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1601 RxPdoMappingCsp0x1601
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={13,0x60400010,0x607A0020,0x60FF0020,0x60710020,0x68400010,0x687A0020,0x68FF0020,0x68710020,0x68BA0020,0x68BB0020,0x60600008,0x68600008,0x00000010}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1602 : RxPdoMappingCsv
******************************************************************************/
/**
* \addtogroup 0x1602 0x1602 | RxPdoMappingCsv
* @{
* \brief Object 0x1602 (RxPdoMappingCsv) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - map control word<br>
* SubIndex 2<br>
* SubIndex 3 - map taget position<br>
* SubIndex 4 - map target velocity<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1602[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - map control word */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 - map taget position */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 - map target velocity */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex22 */
/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1602[] = "RxPdoMappingCsv\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000"
"SubIndex 011\000"
"SubIndex 012\000"
"SubIndex 013\000"
"SubIndex 014\000"
"SubIndex 015\000"
"SubIndex 016\000"
"SubIndex 017\000"
"SubIndex 018\000"
"SubIndex 019\000"
"SubIndex 020\000"
"SubIndex 021\000"
"SubIndex 022\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - map control word */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 - map taget position */
UINT32 SI4; /* Subindex4 - map target velocity */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */   
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */     
} OBJ_STRUCT_PACKED_END
TOBJ1602;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1602 RxPdoMappingCsv0x1602
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={22,0x60400010,0x607A0020,0x60FF0020,0x60710020,0x687A0020,0x68FF0020,0x68710020,0x707A0020,0x70FF0020,0x70710020,0x68400010,0x787A0020,0x78FF0020,0x78710020,0x807A0020,0x80FF0020,0x80710020,0x68BA0020,0x68BB0020,0x60600008,0x68600008,0x00000010}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A00 : TxPdoMappingCspCsv
******************************************************************************/
/**
* \addtogroup 0x1A00 0x1A00 | TxPdoMappingCspCsv
* @{
* \brief Object 0x1A00 (TxPdoMappingCspCsv) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - map status word<br>
* SubIndex 2 - map actual position<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5 - map actual velocity<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A00[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - map status word */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 - map actual position */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 - map actual velocity */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex13 */
/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A00[] = "TxPdoMappingCspCsv\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000"
"SubIndex 011\000"
"SubIndex 012\000"
"SubIndex 013\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - map status word */
UINT32 SI2; /* Subindex2 - map actual position */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 - map actual velocity */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A00;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A00 TxPdoMappingCspCsv0x1A00
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={13,0x60410010,0x60640020,0x606C0020,0x60770020,0x607F0020,0x687F0020,0x68910020,0x68920020,0x68930020,0x68AA0020,0x68AB0020,0x60610008,0x00000018}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A01 : TxPdoMappingCsp
******************************************************************************/
/**
* \addtogroup 0x1A01 0x1A01 | TxPdoMappingCsp
* @{
* \brief Object 0x1A01 (TxPdoMappingCsp) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - map status word<br>
* SubIndex 2 - map actual position<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5 - map actual velocity<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A01[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - map status word */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 - map actual position */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 - map actual velocity */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex19 */
/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A01[] = "TxPdoMappingCsp\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000"
"SubIndex 011\000"
"SubIndex 012\000"
"SubIndex 013\000"
"SubIndex 014\000"
"SubIndex 015\000"
"SubIndex 016\000"
"SubIndex 017\000"
"SubIndex 018\000"
"SubIndex 019\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - map status word */
UINT32 SI2; /* Subindex2 - map actual position */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 - map actual velocity */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */  
UINT32 SI19; /* Subindex19 -  */  
} OBJ_STRUCT_PACKED_END
TOBJ1A01;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A01 TxPdoMappingCsp0x1A01
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={19,0x60410010,0x68410010,0x60640020,0x606C0020,0x60770020,0x607F0020,0x68640020,0x686C0020,0x68770020,0x687F0020,0x68910020,0x68920020,0x68930020,0x68AA0020,0x68AB0020,0x68AC0020,0x60610008,0x68610008,0x00000010}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A02 : TxPdoMappingCsv
******************************************************************************/
/**
* \addtogroup 0x1A02 0x1A02 | TxPdoMappingCsv
* @{
* \brief Object 0x1A02 (TxPdoMappingCsv) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - map status word<br>
* SubIndex 2 - map actual position<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5 - map actual velocity<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A02[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 - map status word */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 - map actual position */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 - map actual velocity */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex29 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A02[] = "TxPdoMappingCsv\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000"
"SubIndex 011\000"
"SubIndex 012\000"
"SubIndex 013\000"
"SubIndex 014\000"
"SubIndex 015\000"
"SubIndex 016\000"
"SubIndex 017\000"
"SubIndex 018\000"
"SubIndex 019\000"
"SubIndex 020\000"
"SubIndex 021\000"
"SubIndex 022\000"
"SubIndex 023\000"
"SubIndex 024\000"
"SubIndex 025\000"
"SubIndex 026\000"
"SubIndex 027\000"
"SubIndex 028\000"
"SubIndex 029\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 - map status word */
UINT32 SI2; /* Subindex2 - map actual position */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 - map actual velocity */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */    
} OBJ_STRUCT_PACKED_END
TOBJ1A02;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A02 TxPdoMappingCsv0x1A02
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={29,0x60410010,0x68410010,0x60640020,0x606C0020,0x60770020,0x607F0020,0x68640020,0x686C0020,0x68770020,0x687F0020,0x70640020,0x706C0020,0x70770020,0x707F0020,0x78640020,0x786C0020,0x78770020,0x787F0020,0x80640020,0x806C0020,0x80770020,0x807F0020,0x68910020,0x68920020,0x68930020,0x68AA0020,0x60610008,0x68610008,0x00000010}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1C12 : SM2 assignment
******************************************************************************/
/**
* \addtogroup 0x1C12 0x1C12 | SM2 assignment
* @{
* \brief Object 0x1C12 (SM2 assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C12[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | ACCESS_WRITE_PREOP }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C12[] = "SM2 assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJ1C12;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C12 sRxPDOassign
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={1,{0x1600}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1C13 : SM3 assignment
******************************************************************************/
/**
* \addtogroup 0x1C13 0x1C13 | SM3 assignment
* @{
* \brief Object 0x1C13 (SM3 assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C13[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ | ACCESS_WRITE_PREOP },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ | ACCESS_WRITE_PREOP }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C13[] = "SM3 assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJ1C13;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C13 sTxPDOassign
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={1,{0x1A00}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x603F : Error Code
******************************************************************************/
/**
* \addtogroup 0x603F 0x603F | Error Code
* @{
* \brief Object 0x603F (Error Code) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x603F = { DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x603F[] = "Error Code\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT16 ErrorCode0x603F
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x0000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6040 : Controlword
******************************************************************************/
/**
* \addtogroup 0x6040 0x6040 | Controlword
* @{
* \brief Object 0x6040 (Controlword) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6040 = { DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6040[] = "Controlword\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT16 Controlword0x6040
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x0000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6041 : Statusword
******************************************************************************/
/**
* \addtogroup 0x6041 0x6041 | Statusword
* @{
* \brief Object 0x6041 (Statusword) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6041 = { DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6041[] = "Statusword\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT16 Statusword0x6041
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x0000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x605A : Quick stop option code
******************************************************************************/
/**
* \addtogroup 0x605A 0x605A | Quick stop option code
* @{
* \brief Object 0x605A (Quick stop option code) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x605A = { DEFTYPE_INTEGER16 , 0x10 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x605A[] = "Quick stop option code\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT16 QuickStopOptionCode0x605A
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x0002
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x605B : Shutdown option code
******************************************************************************/
/**
* \addtogroup 0x605B 0x605B | Shutdown option code
* @{
* \brief Object 0x605B (Shutdown option code) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x605B = { DEFTYPE_INTEGER16 , 0x10 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x605B[] = "Shutdown option code\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT16 ShutdownOptionCode0x605B
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x0000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x605C : Disable operation option code
******************************************************************************/
/**
* \addtogroup 0x605C 0x605C | Disable operation option code
* @{
* \brief Object 0x605C (Disable operation option code) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x605C = { DEFTYPE_INTEGER16 , 0x10 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x605C[] = "Disable operation option code\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT16 DisableOperationOptionCode0x605C
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x0001
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x605E : Fault reaction option code
******************************************************************************/
/**
* \addtogroup 0x605E 0x605E | Fault reaction option code
* @{
* \brief Object 0x605E (Fault reaction option code) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x605E = { DEFTYPE_INTEGER16 , 0x10 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x605E[] = "Fault reaction option code\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT16 FaultReactionOptionCode0x605E
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x0002
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6060 : Modes of operation
******************************************************************************/
/**
* \addtogroup 0x6060 0x6060 | Modes of operation
* @{
* \brief Object 0x6060 (Modes of operation) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6060 = { DEFTYPE_INTEGER8 , 0x08 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6060[] = "Modes of operation\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT8 ModesOfOperation0x6060
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= -1
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6061 : Modes of operation display
******************************************************************************/
/**
* \addtogroup 0x6061 0x6061 | Modes of operation display
* @{
* \brief Object 0x6061 (Modes of operation display) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6061 = { DEFTYPE_INTEGER8 , 0x08 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6061[] = "Modes of operation display\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT8 ModesOfOperationDisplay0x6061
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= -1
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6064 : Position actual value
******************************************************************************/
/**
* \addtogroup 0x6064 0x6064 | Position actual value
* @{
* \brief Object 0x6064 (Position actual value) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6064 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6064[] = "Position actual value\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 PositionActualValue0x6064
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x606C : Velocity actual value
******************************************************************************/
/**
* \addtogroup 0x606C 0x606C | Velocity actual value
* @{
* \brief Object 0x606C (Velocity actual value) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x606C = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x606C[] = "Velocity actual value\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 VelocityActualValue0x606C
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6077 : Torque actual value
******************************************************************************/
/**
* \addtogroup 0x6077 0x6077 | Torque actual value
* @{
* \brief Object 0x6077 (Torque actual value) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6077 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6077[] = "Torque actual value\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TorqueActualValue0x6077
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x607D : Software position limit
******************************************************************************/
/**
* \addtogroup 0x607D 0x607D | Software position limit
* @{
* \brief Object 0x607D (Software position limit) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Min position limit<br>
* SubIndex 2 - Max position limit<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x607D[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE }, /* Subindex1 - Min position limit */
{ DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE }}; /* Subindex2 - Max position limit */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x607D[] = "Software position limit\000"
"Min position limit\000"
"Max position limit\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
INT32 MinPositionLimit; /* Subindex1 - Min position limit */
INT32 MaxPositionLimit; /* Subindex2 - Max position limit */
} OBJ_STRUCT_PACKED_END
TOBJ607D;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ607D SoftwarePositionLimit0x607D
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={2,0x88CA6C00,0x77359400}
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x607F : ENC
******************************************************************************/
/**
* \addtogroup 0x607F 0x607F | ENC
* @{
* \brief Object 0x607F (ENC) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x607F = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x607F[] = "ENC\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 ENC0x607F
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x607A : Target position
******************************************************************************/
/**
* \addtogroup 0x607A 0x607A | Target position
* @{
* \brief Object 0x607A (Target position) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x607A = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x607A[] = "Target position\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetPosition0x607A
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6085 : Quick stop deceleration
******************************************************************************/
/**
* \addtogroup 0x6085 0x6085 | Quick stop deceleration
* @{
* \brief Object 0x6085 (Quick stop deceleration) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6085 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6085[] = "Quick stop deceleration\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 QuickStopDeceleration0x6085
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/





/******************************************************************************
*                    Object 0x60FF : Target velocity
******************************************************************************/
/**
* \addtogroup 0x60FF 0x60FF | Target velocity
* @{
* \brief Object 0x60FF (Target velocity) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x60FF = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x60FF[] = "Target velocity\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetVelocity0x60FF
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6071 : Target torque 
******************************************************************************/
/**
* \addtogroup 0x6071 0x6071 | Target torque
* @{
* \brief Object 0x6071 (Target torque) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6071 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6071[] = "Target torque\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetTorque0x6071
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6090 : PID
******************************************************************************/
/**
* \addtogroup 0x6090 0x6090 | PID
* @{
* \brief Object 0x6090 (PID) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - PID Current P<br>
* SubIndex 2 - PID Current I<br>
* SubIndex 3 - PID Velocity P<br>
* SubIndex 4 - PID Velocity I<br>
* SubIndex 5 - PID Position P<br>
* SubIndex 6 - PID Position I<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6090[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }, /* Subindex1 - PID Current P */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }, /* Subindex2 - PID Current I */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }, /* Subindex3 - PID Velocity P */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }, /* Subindex4 - PID Velocity I */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }, /* Subindex5 - PID Position P */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }}; /* Subindex6 - PID Position I */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6090[] = "PID\000"
"PID Current P\000"
"PID Current I\000"
"PID Velocity P\000"
"PID Velocity I\000"
"PID Position P\000"
"PID Position I\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 PIDCurrentP; /* Subindex1 - PID Current P */
UINT32 PIDCurrentI; /* Subindex2 - PID Current I */
UINT32 PIDVelocityP; /* Subindex3 - PID Velocity P */
UINT32 PIDVelocityI; /* Subindex4 - PID Velocity I */
UINT32 PIDPositionP; /* Subindex5 - PID Position P */
UINT32 PIDPositionI; /* Subindex6 - PID Position I */
} OBJ_STRUCT_PACKED_END
TOBJ6090;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6090 PID0x6090
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={6,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000}
#endif
;
/** @}*/


/******************************************************************************
*                    Object 0x6091 : PID for axis2
******************************************************************************/
/**
* \addtogroup 0x6091 0x6091 | PID for axis2
* @{
* \brief Object 0x6091 (PID for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - PID Current P<br>
* SubIndex 2 - PID Current I<br>
* SubIndex 3 - PID Velocity P<br>
* SubIndex 4 - PID Velocity I<br>
* SubIndex 5 - PID Position P<br>
* SubIndex 6 - PID Position I<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6091[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }, /* Subindex1 - PID for axis2 Current P */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }, /* Subindex2 - PID for axis2 Current I */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }, /* Subindex3 - PID for axis2 Velocity P */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }, /* Subindex4 - PID for axis2 Velocity I */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }, /* Subindex5 - PID for axis2 Position P */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE }}; /* Subindex6 - PID for axis2 Position I */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x6091[] = "PID for axis2\000"
"PID Current P\000"
"PID Current I\000"
"PID Velocity P\000"
"PID Velocity I\000"
"PID Position P\000"
"PID Position I\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 PIDCurrentP; /* Subindex1 - PID Current P */
UINT32 PIDCurrentI; /* Subindex2 - PID Current I */
UINT32 PIDVelocityP; /* Subindex3 - PID Velocity P */
UINT32 PIDVelocityI; /* Subindex4 - PID Velocity I */
UINT32 PIDPositionP; /* Subindex5 - PID Position P */
UINT32 PIDPositionI; /* Subindex6 - PID Position I */
} OBJ_STRUCT_PACKED_END
TOBJ6091;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6091 PID0x6091
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={6,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000}
#endif
;
/** @}*/

/******************************************************************************
*                    Object 0x6092 : LED control
******************************************************************************/
/**
* \addtogroup 0x6092 0x6092 | digital input
* @{
* \brief Object 0x6092 (digital input) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6092 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6092[] = "LED control\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 LEDControl0x6092
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6093 : ID
******************************************************************************/
/**
* \addtogroup 0x6093 0x6093 | ID
* @{
* \brief Object 0x6093 (ID) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6093 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6093[] = "ID\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 ID0x6093
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6094 : lifecount
******************************************************************************/
/**
* \addtogroup 0x6094 0x6094 | lifecount
* @{
* \brief Object 0x6094 (lifecount) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6094 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6094[] = "lifecount\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 Lifecount0x6094
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6095 : number
******************************************************************************/
/**
* \addtogroup 0x6095 0x6095 | number
* @{
* \brief Object 0x6095 (number) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6095 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6095[] = "number\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 Number0x6095
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6096 : ENCDATA
******************************************************************************/
/**
* \addtogroup 0x6096 0x6096 | ENCDATA
* @{
* \brief Object 0x6096 (ENCDATA) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6096 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6096[] = "ENCDATA\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 ENCDATA0x6096
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6097 : Lifecount sum
******************************************************************************/
/**
* \addtogroup 0x6097 0x6097 | Lifecount sum
* @{
* \brief Object 0x6097 (Lifecount sum) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6097 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6097[] = "Lifecount sum\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 LifecountSum0x6097
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6098 : Current1
******************************************************************************/
/**
* \addtogroup 0x6098 0x6098 | Current1
* @{
* \brief Object 0x6098 (Current1) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6098 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6098[] = "Current1\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 Current10x6098
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6099 : Current2
******************************************************************************/
/**
* \addtogroup 0x6099 0x6099 | Current2
* @{
* \brief Object 0x6099 (Current2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6099 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6099[] = "Current2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 Current20x6099
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x609A : Current3
******************************************************************************/
/**
* \addtogroup 0x609A 0x609A | Current3
* @{
* \brief Object 0x609A (Current3) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x609A = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x609A[] = "Current3\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 Current30x609A
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x609B : Mode1
******************************************************************************/
/**
* \addtogroup 0x609B 0x609B | Mode1
* @{
* \brief Object 0x609B (Mode1) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x609B = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x609B[] = "Mode1\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 Mode10x609B
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x609C : Mode2
******************************************************************************/
/**
* \addtogroup 0x609C 0x609C | Mode2
* @{
* \brief Object 0x609C (Mode2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x609C = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x609C[] = "Mode2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 Mode20x609C
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x609D : Mode3
******************************************************************************/
/**
* \addtogroup 0x609D 0x609D | Mode3
* @{
* \brief Object 0x609D (Mode3) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x609D = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x609D[] = "Mode3\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 Mode30x609D
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x60C2 : Interpolation time period
******************************************************************************/
/**
* \addtogroup 0x60C2 0x60C2 | Interpolation time period
* @{
* \brief Object 0x60C2 (Interpolation time period) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Interpolation time period value<br>
* SubIndex 2 - Interpolation time index<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x60C2[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE }, /* Subindex1 - Interpolation time period value */
{ DEFTYPE_INTEGER8 , 0x08 , ACCESS_READWRITE }}; /* Subindex2 - Interpolation time index */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x60C2[] = "Interpolation time period\000"
"Interpolation time period value\000"
"Interpolation time index\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT8 InterpolationTimePeriodValue; /* Subindex1 - Interpolation time period value */
INT8 InterpolationTimeIndex; /* Subindex2 - Interpolation time index */
} OBJ_STRUCT_PACKED_END
TOBJ60C2;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ60C2 InterpolationTimePeriod0x60C2
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={2,0x01,0xFD}
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x6840 : Controlword for axis2 
******************************************************************************/
/**
* \addtogroup 0x6040 0x6840 | Controlword for axis2
* @{
* \brief Object 0x6840 (Controlword for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6840 = { DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6840[] = "Controlword for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT16 ControlwordForAxis20x6840
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x0000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x6841 : Statusword for axis2
******************************************************************************/
/**
* \addtogroup 0x6841 0x6841 | Statusword for axis2
* @{
* \brief Object 0x6841 (Statusword for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6841 = { DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6841[] = "Statusword for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT16 StatuswordForAxis20x6841
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x0000
#endif
;
/** @}*/





/******************************************************************************
*                    Object 0x6860 : Modes of operation for axis2
******************************************************************************/
/**
* \addtogroup 0x6860 0x6860 | Modes of operation for axis2
* @{
* \brief Object 0x6860 (Modes of operation for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6860 = { DEFTYPE_INTEGER8 , 0x08 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6860[] = "Modes of operation for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT8 ModesOfOperationForAxis20x6860
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= -1
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6861 : Modes of operation display for axis2
******************************************************************************/
/**
* \addtogroup 0x6861 0x6861 | Modes of operation display for axis2
* @{
* \brief Object 0x6861 (Modes of operation display for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6861 = { DEFTYPE_INTEGER8 , 0x08 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6861[] = "Modes of operation display for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT8 ModesOfOperationDisplayForAxis20x6861
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= -1
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x6864 : Position actual value for axis2
******************************************************************************/
/**
* \addtogroup 0x6864 0x6864 | Position actual value for axis2
* @{
* \brief Object 0x6864 (Position actual value for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6864 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6864[] = "Position actual value for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 PositionActualValueForAxis20x6864
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x686C : Velocity actual value for axis2
******************************************************************************/
/**
* \addtogroup 0x686C 0x686C | Velocity actual value for axis2
* @{
* \brief Object 0x686C (Velocity actual value for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x686C = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x686C[] = "Velocity actual value for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 VelocityActualValueForAxis20x686C
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x6877 : Torque actual value for axis2
******************************************************************************/
/**
* \addtogroup 0x6877 0x6877 | Torque actual value for axis2
* @{
* \brief Object 0x6877 (Torque actual value for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6877 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6877[] = "Torque actual value for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TorqueActualValueForAxis20x6877
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x687F : ENC for axis2
******************************************************************************/
/**
* \addtogroup 0x687F 0x687F | ENC for axis2
* @{
* \brief Object 0x687F (ENC for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x687F = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x687F[] = "ENC for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 ENCForAxis20x687F
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x687A : Target position for axis2
******************************************************************************/
/**
* \addtogroup 0x687A 0x687A | Target position for axis2
* @{
* \brief Object 0x687A (Target position for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x687A = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x687A[] = "Target position for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetPositionForAxis20x687A
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x68FF : Target velocity for axis2
******************************************************************************/
/**
* \addtogroup 0x68FF 0x68FF | Target velocity for axis2
* @{
* \brief Object 0x68FF (Target velocity for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x68FF = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x68FF[] = "Target velocity for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetVelocityForAxis20x68FF
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6871 : Target torque for axis2
******************************************************************************/
/**
* \addtogroup 0x6871 0x6871 | Target torque for axis2
* @{
* \brief Object 0x6871 (Target torque for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6871 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6871[] = "Target torque for axis2\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetTorqueForAxis20x6871
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x7064 : Position actual value for axis3
******************************************************************************/
/**
* \addtogroup 0x7064 0x7064 | Position actual value for axis3
* @{
* \brief Object 0x7064 (Position actual value for axis3) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x7064 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x7064[] = "Position actual value for axis3\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 PositionActualValueForAxis30x7064
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x706C : Velocity actual value for axis3
******************************************************************************/
/**
* \addtogroup 0x706C 0x706C | Velocity actual value for axis3
* @{
* \brief Object 0x706C (Velocity actual value for axis3) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x706C = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x706C[] = "Velocity actual value for axis3\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 VelocityActualValueForAxis30x706C
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7077 : Torque actual value for axis3
******************************************************************************/
/**
* \addtogroup 0x7077 0x7077 | Torque actual value for axis3
* @{
* \brief Object 0x7077 (Torque actual value for axis3) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x7077 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x7077[] = "Torque actual value for axis3\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TorqueActualValueForAxis30x7077
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x707F : ENC for axis3
******************************************************************************/
/**
* \addtogroup 0x707F 0x707F | ENC for axis3
* @{
* \brief Object 0x707F (ENC for axis3) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x707F = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x707F[] = "ENC for axis3\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 ENCForAxis30x707F
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x707A : Target position for axis3
******************************************************************************/
/**
* \addtogroup 0x707A 0x707A | Target position for axis3
* @{
* \brief Object 0x707A (Target position for axis3) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x707A = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x707A[] = "Target position for axis3\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetPositionForAxis30x707A
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x70FF : Target velocity for axis3
******************************************************************************/
/**
* \addtogroup 0x70FF 0x70FF | Target velocity for axis3
* @{
* \brief Object 0x70FF (Target velocity for axis3) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x70FF = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x70FF[] = "Target velocity for axis3\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetVelocityForAxis30x70FF
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7071 : Target torque for axis3
******************************************************************************/
/**
* \addtogroup 0x7071 0x7071 | Target torque for axis3
* @{
* \brief Object 0x7071 (Target torque for axis3) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x7071 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x7071[] = "Target torque for axis3\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetTorqueForAxis30x7071
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/


































/******************************************************************************
*                    Object 0x7864 : Position actual value for axis4
******************************************************************************/
/**
* \addtogroup 0x7864 0x7864 | Position actual value for axis4
* @{
* \brief Object 0x7864 (Position actual value for axis4) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x7864 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x7864[] = "Position actual value for axis4\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 PositionActualValueForAxis40x7864
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x786C : Velocity actual value for axis4
******************************************************************************/
/**
* \addtogroup 0x786C 0x786C | Velocity actual value for axis4
* @{
* \brief Object 0x786C (Velocity actual value for axis4) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x786C = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x786C[] = "Velocity actual value for axis4\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 VelocityActualValueForAxis40x786C
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7877 : Torque actual value for axis4
******************************************************************************/
/**
* \addtogroup 0x7877 0x7877 | Torque actual value for axis4
* @{
* \brief Object 0x7877 (Torque actual value for axis4) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x7877 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x7877[] = "Torque actual value for axis4\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TorqueActualValueForAxis40x7877
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x787F : ENC for axis4
******************************************************************************/
/**
* \addtogroup 0x787F 0x787F | ENC for axis4
* @{
* \brief Object 0x787F (ENC for axis4) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x787F = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x787F[] = "ENC for axis4\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 ENCForAxis40x787F
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x787A : Target position for axis4
******************************************************************************/
/**
* \addtogroup 0x787A 0x787A | Target position for axis4
* @{
* \brief Object 0x787A (Target position for axis4) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x787A = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x787A[] = "Target position for axis4\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetPositionForAxis40x787A
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x78FF : Target velocity for axis4
******************************************************************************/
/**
* \addtogroup 0x78FF 0x78FF | Target velocity for axis4
* @{
* \brief Object 0x78FF (Target velocity for axis4) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x78FF = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x78FF[] = "Target velocity for axis4\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetVelocityForAxis40x78FF
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7871 : Target torque for axis4
******************************************************************************/
/**
* \addtogroup 0x7871 0x7871 | Target torque for axis4
* @{
* \brief Object 0x7871 (Target torque for axis4) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x7871 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x7871[] = "Target torque for axis4\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetTorqueForAxis40x7871
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




































/******************************************************************************
*                    Object 0x8064 : Position actual value for axis5
******************************************************************************/
/**
* \addtogroup 0x8064 0x8064 | Position actual value for axis5
* @{
* \brief Object 0x8064 (Position actual value for axis5) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x8064 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x8064[] = "Position actual value for axis5\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 PositionActualValueForAxis50x8064
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x806C : Velocity actual value for axis5
******************************************************************************/
/**
* \addtogroup 0x806C 0x806C | Velocity actual value for axis5
* @{
* \brief Object 0x806C (Velocity actual value for axis5) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x806C = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x806C[] = "Velocity actual value for axis5\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 VelocityActualValueForAxis50x806C
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8077 : Torque actual value for axis5
******************************************************************************/
/**
* \addtogroup 0x8077 0x8077 | Torque actual value for axis5
* @{
* \brief Object 0x8077 (Torque actual value for axis5) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x8077 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x8077[] = "Torque actual value for axis5\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TorqueActualValueForAxis50x8077
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x807F : ENC for axis5
******************************************************************************/
/**
* \addtogroup 0x807F 0x807F | ENC for axis5
* @{
* \brief Object 0x807F (ENC for axis5) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x807F = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x807F[] = "ENC for axis5\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 ENCForAxis50x807F
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x807A : Target position for axis5
******************************************************************************/
/**
* \addtogroup 0x807A 0x807A | Target position for axis5
* @{
* \brief Object 0x807A (Target position for axis5) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x807A = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x807A[] = "Target position for axis5\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetPositionForAxis50x807A
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x80FF : Target velocity for axis5
******************************************************************************/
/**
* \addtogroup 0x80FF 0x80FF | Target velocity for axis5
* @{
* \brief Object 0x80FF (Target velocity for axis5) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x80FF = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x80FF[] = "Target velocity for axis5\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetVelocityForAxis50x80FF
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x8071 : Target torque for axis5
******************************************************************************/
/**
* \addtogroup 0x8071 0x8071 | Target torque for axis5
* @{
* \brief Object 0x8071 (Target torque for axis5) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x8071 = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x8071[] = "Target torque for axis5\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 TargetTorqueForAxis50x8071
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/









/******************************************************************************
*                    Object 0x6502 : Supported drive modes
******************************************************************************/
/**
* \addtogroup 0x6502 0x6502 | Supported drive modes
* @{
* \brief Object 0x6502 (Supported drive modes) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6502 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6502[] = "Supported drive modes\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 SupportedDriveModes0x6502
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000100
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x6891 : Analog input
******************************************************************************/
/**
* \addtogroup 0x6891 0x6891 | Analog input
* @{
* \brief Object 0x6891 (Analog input) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6891 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6891[] = "Analog input\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 AnalogInput0x6891
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x6892 : Digital input
******************************************************************************/
/**
* \addtogroup 0x6892 0x6892 | Digital input
* @{
* \brief Object 0x6892 (Digital input) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6892 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6892[] = "Digital input\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 DigitalInput0x6892
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6893 : Error message
******************************************************************************/
/**
* \addtogroup 0x6893 0x6893 | Error message
* @{
* \brief Object 0x6893 (Error message) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x6893 = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x6893[] = "Error message\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 ErrorMessage0x6893
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x68AA : Heartbeat
******************************************************************************/
/**
* \addtogroup 0x68AA 0x68AA | Heartbeat
* @{
* \brief Object 0x68AA (Heartbeat) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x68AA = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x68AA[] = "Heartbeat\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 Heartbeat0x68AA
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0x68AB : Flux actual value
******************************************************************************/
/**
* \addtogroup 0x68AB 0x68AB | Flux actual value
* @{
* \brief Object 0x68AB (Flux actual value) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x68AB = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x68AB[] = "Flux actual value\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 FluxActualValue0x68AB
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x68AC : Flux actual value for axis2
******************************************************************************/
/**
* \addtogroup 0x68AC 0x68AC | Flux actual value for axis2
* @{
* \brief Object 0x68AC (Flux actual value for axis2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x68AC = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x68AC[] = "Flux actual value for axis2 value\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 FluxActualValueForAxis20x68AC
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/





/******************************************************************************
*                    Object 0x68BA : Host heartbeat
******************************************************************************/
/**
* \addtogroup 0x68BA 0x68BA | Host heartbeat
* @{
* \brief Object 0x68BA (Host heartbeat) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x68BA = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x68BA[] = "Host heartbeat\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 HostHeartbeat0x68BA
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/


/******************************************************************************
*                    Object 0x68BB : Digital output
******************************************************************************/
/**
* \addtogroup 0x68BB 0x68BB | Digital output
* @{
* \brief Object 0x68BB (Digital output) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x68BB = { DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READWRITE };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x68BB[] = "Digital output\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO UINT32 DigitalOutput0x68BB
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= 0x00000000
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x69FF : Device software version
******************************************************************************/
/**
* \addtogroup 0x69FF 0x69FF | Device software version
* @{
* \brief Object 0x69FF (Device software version) definition
*/
#ifdef _OBJD_
/**
* \brief Entry description
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x69FF = { DEFTYPE_INTEGER32 , 0x20 , ACCESS_READ };
/**
* \brief Object name
*/
OBJCONST UCHAR OBJMEM aName0x69FF[] = "Device software version\000\377";
#endif //#ifdef _OBJD_

/**
* \brief Object variable
*/
PROTO INT32 DeviceSoftwareVersion0x69FF
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
= DeviceVersion
#endif
;
/** @}*/




/******************************************************************************
*                    Object 0xF000 : Modular device profile
******************************************************************************/
/**
* \addtogroup 0xF000 0xF000 | Modular device profile
* @{
* \brief Object 0xF000 (Modular device profile) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Index distance <br>
* SubIndex 2 - Maximum number of modules <br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex1 - Index distance  */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }}; /* Subindex2 - Maximum number of modules  */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xF000[] = "Modular device profile\000"
"Index distance \000"
"Maximum number of modules \000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT16 IndexDistance; /* Subindex1 - Index distance  */
UINT16 MaximumNumberOfModules; /* Subindex2 - Maximum number of modules  */
} OBJ_STRUCT_PACKED_END
TOBJF000;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF000 ModularDeviceProfile0xF000
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={2,0x0800,0x0001}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xF010 : Module profile list
******************************************************************************/
/**
* \addtogroup 0xF010 0xF010 | Module profile list
* @{
* \brief Object 0xF010 (Module profile list) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF010[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0xF010[] = "Module profile list\000\377";
#endif //#ifdef _OBJD_

#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT32 aEntries[1];  /**< \brief Subindex 1 - 1 */
} OBJ_STRUCT_PACKED_END
TOBJF010;
#endif //#ifndef _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF010 ModuleProfileList0xF010
#if defined(_AX58200__MOTOR_CONTROL_) && (_AX58200__MOTOR_CONTROL_ == 1)
={1,{0x00020192}}
#endif
;
/** @}*/





PROTO UINT8 ModeRead(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);
PROTO UINT8 ModeWrite(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess);


#ifdef _OBJD_
TOBJECT    OBJMEM ApplicationObjDic[] = {
/* Object 0x1600 */
{NULL , NULL ,  0x1600 , {DEFTYPE_PDOMAPPING , 8 | (OBJCODE_REC << 8)} , asEntryDesc0x1600 , aName0x1600 , &RxPdoMappingCspCsv0x1600 , NULL , NULL , 0x0000 },
/* Object 0x1601 */
{NULL , NULL ,  0x1601 , {DEFTYPE_PDOMAPPING , 13 | (OBJCODE_REC << 8)} , asEntryDesc0x1601 , aName0x1601 , &RxPdoMappingCsp0x1601 , NULL , NULL , 0x0000 },
/* Object 0x1602 */
{NULL , NULL ,  0x1602 , {DEFTYPE_PDOMAPPING , 22 | (OBJCODE_REC << 8)} , asEntryDesc0x1602 , aName0x1602 , &RxPdoMappingCsv0x1602 , NULL , NULL , 0x0000 },
/* Object 0x1A00 */
{NULL , NULL ,  0x1A00 , {DEFTYPE_PDOMAPPING , 13 | (OBJCODE_REC << 8)} , asEntryDesc0x1A00 , aName0x1A00 , &TxPdoMappingCspCsv0x1A00 , NULL , NULL , 0x0000 },
/* Object 0x1A01 */
{NULL , NULL ,  0x1A01 , {DEFTYPE_PDOMAPPING , 19 | (OBJCODE_REC << 8)} , asEntryDesc0x1A01 , aName0x1A01 , &TxPdoMappingCsp0x1A01 , NULL , NULL , 0x0000 },
/* Object 0x1A02 */
{NULL , NULL ,  0x1A02 , {DEFTYPE_PDOMAPPING , 29 | (OBJCODE_REC << 8)} , asEntryDesc0x1A02 , aName0x1A02 , &TxPdoMappingCsv0x1A02 , NULL , NULL , 0x0000 },
/* Object 0x1C12 */
{NULL , NULL ,  0x1C12 , {DEFTYPE_UNSIGNED16 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C12 , aName0x1C12 , &sRxPDOassign , NULL , NULL , 0x0000 },
/* Object 0x1C13 */
{NULL , NULL ,  0x1C13 , {DEFTYPE_UNSIGNED16 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C13 , aName0x1C13 , &sTxPDOassign , NULL , NULL , 0x0000 },
/* Object 0x603F */
{NULL , NULL ,  0x603F , {DEFTYPE_UNSIGNED16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x603F , aName0x603F , &ErrorCode0x603F , NULL , NULL , 0x0000 },
/* Object 0x6040 */
{NULL , NULL ,  0x6040 , {DEFTYPE_UNSIGNED16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6040 , aName0x6040 , &Controlword0x6040 , NULL , NULL , 0x0000 },
/* Object 0x6041 */
{NULL , NULL ,  0x6041 , {DEFTYPE_UNSIGNED16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6041 , aName0x6041 , &Statusword0x6041 , NULL , NULL , 0x0000 },
/* Object 0x605A */
{NULL , NULL ,  0x605A , {DEFTYPE_INTEGER16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x605A , aName0x605A , &QuickStopOptionCode0x605A , NULL , NULL , 0x0000 },
/* Object 0x605B */
{NULL , NULL ,  0x605B , {DEFTYPE_INTEGER16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x605B , aName0x605B , &ShutdownOptionCode0x605B , NULL , NULL , 0x0000 },
/* Object 0x605C */
{NULL , NULL ,  0x605C , {DEFTYPE_INTEGER16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x605C , aName0x605C , &DisableOperationOptionCode0x605C , NULL , NULL , 0x0000 },
/* Object 0x605E */
{NULL , NULL ,  0x605E , {DEFTYPE_INTEGER16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x605E , aName0x605E , &FaultReactionOptionCode0x605E , NULL , NULL , 0x0000 },
/* Object 0x6060 */
{NULL , NULL ,  0x6060 , {DEFTYPE_INTEGER8 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6060 , aName0x6060 , &ModesOfOperation0x6060 , ModeRead , ModeWrite , 0x0000 },
/* Object 0x6061 */
{NULL , NULL ,  0x6061 , {DEFTYPE_INTEGER8 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6061 , aName0x6061 , &ModesOfOperationDisplay0x6061 , ModeRead , ModeWrite , 0x0000 },
/* Object 0x6064 */
{NULL , NULL ,  0x6064 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6064 , aName0x6064 , &PositionActualValue0x6064 , NULL , NULL , 0x0000 },
/* Object 0x606C */
{NULL , NULL ,  0x606C , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x606C , aName0x606C , &VelocityActualValue0x606C , NULL , NULL , 0x0000 },
/* Object 0x6077 */
{NULL , NULL ,  0x6077 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6077 , aName0x6077 , &TorqueActualValue0x6077 , NULL , NULL , 0x0000 },
/* Object 0x607D */
{NULL , NULL ,  0x607D , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0x607D , aName0x607D , &SoftwarePositionLimit0x607D , NULL , NULL , 0x0000 },
/* Object 0x607F */
{NULL , NULL ,  0x607F , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x607F , aName0x607F , &ENC0x607F , NULL , NULL , 0x0000 },
/* Object 0x607A */
{NULL , NULL ,  0x607A , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x607A , aName0x607A , &TargetPosition0x607A , NULL , NULL , 0x0000 },
/* Object 0x6085 */
{NULL , NULL ,  0x6085 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6085 , aName0x6085 , &QuickStopDeceleration0x6085 , NULL , NULL , 0x0000 },
/* Object 0x60FF */
{NULL , NULL ,  0x60FF , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x60FF , aName0x60FF , &TargetVelocity0x60FF , NULL , NULL , 0x0000 },
/* Object 0x6071 */
{NULL , NULL ,  0x6071 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6071 , aName0x6071 , &TargetTorque0x6071 , NULL , NULL , 0x0000 },
/* Object 0x6090 */
{NULL , NULL ,  0x6090 , {DEFTYPE_RECORD , 6 | (OBJCODE_REC << 8)} , asEntryDesc0x6090 , aName0x6090 , &PID0x6090 , NULL , NULL , 0x0000 },
/* Object 0x6091 */
{NULL , NULL ,  0x6091 , {DEFTYPE_RECORD , 6 | (OBJCODE_REC << 8)} , asEntryDesc0x6091 , aName0x6091 , &PID0x6091 , NULL , NULL , 0x0000 },
/* Object 0x6092 */
{NULL , NULL ,  0x6092 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6092 , aName0x6092 , &LEDControl0x6092 , NULL , NULL , 0x0000 },
/* Object 0x6093 */
{NULL , NULL ,  0x6093 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6093 , aName0x6093 , &ID0x6093 , NULL , NULL , 0x0000 },
/* Object 0x6094 */
{NULL , NULL ,  0x6094 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6094 , aName0x6094 , &Lifecount0x6094 , NULL , NULL , 0x0000 },
/* Object 0x6095 */
{NULL , NULL ,  0x6095 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6095 , aName0x6095 , &Number0x6095 , NULL , NULL , 0x0000 },
/* Object 0x6096 */
{NULL , NULL ,  0x6096 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6096 , aName0x6096 , &ENCDATA0x6096 , NULL , NULL , 0x0000 },
/* Object 0x6097 */
{NULL , NULL ,  0x6097 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6097 , aName0x6097 , &LifecountSum0x6097 , NULL , NULL , 0x0000 },
/* Object 0x6098 */
{NULL , NULL ,  0x6098 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6098 , aName0x6098 , &Current10x6098 , NULL , NULL , 0x0000 },
/* Object 0x6099 */
{NULL , NULL ,  0x6099 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6099 , aName0x6099 , &Current20x6099 , NULL , NULL , 0x0000 },
/* Object 0x609A */
{NULL , NULL ,  0x609A , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x609A , aName0x609A , &Current30x609A , NULL , NULL , 0x0000 },
/* Object 0x609B */
{NULL , NULL ,  0x609B , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x609B , aName0x609B , &Mode10x609B , NULL , NULL , 0x0000 },
/* Object 0x609C */
{NULL , NULL ,  0x609C , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x609C , aName0x609C , &Mode20x609C , NULL , NULL , 0x0000 },
/* Object 0x609D */
{NULL , NULL ,  0x609D , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x609D , aName0x609D , &Mode30x609D , NULL , NULL , 0x0000 },
/* Object 0x60C2 */
{NULL , NULL ,  0x60C2 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0x60C2 , aName0x60C2 , &InterpolationTimePeriod0x60C2 , NULL , NULL , 0x0000 },
/* Object 0x6840 */
{NULL , NULL ,  0x6840 , {DEFTYPE_UNSIGNED16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6840 , aName0x6840 , &ControlwordForAxis20x6840 , NULL , NULL , 0x0000 },
/* Object 0x6841 */
{NULL , NULL ,  0x6841 , {DEFTYPE_UNSIGNED16 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6841 , aName0x6841 , &StatuswordForAxis20x6841 , NULL , NULL , 0x0000 },
/* Object 0x6860 */
{NULL , NULL ,  0x6860 , {DEFTYPE_INTEGER8 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6860 , aName0x6860 , &ModesOfOperationForAxis20x6860 , ModeRead , ModeWrite , 0x0000 },
/* Object 0x6861 */
{NULL , NULL ,  0x6861 , {DEFTYPE_INTEGER8 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6861 , aName0x6861 , &ModesOfOperationDisplayForAxis20x6861 , ModeRead , ModeWrite , 0x0000 },
/* Object 0x6864 */
{NULL , NULL ,  0x6864 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6864 , aName0x6864 , &PositionActualValueForAxis20x6864 , NULL , NULL , 0x0000 },
/* Object 0x686C */
{NULL , NULL ,  0x686C , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x686C , aName0x686C , &VelocityActualValueForAxis20x686C , NULL , NULL , 0x0000 },
/* Object 0x6877 */
{NULL , NULL ,  0x6877 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6877 , aName0x6877 , &TorqueActualValueForAxis20x6877 , NULL , NULL , 0x0000 },
/* Object 0x687F */
{NULL , NULL ,  0x687F , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x687F , aName0x687F , &ENCForAxis20x687F , NULL , NULL , 0x0000 },
/* Object 0x687A */
{NULL , NULL ,  0x687A , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x687A , aName0x687A , &TargetPositionForAxis20x687A , NULL , NULL , 0x0000 },
/* Object 0x68FF */
{NULL , NULL ,  0x68FF , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x68FF , aName0x68FF , &TargetVelocityForAxis20x68FF , NULL , NULL , 0x0000 },
/* Object 0x6871 */
{NULL , NULL ,  0x6871 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6871 , aName0x6871 , &TargetTorqueForAxis20x6871 , NULL , NULL , 0x0000 },
/* Object 0x7064 */
{NULL , NULL ,  0x7064 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x7064 , aName0x7064 , &PositionActualValueForAxis30x7064 , NULL , NULL , 0x0000 },
/* Object 0x706C */
{NULL , NULL ,  0x706C , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x706C , aName0x706C , &VelocityActualValueForAxis30x706C , NULL , NULL , 0x0000 },
/* Object 0x7077 */
{NULL , NULL ,  0x7077 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x7077 , aName0x7077 , &TorqueActualValueForAxis30x7077 , NULL , NULL , 0x0000 },
/* Object 0x707F */
{NULL , NULL ,  0x707F , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x707F , aName0x707F , &ENCForAxis30x707F , NULL , NULL , 0x0000 },
/* Object 0x707A */
{NULL , NULL ,  0x707A , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x707A , aName0x707A , &TargetPositionForAxis30x707A , NULL , NULL , 0x0000 },
/* Object 0x70FF */
{NULL , NULL ,  0x70FF , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x70FF , aName0x70FF , &TargetVelocityForAxis30x70FF , NULL , NULL , 0x0000 },
/* Object 0x7071 */
{NULL , NULL ,  0x7071 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x7071 , aName0x7071 , &TargetTorqueForAxis30x7071 , NULL , NULL , 0x0000 },
/* Object 0x7864 */
{NULL , NULL ,  0x7864 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x7864 , aName0x7864 , &PositionActualValueForAxis40x7864 , NULL , NULL , 0x0000 },
/* Object 0x786C */
{NULL , NULL ,  0x786C , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x786C , aName0x786C , &VelocityActualValueForAxis40x786C , NULL , NULL , 0x0000 },
/* Object 0x7877 */
{NULL , NULL ,  0x7877 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x7877 , aName0x7877 , &TorqueActualValueForAxis40x7877 , NULL , NULL , 0x0000 },
/* Object 0x787F */
{NULL , NULL ,  0x787F , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x787F , aName0x787F , &ENCForAxis40x787F , NULL , NULL , 0x0000 },
/* Object 0x787A */
{NULL , NULL ,  0x787A , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x787A , aName0x787A , &TargetPositionForAxis40x787A , NULL , NULL , 0x0000 },
/* Object 0x78FF */
{NULL , NULL ,  0x78FF , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x78FF , aName0x78FF , &TargetVelocityForAxis40x78FF , NULL , NULL , 0x0000 },
/* Object 0x7871 */
{NULL , NULL ,  0x7871 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x7871 , aName0x7871 , &TargetTorqueForAxis40x7871 , NULL , NULL , 0x0000 },
/* Object 0x8064 */
{NULL , NULL ,  0x8064 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x8064 , aName0x8064 , &PositionActualValueForAxis50x8064 , NULL , NULL , 0x0000 },
/* Object 0x806C */
{NULL , NULL ,  0x806C , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x806C , aName0x806C , &VelocityActualValueForAxis50x806C , NULL , NULL , 0x0000 },
/* Object 0x8077 */
{NULL , NULL ,  0x8077 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x8077 , aName0x8077 , &TorqueActualValueForAxis50x8077 , NULL , NULL , 0x0000 },
/* Object 0x807F */
{NULL , NULL ,  0x807F , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x807F , aName0x807F , &ENCForAxis50x807F , NULL , NULL , 0x0000 },
/* Object 0x807A */
{NULL , NULL ,  0x807A , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x807A , aName0x807A , &TargetPositionForAxis50x807A , NULL , NULL , 0x0000 },
/* Object 0x80FF */
{NULL , NULL ,  0x80FF , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x80FF , aName0x80FF , &TargetVelocityForAxis50x80FF , NULL , NULL , 0x0000 },
/* Object 0x8071 */
{NULL , NULL ,  0x8071 , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x8071 , aName0x8071 , &TargetTorqueForAxis50x8071 , NULL , NULL , 0x0000 },
/* Object 0x6502 */
{NULL , NULL ,  0x6502 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6502 , aName0x6502 , &SupportedDriveModes0x6502 , NULL , NULL , 0x0000 },
/* Object 0x6891 */
{NULL , NULL ,  0x6891 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6891 , aName0x6891 , &AnalogInput0x6891 , NULL , NULL , 0x0000 },
/* Object 0x6892 */
{NULL , NULL ,  0x6892 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6892 , aName0x6892 , &DigitalInput0x6892 , NULL , NULL , 0x0000 },
/* Object 0x6893 */
{NULL , NULL ,  0x6893 , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x6893 , aName0x6893 , &ErrorMessage0x6893 , NULL , NULL , 0x0000 },
/* Object 0x68AA */
{NULL , NULL ,  0x68AA , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x68AA , aName0x68AA , &Heartbeat0x68AA , NULL , NULL , 0x0000 },
/* Object 0x68AB */
{NULL , NULL ,  0x68AB , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x68AB , aName0x68AB , &FluxActualValue0x68AB , NULL , NULL , 0x0000 },
/* Object 0x68AC */
{NULL , NULL ,  0x68AC , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x68AC , aName0x68AC , &FluxActualValueForAxis20x68AC , NULL , NULL , 0x0000 },
/* Object 0x68BA */
{NULL , NULL ,  0x68BA , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x68BA , aName0x68BA , &HostHeartbeat0x68BA , NULL , NULL , 0x0000 },
/* Object 0x68BB */
{NULL , NULL ,  0x68BB , {DEFTYPE_INTEGER32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x68BB , aName0x68BB , &DigitalOutput0x68BB , NULL , NULL , 0x0000 },
/* Object 0x69FF */
{NULL , NULL ,  0x69FF , {DEFTYPE_UNSIGNED32 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x69FF , aName0x69FF , &DeviceSoftwareVersion0x69FF , NULL , NULL , 0x0000 },
/* Object 0xF000 */
{NULL , NULL ,  0xF000 , {DEFTYPE_RECORD , 2 | (OBJCODE_REC << 8)} , asEntryDesc0xF000 , aName0xF000 , &ModularDeviceProfile0xF000 , NULL , NULL , 0x0000 },
/* Object 0xF010 */
{NULL , NULL ,  0xF010 , {DEFTYPE_UNSIGNED32 , 1 | (OBJCODE_ARR << 8)} , asEntryDesc0xF010 , aName0xF010 , &ModuleProfileList0xF010 , NULL , NULL , 0x0000 },
{NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};
#endif    //#ifdef _OBJD_
#undef PROTO

/** @}*/
#define _AX58200__MOTOR_CONTROL_OBJECTS_H_

/**
\addtogroup AX58200 MotorControl implementation file
@{
*/

/**
\file AX58200_MotorControl.c
\brief Implementation

\version 1.0.0.0
*/


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "ecat_def.h"

#include "applInterface.h"

#include "usart.h"
#include "task_tmc4671.h"
#include "TMC4671.h"
#include "spi_ENC2.h"
#include "spi_ENC3.h"
#include "FMC_DATA.h"
#include "SPI_Simulation.h"
#define _AX58200__MOTOR_CONTROL_ 1
#include "AX58200_MotorControl.h"
#undef _AX58200__MOTOR_CONTROL_
/*-----------------------------------------------------------------------------------------
------
------    global variables and constants
------
-----------------------------------------------------------------------------------------*/
extern TOBJECT OBJMEM ApplicationObjDic[];
CiA402_AXIS_T  CiA402Axis;
ErrorMessagePack ErrorMessage;
extern struct
{
	unsigned char loop;
	unsigned char flag;
	unsigned int counter;
}breakout;

extern struct
{
	UINT16 Current_P;
	UINT16 Current_I;
	UINT16 Velocity_P;
	UINT16 Velocity_I;
	UINT16 Position_P;
	UINT16 Position_I; 
}PID4671;
extern struct
{
	unsigned int count;
	unsigned char loop;
	unsigned char mode;
}led;
extern unsigned int ENC2_raw;
extern unsigned char ENC2_counter;
int32_t Joint6_Position_Last=0;
int32_t Joint6_Position=0;
/*--------------------------------------------------------------------------------------
------
------    local types and defines
------
--------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    application specific functions
------
-----------------------------------------------------------------------------------------*/
void CiA402_StateMachine(CiA402_AXIS_T *pAxis);
void CiA402_Application(CiA402_AXIS_T *pAxis);
int  CiA402_MotionControl(CiA402_AXIS_T *pAxis);
void CiA402_LocalError(UINT16 ErrorCode);
BOOL CiA402_TransitionAction(INT16 Characteristic, CiA402_AXIS_T *pAxis);
/*
 * ----------------------------------------------------------------------------
 * Function Name: CiA402_Init()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void CiA402_Init(void)
{
	CiA402_AXIS_T  *pAxis = &CiA402Axis;
	
	/* Initialize axis data structure */
	HMEMSET(pAxis, 0, SIZEOF(CiA402_AXIS_T));
	
	pAxis->bAxisIsActive = FALSE;
	pAxis->bBrakeApplied = TRUE;
	pAxis->bLowLevelPowerApplied = TRUE;
	pAxis->bHighLevelPowerApplied = FALSE;
	pAxis->bAxisFunctionEnabled = FALSE;
	pAxis->bConfigurationAllowed = TRUE;
	
	pAxis->i16State = STATE_NOT_READY_TO_SWITCH_ON;
	pAxis->u16PendingOptionCode = 0x00;
	
	/* Binding object dictionary into axis data structure */
	pAxis->pObjDic = ApplicationObjDic;

	/* Link CiA402 variables */
	pAxis->CiA402Var.pRxPdoMappingCspCsv0x1600 = &RxPdoMappingCspCsv0x1600;
	pAxis->CiA402Var.pRxPdoMappingCsp0x1601 = &RxPdoMappingCsp0x1601;
	pAxis->CiA402Var.pRxPdoMappingCsv0x1602 = &RxPdoMappingCsv0x1602;
	pAxis->CiA402Var.pTxPdoMappingCspCsv0x1A00 = &TxPdoMappingCspCsv0x1A00;
	pAxis->CiA402Var.pTxPdoMappingCsp0x1A01 = &TxPdoMappingCsp0x1A01;
	pAxis->CiA402Var.pTxPdoMappingCsv0x1A02 = &TxPdoMappingCsv0x1A02;	
	pAxis->CiA402Var.psRxPDOassign = &sRxPDOassign;	
	pAxis->CiA402Var.psTxPDOassign = &sTxPDOassign;	
	pAxis->CiA402Var.pErrorCode0x603F = &ErrorCode0x603F;
    /**                        AXIS 1                             **/        
	pAxis->CiA402Var.pControlword0x6040 = &Controlword0x6040;
	pAxis->CiA402Var.pStatusword0x6041 = &Statusword0x6041;
	pAxis->CiA402Var.pQuickStopOptionCode0x605A = &QuickStopOptionCode0x605A;	
	pAxis->CiA402Var.pShutdownOptionCode0x605B = &ShutdownOptionCode0x605B;
	pAxis->CiA402Var.pDisableOperationOptionCode0x605C = &DisableOperationOptionCode0x605C;
	pAxis->CiA402Var.pFaultReactionOptionCode0x605E = &FaultReactionOptionCode0x605E;
	pAxis->CiA402Var.pModesOfOperation0x6060 = &ModesOfOperation0x6060;
	pAxis->CiA402Var.pModesOfOperationDisplay0x6061 = &ModesOfOperationDisplay0x6061;        
	pAxis->CiA402Var.pPositionActualValue0x6064 = &PositionActualValue0x6064;    
	pAxis->CiA402Var.pVelocityActualValue0x606C = &VelocityActualValue0x606C;
	pAxis->CiA402Var.pTorqueActualValue0x6077 = &TorqueActualValue0x6077;    
	pAxis->CiA402Var.pSoftwarePositionLimit0x607D = &SoftwarePositionLimit0x607D;    
	pAxis->CiA402Var.pENC0x607F = &ENC0x607F;    
	pAxis->CiA402Var.pTargetPosition0x607A = &TargetPosition0x607A;
	pAxis->CiA402Var.pQuickStopDeceleration0x6085 = &QuickStopDeceleration0x6085;
	pAxis->CiA402Var.pTargetVelocity0x60FF = &TargetVelocity0x60FF;
    pAxis->CiA402Var.pTargetTorque0x6071   = &TargetTorque0x6071;
    /**                         COE                                **/ 
	pAxis->CiA402Var.pPID_Current_P0x6090 = &PID0x6090.PIDCurrentP;
	pAxis->CiA402Var.pPID_Current_I0x6090 = &PID0x6090.PIDCurrentI;
	pAxis->CiA402Var.pPIN_Velocity_P0x6090 = &PID0x6090.PIDVelocityP;
	pAxis->CiA402Var.pPIN_Velocity_I0x6090 = &PID0x6090.PIDVelocityI;
	pAxis->CiA402Var.pPID_Position_P0x6090 = &PID0x6090.PIDPositionP;
	pAxis->CiA402Var.pPID_Position_I0x6090 = &PID0x6090.PIDPositionI;
	pAxis->CiA402Var.pLEDControl0x6092 = &LEDControl0x6092;
	pAxis->CiA402Var.pID0x6093 = &ID0x6093;    
	pAxis->CiA402Var.pLifecount0x6094 = &Lifecount0x6094;
	pAxis->CiA402Var.pNumber0x6095 = &Number0x6095;
	pAxis->CiA402Var.pENCDATA0x6096 = &ENCDATA0x6096;
	pAxis->CiA402Var.pLifecountSum0x6097 = &LifecountSum0x6097;
	pAxis->CiA402Var.pCurrent10x6098 = &Current10x6098;
	pAxis->CiA402Var.pCurrent20x6099 = &Current20x6099;
	pAxis->CiA402Var.pCurrent30x609A = &Current30x609A;
	pAxis->CiA402Var.pMode10x609B = &Mode10x609B;
	pAxis->CiA402Var.pMode20x609C = &Mode20x609C;
	pAxis->CiA402Var.pMode30x609D = &Mode30x609D;
	pAxis->CiA402Var.pInterpolationTimePeriod0x60C2 = &InterpolationTimePeriod0x60C2;    
    
    /**                          AXIS 2                         **/          
	pAxis->CiA402Var.pControlwordForAxis20x6840 = &ControlwordForAxis20x6840;
	pAxis->CiA402Var.pStatuswordForAxis20x6841 = &StatuswordForAxis20x6841;
	pAxis->CiA402Var.pPositionActualValueForAxis20x6864 = &PositionActualValueForAxis20x6864;
	pAxis->CiA402Var.pVelocityActualValueForAxis20x686C = &VelocityActualValueForAxis20x686C;
	pAxis->CiA402Var.pTorqueActualValueForAxis20x6877 = &TorqueActualValueForAxis20x6877;
	pAxis->CiA402Var.pENCForAxis20x687F = &ENCForAxis20x687F;
	pAxis->CiA402Var.pTargetPositionForAxis20x687A = &TargetPositionForAxis20x687A;
	pAxis->CiA402Var.pTargetVelocityForAxis20x68FF = &TargetVelocityForAxis20x68FF;
	pAxis->CiA402Var.pTargetTorqueForAxis20x6871 = &TargetTorqueForAxis20x6871;
    
    /**                            AXIS 3                     **/          
    
	pAxis->CiA402Var.pPositionActualValueForAxis30x7064 = &PositionActualValueForAxis30x7064;
	pAxis->CiA402Var.pVelocityActualValueForAxis30x706C = &VelocityActualValueForAxis30x706C;
	pAxis->CiA402Var.pTorqueActualValueForAxis30x7077 = &TorqueActualValueForAxis30x7077;
	pAxis->CiA402Var.pENCForAxis30x707F = &ENCForAxis30x707F;
	pAxis->CiA402Var.pTargetPositionForAxis30x707A = &TargetPositionForAxis30x707A;
	pAxis->CiA402Var.pTargetVelocityForAxis30x70FF = &TargetVelocityForAxis30x70FF;
	pAxis->CiA402Var.pTargetTorqueForAxis30x7071 = &TargetTorqueForAxis30x7071;
	pAxis->CiA402Var.pSupportedDriveModes0x6502 = &SupportedDriveModes0x6502;

    /**                           feedback                   **/        

	pAxis->CiA402Var.pAnalogInput0x6891 = &AnalogInput0x6891;
	pAxis->CiA402Var.pDigitalInput0x6892 = &DigitalInput0x6892;
	pAxis->CiA402Var.pErrorMessage0x6893 = &ErrorMessage0x6893;
	pAxis->CiA402Var.pHeartbeat0x68AA = &Heartbeat0x68AA;
	pAxis->CiA402Var.pFluxActualValue0x68AB = &FluxActualValue0x68AB;
	pAxis->CiA402Var.pFluxActualValueForAxis20x68AC = &FluxActualValueForAxis20x68AC;
	pAxis->CiA402Var.pHostHeartbeat0x68BA = &HostHeartbeat0x68BA;
	pAxis->CiA402Var.pDigitalOutput0x68BB = &DigitalOutput0x68BB;
	pAxis->CiA402Var.pDeviceSoftwareVersion0x69FF = &DeviceSoftwareVersion0x69FF;

	pAxis->CiA402Var.pModularDeviceProfile0xF000 = &ModularDeviceProfile0xF000;	
	pAxis->CiA402Var.pModuleProfileList0xF010 = &ModuleProfileList0xF010;

	/* Binding MC structure */
	pAxis->pMC = &McObj;
	
} /* End of CiA402_Init() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: CiA402_DeInit()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void CiA402_DeInit(void)
{
	nPdOutputSize = 0;
	nPdInputSize = 0;
	
} /* End of CiA402_DeInit() */

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    CiA402_MotionControl
 \brief
*////////////////////////////////////////////////////////////////////////////////////////

void ABN_HALL(void)
{
	static unsigned char loop=0;
	static INT16 temp0,temp1;
	switch(loop)
	{
		case 0:
		{
			switch(abn_offset)
			{
				case 6345:
				{
					loop=3;
					break;
				}
				case 17268:
				{
					loop=5;
					break;
				}
				case 28190:
				{
					loop=7;
					break;
				}
				case -26423:
				{
					loop=9;
					break;
				}
				case -15500:
				{
					loop=11;
					break;
				}
				case -4578:
				{
					loop=13;
					break;
				}
			}
			break;
		}
		case 1:
		{
			break;
		}
		case 3://6345
		{
			temp0=tmc4671_getHALLPHI();
			if((temp0==17268)||(temp0==-4578))
			{
				loop=4;
			}

			break;
		}
		case 4:
		{
			temp1=tmc4671_getPHI();
			
			if(temp0==17268)  //
			{
				temp0=6345+5461;   //当前值+高电平信号的一半
				temp0-=temp1;
				abn_offset+=temp0;
				loop=5;
			}
			else if(temp0==-4578)
			{
				temp0+=5461;
				temp1-=temp0;
				abn_offset-=temp1;
				loop=13;
			}
            //加上 绝对值计算得出的电角度 
			tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, abn_offset<<16);
            
			break;
		}
		case 5://17268
		{
			temp0=tmc4671_getHALLPHI();
			if((temp0==28190)||(temp0==6345))
			{
				loop=6;
			}

			break;
		}
		case 6:
		{
			temp1=tmc4671_getPHI();
			
			
			if(temp0==28190)
			{
				loop=7;
				temp0=17268+5461;
				temp0-=temp1;
				abn_offset+=temp0;
			}
			else if(temp0==6345)
			{
				loop=3;
				temp0+=5461;
				temp1-=temp0;
				abn_offset-=temp1;
			}
			tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, abn_offset<<16);
			break;
		}
		case 7://28190
		{
			temp0=tmc4671_getHALLPHI();
			if((temp0==-26423)||(temp0==17268))
			{
				loop=8;
			}

			break;
		}
		case 8:
		{
			temp1=tmc4671_getPHI();
			if(temp0==-26423)
			{
				loop=9;
				temp0=28190+5461;
				temp0-=temp1;
				abn_offset+=temp0;
			}
			else if(temp0==17268)
			{
				loop=5;
				temp0+=5461;
				temp1-=temp0;
				abn_offset-=temp1;
			}
			tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, abn_offset<<16);
			break;
		}
		case 9://-26423
		{
			temp0=tmc4671_getHALLPHI();
			if((temp0==-15500)||(temp0==28190))
			{
				loop=10;
			}

			break;
		}
		case 10:
		{
			temp1=tmc4671_getPHI();
			if(temp0==-15500)
			{
				loop=11;
				temp0=-26423+5461;
				temp0-=temp1;
				abn_offset+=temp0;
			}
			else if(temp0==28190)
			{
				loop=7;
				temp0+=5461;
				temp1-=temp0;
				abn_offset-=temp1;
			}
			tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, abn_offset<<16);
			break;
		}
		case 11://-15500
		{
			temp0=tmc4671_getHALLPHI();
			if((temp0==-4578)||(temp0==-26423))
			{
				loop=12;
			}

			break;
		}
		case 12:
		{
			temp1=tmc4671_getPHI();
			
			
			if(temp0==-4578)
			{
				loop=13;
				temp0=-15500+5461;
				temp0-=temp1;
			abn_offset+=temp0;
			}
			else if(temp0==-26423)
			{
				loop=9;
				temp0+=5461;
				temp1-=temp0;
			abn_offset-=temp1;
			}
			tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, abn_offset<<16);
			break;
		}
		case 13://-4578
		{
			temp0=tmc4671_getHALLPHI();
			if((temp0==6345)||(temp0==-15500))
			{
				loop=14;
			}

			break;
		}
		case 14:
		{
			temp1=tmc4671_getPHI();
			if(temp0==6345)
			{
				loop=3;
				temp0=-4578+5461;
				temp0-=temp1;
			abn_offset+=temp0;
			}
			else if(temp0==-15500)
			{
				loop=11;
				temp0+=5461;
				temp1-=temp0;
			abn_offset-=temp1;
			}
			tmc4671_writeInt(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, abn_offset<<16);
			break;
		}
		default: break;
	}
}

unsigned char motorcurrent;
int ABZ_count;
short count_offset=0;

void delay(unsigned int n)
{
	unsigned char i;
	i=10;
	while(n--)
	{
		while(i--);
	}
}

unsigned char motorcurrentmode_en(void)
{
	unsigned char temp;
	if(!motorcurrent)
	{
		tmc4671_switchToMotionMode(0, TMC4671_MOTION_MODE_TORQUE );
		EN;
		tmc4671_setTargetTorque_raw(0, -850);
		delay(10000);
		if(HW_GPIO_ReadPin(PC,BIT_5))
		{
			motorcurrent=1;
			return 32;
		}
		else
		{
			tmc4671_setTargetTorque_raw(0, 850);
			delay(10000);
			if(HW_GPIO_ReadPin(PC,BIT_5))
			{
				motorcurrent=1;
				return 32;
			}
		}
		breakout.flag=1;
		motorcurrent=1;
		if(currentmode==0)
		{
			tmc4671_switchToMotionMode(0, TMC4671_MOTION_MODE_POSITION );
		}
	}
	
	return 0;
	
}	

short ABZ_Eangle(void)
{
	
	short Eangle;
	
	Eangle=(short)(ABZ_count*8%65536);
	Eangle+=ABZ_HALL_offset;
	Eangle+=count_offset;
	
	return Eangle;
}

int32_t HALLPEI = 0;
int32_t PEI = 0; 
int CiA402_MotionControl(CiA402_AXIS_T *pAxis)
{
	UINT16 tmp16;
	unsigned temp;
	int TargetPosition;
	int TargetVelocity;
	int TargetTorque;
	unsigned char PID_FLAG=0;
	int hall_abz;	

    
/************************数据反馈****************************************/    
    *(pAxis->CiA402Var.pPositionActualValue0x6064) = tmc4671_getActualPosition(0);
 	*(pAxis->CiA402Var.pVelocityActualValue0x606C) = tmc4671_getActualVelocity(0);   
	*(pAxis->CiA402Var.pTorqueActualValue0x6077)   = tmc4671_getActualTorque_raw(0);

    
/************************ENC数据处理****************************************/  
    
    FR_ENC.FR_Mode_1 = (*(pAxis->CiA402Var.pDigitalOutput0x68BB) >> 16) & 0xffff;
    switch(FR_ENC.FR_Mode_1)
    {
        case 1:   //读取版本模式
            Current10x6098=FRReadVersion();
        break;
        
        case 2:   //校准模式
            FRWriteJZ();
        break;
        
        case 0:   //数据读取模式      
            FR_ENC.FR_Motor1Data1 = SimulationSPI_FRENCRead();
            if((FR_ENC.FR_Motor1Data1==0 || FR_ENC.FR_Motor1Data1==131071)&& (FR_ENC.FR_CRC_Flag==1) ) 
            {
                FR_ENC.FR_DataBlank_Times++;
                if(FR_ENC.FR_DataBlank_Times>10000)
                {
                    FR_ENC.FR_DataBlank_Flag=1;
                }
            }
            else
            {
                 FR_ENC.FR_DataBlank_Times=0;
                 FR_ENC.FR_DataBlank_Flag=0;
            }    
            if(FR_ENC.FR_CRC_Flag==0)  *(pAxis->CiA402Var.pENC0x607F)=FR_ENC.FR_Motor1Data1;
        break;
        default:
        break;
    }     
    
    ErrorMessage.ENCError =   FR_ENC.FR_DataBlank_Flag<<6   |   FR_ENC.FR_ErrorFlagTimes_Flag<<7    |  FR_ENC.FR_CRCVerifyErrorTimes_Flag<<8  \
                            | FR_ENC.FR_DataBlank_Flag_2<<9 |   FR_ENC.FR_ErrorFlagTimes_Flag_2<<10  |  FR_ENC.FR_CRCVerifyErrorTimes_Flag_2<<11  \
                            | FR_ENC.FR_DataBlank_Flag_3<<12 |   FR_ENC.FR_ErrorFlagTimes_Flag_3<<13  |  FR_ENC.FR_CRCVerifyErrorTimes_Flag_3<<14 ;     

    
/************************电机数据处理****************************************/     
    
	ABZ_count=*(pAxis->CiA402Var.pPositionActualValue0x6064);   
    ABN_HALL();
    HALLPEI = tmc4671_getHALLPHI(); 
    PEI = tmc4671_getPHI(); 
    hall_abz =  HALLPEI -  PEI;  
    if(abs(hall_abz)>50000 || abs(hall_abz)<20000)
    {
        ErrorMessage.PEIDeviationError = 0;                
    }
    else
    {
        ErrorMessage.PEIDeviationError = 1<<24; 
    }
    
    
    //电流模式下接收目标位置，避免去使能切换模式时，出现抖动
    if(Servo_Ready_Flag)
    {       
        TargetPosition = *(pAxis->CiA402Var.pTargetPosition0x607A);         
        tmc4671_setAbsolutTargetPosition(0,TargetPosition);
        TargetTorque = *(pAxis->CiA402Var.pTargetTorque0x6071);
        tmc4671_setTargetTorque_raw(0, TargetTorque);    
    }  
    
/************************电机数据处理****************************************/ 
	if ((*(pAxis->CiA402Var.pModesOfOperationDisplay0x6061) != *(pAxis->CiA402Var.pModesOfOperation0x6060)))
	{
		*(pAxis->CiA402Var.pModesOfOperationDisplay0x6061) = *(pAxis->CiA402Var.pModesOfOperation0x6060);
		switch(*(pAxis->CiA402Var.pModesOfOperationDisplay0x6061)&0xFF)
		{
			case 0:
			{
				currentmode=0;
				tmc4671_switchToMotionMode(0, TMC4671_MOTION_MODE_POSITION );
				Position_PID=0;
				break;
			}
			case 1:
			{                
				currentmode=1;
				tmc4671_switchToMotionMode(0, TMC4671_MOTION_MODE_TORQUE );
				Position_PID=0;

				break;
			}
			case 3:
			{
				count_offset=ABZ_Eangle();
				count_offset-=ABZ_HALL_offset;
				tmc4671_setActualPosition(0, 0);
				break;
			}
			case 4:
			{
				tmc4671_switchToMotionMode(0, TMC4671_MOTION_MODE_VELOCITY );
				Position_PID=0;
				break;
			}
			case 5:
			{
				tmc4671_switchToMotionMode(0, TMC4671_MOTION_MODE_POSITION );
				Position_PID=0;
				break;
			}
			case 6:
			{
				tmc4671_switchToMotionMode(0, TMC4671_MOTION_MODE_TORQUE );
				Position_PID=1;
				break;
			}
			case 10:
			{
				*(pAxis->CiA402Var.pDigitalInput0x6892)=0;
				break;
			}
			default:break;
		}	
	}
	if(Servo_Ready_Flag)
	{
		switch(*(pAxis->CiA402Var.pModesOfOperationDisplay0x6061)&0xFF)
		{
			case 0:
			{
				EN;				
				breakout.flag=1;

				break;
			}
			case 1:
			{
				EN;				
				breakout.flag=1;

				
				break;
			}
			case 4:
			{
				EN;
				breakout.flag=1;
				TargetVelocity = *(pAxis->CiA402Var.pTargetVelocity0x60FF);
				tmc4671_setTargetVelocity(0, TargetVelocity);
				TargetTorque = *(pAxis->CiA402Var.pTargetTorque0x6071);
				tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_OFFSET, TargetTorque<<16);
				break;
			}
			case 5:
			{
				EN;
				breakout.flag=1;
				TargetPosition = *(pAxis->CiA402Var.pTargetPosition0x607A);
				tmc4671_setAbsolutTargetPosition(0, TargetPosition);
				TargetTorque = *(pAxis->CiA402Var.pTargetTorque0x6071);
				tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_OFFSET, TargetTorque<<16);
				break;
			}
			case 6:
			{
				EN;
				breakout.flag=1;
				Set_position = *(pAxis->CiA402Var.pTargetPosition0x607A);
				Current_FB =*(pAxis->CiA402Var.pTargetTorque0x6071);
				break;
			}
			default:break;
		}
	}
	else
	{
		stop=1;
		Position_PID=0;
		motorcurrent=0;
	}
    

/************************PID处理机制****************************************/     
	if((PID4671.Current_P!=*(pAxis->CiA402Var.pPID_Current_P0x6090))    ||  (PID4671.Current_I!=*(pAxis->CiA402Var.pPID_Current_I0x6090))   ||      \
       (PID4671.Velocity_P!=*(pAxis->CiA402Var.pPIN_Velocity_P0x6090))  ||  (PID4671.Velocity_I!=*(pAxis->CiA402Var.pPIN_Velocity_I0x6090)) ||      \
       (PID4671.Position_P!=*(pAxis->CiA402Var.pPID_Position_P0x6090))  ||  (PID4671.Position_I!=*(pAxis->CiA402Var.pPID_Position_I0x6090)))
	{
        if(*(pAxis->CiA402Var.pModesOfOperationDisplay0x6061) == 20)
        {
            PID4671.Current_P=*(pAxis->CiA402Var.pPID_Current_P0x6090);
            PID4671.Current_I=*(pAxis->CiA402Var.pPID_Current_I0x6090);
            PID4671.Velocity_P=*(pAxis->CiA402Var.pPIN_Velocity_P0x6090);
            PID4671.Velocity_I=*(pAxis->CiA402Var.pPIN_Velocity_I0x6090);
            PID4671.Position_P=*(pAxis->CiA402Var.pPID_Position_P0x6090);
            PID4671.Position_I=*(pAxis->CiA402Var.pPID_Position_I0x6090);
            PID_FLAG=1;
        }
        else
        {
            *(pAxis->CiA402Var.pPID_Current_P0x6090) = PID4671.Current_P;
            *(pAxis->CiA402Var.pPID_Current_I0x6090) = PID4671.Current_I;
            *(pAxis->CiA402Var.pPIN_Velocity_P0x6090)= PID4671.Velocity_P;
            *(pAxis->CiA402Var.pPIN_Velocity_I0x6090)= PID4671.Velocity_I;
            *(pAxis->CiA402Var.pPID_Position_P0x6090)= PID4671.Position_P;  
            *(pAxis->CiA402Var.pPID_Position_I0x6090)= PID4671.Position_I;
        }
	}
	if(PID_FLAG)
	{
		PID_FLAG=0;
		PID[0]=*(pAxis->CiA402Var.pPID_Current_P0x6090);
		
		PID[1]=*(pAxis->CiA402Var.pPID_Current_I0x6090);
		
		PID[2]=*(pAxis->CiA402Var.pPIN_Velocity_P0x6090);
		
		PID[3]=*(pAxis->CiA402Var.pPIN_Velocity_I0x6090);
		
		PID[4]=*(pAxis->CiA402Var.pPID_Position_P0x6090);
		
		PID[5]=55;
//		Fmc_PID_Write();
		set_4671PID();
	}           
    
/************************相电流信号采集 及 HALL信号采集 ****************************************/ 
   
    //每200ms检测一次HALL
    if(ENC2_counter>200)
    {    
        ENC2_counter=0;
        //HALL
        PhaseMissing.HALL_UVW = ((tmc4671_readInt(0,TMC4671_INPUTS_RAW) >> 8) & 0x07);
        // 111  ->  WVU 
        PhaseMissing.HALL_U = (PhaseMissing.HALL_UVW>>0) & 0x01;
        PhaseMissing.HALL_V = (PhaseMissing.HALL_UVW>>1) & 0x01;
        PhaseMissing.HALL_W = (PhaseMissing.HALL_UVW>>2) & 0x01;
        
        if((PhaseMissing.HALL_UVW==0) || (PhaseMissing.HALL_UVW==7))
        {
            ErrorMessage.HALLError = 1<<27;        
        }    
        *(pAxis->CiA402Var.pAnalogInput0x6891)=(PhaseMissing.HALL_U<<0) |  (PhaseMissing.HALL_V<<1) | (PhaseMissing.HALL_W<<2);
    }  
    
/**     
    //相电流
    if((abs(*(pAxis->CiA402Var.pVelocityActualValue0x606C))>200) )      
    {
        tmc4671_writeInt(0, TMC4671_INTERIM_ADDR, 8);        
        PhaseMissing.PhaseCurrent_U =(int16_t) (tmc4671_readRegister16BitValue(0, TMC4671_INTERIM_DATA, BIT_0_TO_15));
        PhaseMissing.PhaseCurrent_W =(int16_t)(tmc4671_readRegister16BitValue(0, TMC4671_INTERIM_DATA, BIT_16_TO_31));
        
        tmc4671_writeInt(0, TMC4671_INTERIM_ADDR, 9);          
        PhaseMissing.PhaseCurrent_V =(int16_t) (tmc4671_readRegister16BitValue(0, TMC4671_INTERIM_DATA, BIT_0_TO_15));
 
        PhaseMissing.Count++;
        
        PhaseMissing.PhaseCurrentGroup_U[0] += abs(PhaseMissing.PhaseCurrent_U);
        PhaseMissing.PhaseCurrentGroup_V[0] += abs(PhaseMissing.PhaseCurrent_V);       
        PhaseMissing.PhaseCurrentGroup_W[0] += abs(PhaseMissing.PhaseCurrent_W);        
        

        if(PhaseMissing.Count>=100)
        {  
            PhaseMissing.PhaseCurrentSum_U = PhaseMissing.PhaseCurrentGroup_U[0];  
            PhaseMissing.PhaseCurrentSum_V = PhaseMissing.PhaseCurrentGroup_V[0]; 
            PhaseMissing.PhaseCurrentSum_W = PhaseMissing.PhaseCurrentGroup_W[0]; 
            
            PhaseMissing.Result_MAX = max_of_three(PhaseMissing.PhaseCurrentSum_U,PhaseMissing.PhaseCurrentSum_V,PhaseMissing.PhaseCurrentSum_W);
            PhaseMissing.Result_MIN = min_of_three(PhaseMissing.PhaseCurrentSum_U,PhaseMissing.PhaseCurrentSum_V,PhaseMissing.PhaseCurrentSum_W);        
            PhaseMissing.Result = PhaseMissing.Result_MIN/PhaseMissing.Result_MAX *100;            
            //计算结束清零
            PhaseMissing.PhaseCurrentGroup_U[0] = 0 ;
            PhaseMissing.PhaseCurrentGroup_V[0] = 0;       
            PhaseMissing.PhaseCurrentGroup_W[0] = 0;           
            PhaseMissing.Count=0;
            PhaseMissing.ResultFlag=1;
        }        
    }
    else
    {
        PhaseMissing.PhaseCurrentGroup_U[0] = 0 ;
        PhaseMissing.PhaseCurrentGroup_V[0] = 0;       
        PhaseMissing.PhaseCurrentGroup_W[0] = 0;          
        PhaseMissing.Count=0;        
    }
    //判断缺相的标准：1、使能状态下，电流模式下，不响应目标电流；2、积分法计算结果小于10 
    //这里考虑如果缺相导致无法驱动时，上位机会有其他报错可以判断，暂不添加避免误判。
    if(PhaseMissing.ResultFlag) //计算得到结果
    {
         PhaseMissing.ResultFlag=0;  
         if(PhaseMissing.Result<15)
         {
             ErrorMessage.MotorVUWError = 1<<30;
         }
         else
         {
             ErrorMessage.MotorVUWError = 0;
         }
    }   

*/   
    
/************************心跳信号****************************************/ 

    if(Heartbeat.Heartbeat_count>100)
    {
        Heartbeat.Heartbeat_count = 0;   
        Heartbeat.Heartbeat_Flag=!Heartbeat.Heartbeat_Flag;
    }
     *(pAxis->CiA402Var.pHeartbeat0x68AA) = Heartbeat.Heartbeat_Flag;
    
/************************状态反馈****************************************/ 
	if(nAlStatus != STATE_OP)
	{     
		stop=1;
		Servo_Ready_Flag=0;
        ErrorMessage.EtherCATOPError =1 ;        
	}
    else
    {
        ErrorMessage.EtherCATOPError =0 ; 
    } 

	*(pAxis->CiA402Var.pDigitalInput0x6892)=Task_Key();    
 	*(pAxis->CiA402Var.pErrorMessage0x6893)= ErrorMessage.PEIDeviationError     | ErrorMessage.MotorSPICommunication   | ErrorMessage.ENCError | ErrorMessage.UPSError  \
                                           | ErrorMessage.XLControlError   \
                                           | ErrorMessage.SpringFractureError   | ErrorMessage.IncrementalEncoderError | ErrorMessage.HALLError  \
                                           | ErrorMessage.HostHeartbeatError    | ErrorMessage.EtherCATOPError         | ErrorMessage.MotorVUWError \
                                           | ErrorMessage.MotorDriveError;	
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    CiA402-Statemachine
        This function handles the state machine for devices using the CiA402 profile.
        called cyclic from MainLoop()
        All described transition numbers are referring to the document
        "ETG Implementation Guideline for the CiA402 Axis Profile" located on the EtherCAT.org download section

*////////////////////////////////////////////////////////////////////////////////////////
void CiA402_StateMachine(CiA402_AXIS_T *pAxis)
{
	UINT16 StatusWord = 0;
	UINT16 ControlWord = 0;

	/* Skip unavailable axis */
	if(pAxis->bAxisIsActive == 0)
	{
		return;
	}

	StatusWord = *(pAxis->CiA402Var.pStatusword0x6041);
	ControlWord = *(pAxis->CiA402Var.pControlword0x6040);

	/* Clear statusword state and controlword processed complete bits */
	StatusWord &= ~(STATUSWORD_STATE_MASK | STATUSWORD_REMOTE);

	/* Skip state state transition if the previous transition is pending */
	if(pAxis->u16PendingOptionCode!= 0x0)
	{
		return;
	}
		
	/* Skip transition 1 and 2 if Slave has been under OP mode */
	if(pAxis->i16State < STATE_READY_TO_SWITCH_ON && nAlStatus == STATE_OP)
	{
		pAxis->i16State = STATE_READY_TO_SWITCH_ON;
	}
				
	switch(pAxis->i16State)
	{
	case STATE_NOT_READY_TO_SWITCH_ON:
		StatusWord |= (STATUSWORD_STATE_NOTREADYTOSWITCHON);
		if(nAlStatus == STATE_OP)
		{
			// Automatic transition -> Communication shall be activated
			pAxis->i16State = STATE_SWITCH_ON_DISABLED; // Transition 1
			DBG_PRINT("Transition 1, NOT_READY_TO_SWITCH_ON->SWITCH_ON_DISABLED\r\n");
		}
		else
		{
			/* CiA402 statemachine shall stay in "STATE_NOT_READY_TO_SWITCH_ON" if EtherCAT state is not OP. */
			pAxis->i16State = STATE_NOT_READY_TO_SWITCH_ON; // stay in current state
		}
		break;
			
	case STATE_SWITCH_ON_DISABLED:
		StatusWord |= (STATUSWORD_STATE_SWITCHEDONDISABLED);
		if ((ControlWord & CONTROLWORD_COMMAND_SHUTDOWN_MASK) == CONTROLWORD_COMMAND_SHUTDOWN)
		{
			pAxis->i16State = STATE_READY_TO_SWITCH_ON; // Transition 2
			DBG_PRINT("Transition 2, SWITCH_ON_DISABLED->READY_TO_SWITCH_ON\r\n");
		}
		break;
			
	case STATE_READY_TO_SWITCH_ON:
		StatusWord |= (STATUSWORD_STATE_READYTOSWITCHON);
		if (((ControlWord & CONTROLWORD_COMMAND_QUICKSTOP_MASK) == CONTROLWORD_COMMAND_QUICKSTOP)
			|| ((ControlWord & CONTROLWORD_COMMAND_DISABLEVOLTAGE_MASK) == CONTROLWORD_COMMAND_DISABLEVOLTAGE))
		{
			pAxis->i16State = STATE_SWITCH_ON_DISABLED; // Transition 7
			if (nAlStatus != STATE_OP)
			{
				DBG_PRINT("Transition 7, READY_TO_SWITCH_ON->SWITCH_ON_DISABLED(0x%04x)\r\n", ControlWord);							
			}
		}
		else if (((ControlWord & CONTROLWORD_COMMAND_SWITCHON_MASK) == CONTROLWORD_COMMAND_SWITCHON) &&
							((ControlWord & CONTROLWORD_COMMAND_SWITCHON_ENABLEOPERATION_MASK) != CONTROLWORD_COMMAND_SWITCHON_ENABLEOPERATION))
		{
			pAxis->i16State = STATE_SWITCHED_ON;        // Transition 3
			DBG_PRINT("Transition 3, READY_TO_SWITCH_ON->SWITCHED_ON\r\n");
		}
		break;
			
	case STATE_SWITCHED_ON:
		StatusWord |= (STATUSWORD_STATE_SWITCHEDON);
		if ((ControlWord & CONTROLWORD_COMMAND_SHUTDOWN_MASK) == CONTROLWORD_COMMAND_SHUTDOWN)
		{
			pAxis->i16State = STATE_READY_TO_SWITCH_ON; // Transition 6
			DBG_PRINT("Transition 6, SWITCHED_ON->READY_TO_SWITCH_ON\r\n");
		} 
		else if (((ControlWord & CONTROLWORD_COMMAND_QUICKSTOP_MASK) == CONTROLWORD_COMMAND_QUICKSTOP
						|| (ControlWord & CONTROLWORD_COMMAND_DISABLEVOLTAGE_MASK) == CONTROLWORD_COMMAND_DISABLEVOLTAGE))
		{
			pAxis->i16State = STATE_SWITCH_ON_DISABLED; // Transition 10
			DBG_PRINT("Transition 10, SWITCHED_ON->SWITCH_ON_DISABLED\r\n");
		}
		else if ((ControlWord & CONTROLWORD_COMMAND_ENABLEOPERATION_MASK) == CONTROLWORD_COMMAND_ENABLEOPERATION)
		{
			/* Start motor */
			
			Servo_Ready_Flag=1;
			stop=0;
			pAxis->i16State = STATE_OPERATION_ENABLED;  // Transition 4
			DBG_PRINT("Transition 4, SWITCHED_ON->OPERATION_ENABLED\r\n");
			//The Axis function shall be enabled and all internal set-points cleared.
		}
		break;
			
	case STATE_OPERATION_ENABLED:
		StatusWord |= (STATUSWORD_STATE_OPERATIONENABLED);
		if ((ControlWord & CONTROLWORD_COMMAND_DISABLEOPERATION_MASK) == CONTROLWORD_COMMAND_DISABLEOPERATION)
		{
			if(*(pAxis->CiA402Var.pDisableOperationOptionCode0x605C)!= DISABLE_DRIVE)
			{
				/* Disable operation pending */
				pAxis->u16PendingOptionCode = 0x605C; //STATE_TRANSITION (STATE_OPERATION_ENABLED,STATE_SWITCHED_ON);
//				return;
			}
			pAxis->i16State = STATE_SWITCHED_ON;           // Transition 5
			DBG_PRINT("Transition 5, OPERATION_ENABLED->SWITCHED_ON\r\n");


			/* Stop motor */			
			Servo_Ready_Flag=0;
			stop=1;
		}
		else if ((ControlWord & CONTROLWORD_COMMAND_QUICKSTOP_MASK) == CONTROLWORD_COMMAND_QUICKSTOP)
		{
			/* Stop motor */			
			Servo_Ready_Flag=0;
			stop=1;
            
			pAxis->i16State = STATE_QUICK_STOP_ACTIVE;  // Transition 11
			DBG_PRINT("Transition 11, OPERATION_ENABLED->QUICK_STOP_ACTIVE\r\n");
		}
		else if ((ControlWord & CONTROLWORD_COMMAND_SHUTDOWN_MASK) == CONTROLWORD_COMMAND_SHUTDOWN)
		{
			if (*(pAxis->CiA402Var.pShutdownOptionCode0x605B) != DISABLE_DRIVE)
			{
				/* Shutdown operation required */
				pAxis->u16PendingOptionCode = 0x605B; //STATE_TRANSITION (STATE_OPERATION_ENABLED,STATE_READY_TO_SWITCH_ON);
//				return;
			}
			pAxis->i16State = STATE_READY_TO_SWITCH_ON; // Transition 8
			DBG_PRINT("Transition 8, OPERATION_ENABLED->READY_TO_SWITCH_ON\r\n");
				

			/* Stop motor */			
			Servo_Ready_Flag=0;
			stop=1;
		}
		else if ((ControlWord & CONTROLWORD_COMMAND_DISABLEVOLTAGE_MASK) == CONTROLWORD_COMMAND_DISABLEVOLTAGE)
		{
			pAxis->i16State = STATE_SWITCH_ON_DISABLED; // Transition 9
			DBG_PRINT("Transition 9, OPERATION_ENABLED->SWITCH_ON_DISABLED\r\n");
				

			/* Stop motor */			
			Servo_Ready_Flag=0;
			stop=1;
		}
		break;
			
	case STATE_QUICK_STOP_ACTIVE:
		StatusWord |= STATUSWORD_STATE_QUICKSTOPACTIVE;
		if ((*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) != DISABLE_DRIVE) &&
				((*(pAxis->CiA402Var.pStatusword0x6041) & STATUSWORD_STATE_MASK)!= STATUSWORD_STATE_QUICKSTOPACTIVE))
		{
			/* Only execute quick stop action in state transition 11 */
			pAxis->u16PendingOptionCode = 0x605A;//STATE_TRANSITION (STATE_OPERATION_ENABLED,STATE_QUICK_STOP_ACTIVE);
			DBG_PRINT("Set u16PendingOptionCode = 0x605A\r\n");
			return;
		}

		if ((ControlWord & CONTROLWORD_COMMAND_DISABLEVOLTAGE_MASK) == CONTROLWORD_COMMAND_DISABLEVOLTAGE)
		{
			pAxis->i16State = STATE_SWITCH_ON_DISABLED; // Transition 12
			DBG_PRINT("Transition 12, QUICK_STOP_ACTIVE->SWITCH_ON_DISABLED\r\n");
		}
		/* NOTE: it is not recommend to support transition 16 */
		break;
			
	case STATE_FAULT_REACTION_ACTIVE:
		StatusWord |= (STATUSWORD_STATE_FAULTREACTIONACTIVE);
		if (*(pAxis->CiA402Var.pFaultReactionOptionCode0x605E)!= DISABLE_DRIVE)
		{
			/* Fault reaction pending */
			pAxis->u16PendingOptionCode = 0x605E;
			return;
		}

		// Automatic transition
		pAxis->i16State = STATE_FAULT;// Transition 14
//			DBG_PRINT("Transition 14, FAULT_REACTION_ACTIVE->FAULT\r\n");
		break;
			
	case STATE_FAULT:
		StatusWord |= (STATUSWORD_STATE_FAULT);

		if ((ControlWord & CONTROLWORD_COMMAND_FAULTRESET_MASK) == CONTROLWORD_COMMAND_FAULTRESET)
		{
			/* Clear motor driver fault until state change */
//			if (MC_GetState(pAxis->pMC) == MCSTE_FAULT)
//			{
//				MC_FaultReset(pAxis->pMC);
//				return;
//			}
			*(pAxis->CiA402Var.pErrorCode0x603F) = 0;
			DBG_PRINT("Fault reset\r\n");				

			pAxis->i16State = STATE_SWITCH_ON_DISABLED;// Transition 15
			DBG_PRINT("Transition 15, FAULT->SWITCH_ON_DISABLED\r\n");							
		}
		break;

	default:    //the sate variable is set to in invalid value => rest Axis
		StatusWord = (STATUSWORD_STATE_NOTREADYTOSWITCHON);
		pAxis->i16State = STATE_NOT_READY_TO_SWITCH_ON;
		break;

	}// switch(current state)

	/* Update operational functions (the low level power supply is always TRUE */
	switch(pAxis->i16State)
	{
	case STATE_NOT_READY_TO_SWITCH_ON:
	case STATE_SWITCH_ON_DISABLED:
	case STATE_READY_TO_SWITCH_ON:
		pAxis->bBrakeApplied = TRUE;
		pAxis->bHighLevelPowerApplied =  FALSE;
		pAxis->bAxisFunctionEnabled = FALSE;
		pAxis->bConfigurationAllowed = TRUE;
		break;
	case STATE_SWITCHED_ON:
		pAxis->bBrakeApplied = TRUE;
		pAxis->bHighLevelPowerApplied =  TRUE;
		pAxis->bAxisFunctionEnabled = FALSE;
		pAxis->bConfigurationAllowed = TRUE;
		break;
	case STATE_OPERATION_ENABLED:
	case STATE_QUICK_STOP_ACTIVE:
	case STATE_FAULT_REACTION_ACTIVE:
		pAxis->bBrakeApplied = FALSE;
		pAxis->bHighLevelPowerApplied =  TRUE;
		pAxis->bAxisFunctionEnabled = TRUE;
		pAxis->bConfigurationAllowed = FALSE;
		break;
	case STATE_FAULT:
		pAxis->bBrakeApplied = TRUE;
		pAxis->bHighLevelPowerApplied =  FALSE;
		pAxis->bAxisFunctionEnabled = FALSE;
		pAxis->bConfigurationAllowed = TRUE;
		break;
	default:
		pAxis->bBrakeApplied = TRUE;
		pAxis->bHighLevelPowerApplied =  FALSE;
		pAxis->bAxisFunctionEnabled = FALSE;
		pAxis->bConfigurationAllowed = TRUE;
		break;
	}

	if (pAxis->bHighLevelPowerApplied == TRUE)
	{
		StatusWord |= STATUSWORD_VOLTAGE_ENABLED;
	}
	else
	{
		StatusWord &= ~STATUSWORD_VOLTAGE_ENABLED;
	}
		
	/* State transition finished set controlword complete bit and update status object 0x6041 */
	*(pAxis->CiA402Var.pStatusword0x6041) = (StatusWord | STATUSWORD_REMOTE);

	/* Motor control driver */
	

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param ErrorCode

 \brief    CiA402_LocalError
 \brief this function is called if an error was detected
*////////////////////////////////////////////////////////////////////////////////////////
void CiA402_LocalError(UINT16 ErrorCode)
{
	CiA402_AXIS_T *pAxis = &CiA402Axis;

	if(pAxis->bAxisIsActive)
	{
		pAxis->i16State = STATE_FAULT_REACTION_ACTIVE;
		*(pAxis->CiA402Var.pErrorCode0x603F) = ErrorCode;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return TRUE if moving on predefined ramp is finished

 \brief    CiA402-TransitionAction
 \brief this function shall calculate the desired Axis input values to move on a predefined ramp
 \brief if the ramp is finished return TRUE
*////////////////////////////////////////////////////////////////////////////////////////
BOOL CiA402_TransitionAction(INT16 Characteristic, CiA402_AXIS_T *pAxis)
{
	INT32 tmp32;
	
	switch(Characteristic)
	{
	case DISABLE_DRIVE:
//		tmp32 = MC_GetState(pAxis->pMC);
//		if (tmp32 == MCSTE_RUN)
//		{
//			MC_StopMotor(pAxis->pMC);
//			return FALSE;
//		}
//		DBG_PRINT("DISABLE_DRIVE\r\n");			
		return TRUE;
		
	case SLOW_DOWN_RAMP:
//		tmp32 = MC_GetState(pAxis->pMC);
//		if (tmp32 == MCSTE_RUN)
//		{
//			MC_StopMotor(pAxis->pMC);
//			return FALSE;
//		}
//		DBG_PRINT("SLOW_DOWN_RAMP\r\n");			
		return TRUE;
		
	case QUICKSTOP_RAMP:
//		tmp32 = MC_GetState(pAxis->pMC);
//		if (tmp32 == MCSTE_RUN)
//		{
//			MC_StopMotor(pAxis->pMC);
//			return FALSE;
//		}
//		DBG_PRINT("QUICKSTOP_RAMP\r\n");			
		return TRUE;
		
	case STOP_ON_CURRENT_LIMIT:
//		tmp32 = MC_GetState(pAxis->pMC);
//		if (tmp32 == MCSTE_RUN)
//		{
//			MC_StopMotor(pAxis->pMC);
//			return FALSE;
//		}
//		DBG_PRINT("STOP_ON_CURRENT_LIMIT\r\n");
		return TRUE;
		
	case STOP_ON_VOLTAGE_LIMIT:
//		tmp32 = MC_GetState(pAxis->pMC);
//		if (tmp32 == MCSTE_RUN)
//		{
//			MC_StopMotor(pAxis->pMC);
//			return FALSE;
//		}
//		DBG_PRINT("STOP_ON_VOLTAGE_LIMIT\r\n");			
		return TRUE;			
		
	default:
		break;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    CiA402-Application
 \brief check if a state transition is pending and pass desired ramp-code to CiA402TransitionAction()
 \brief if this functions returns true the state transition is finished.
*////////////////////////////////////////////////////////////////////////////////////////
void CiA402_Application(CiA402_AXIS_T *pAxis)
{
	/*clear "Drive follows the command value" flag if the target values from the master overwritten by the local application*/
	if(pAxis->u16PendingOptionCode != 0 &&
			(*(pAxis->CiA402Var.pModesOfOperationDisplay0x6061) == CYCLIC_SYNC_POSITION_MODE ||
			 *(pAxis->CiA402Var.pModesOfOperationDisplay0x6061) == CYCLIC_SYNC_VELOCITY_MODE))
	{
		*(pAxis->CiA402Var.pStatusword0x6041) &= ~ STATUSWORD_DRIVE_FOLLOWS_COMMAND;
	}
  else
	{
		*(pAxis->CiA402Var.pStatusword0x6041) |= STATUSWORD_DRIVE_FOLLOWS_COMMAND;
	}

	switch(pAxis->u16PendingOptionCode)
	{
	case 0x605A:
		/*state transition 11 is pending analyse shutdown option code (0x605A)*/
		{
			UINT16 ramp = *(pAxis->CiA402Var.pQuickStopOptionCode0x605A);
			/*masked and execute specified quick stop ramp characteristic */
			if(*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) > 4 && *(pAxis->CiA402Var.pQuickStopOptionCode0x605A) <9)
			{
				if(*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) == 5)
					ramp = 1;
				if(*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) == 6)
					ramp = 2;
				if(*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) == 7)
					ramp = 3;
				if(*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) == 8)
					ramp = 4;
			}

			if(CiA402_TransitionAction(ramp,pAxis))
			{
				/*quick stop ramp is finished complete state transition*/
				pAxis->u16PendingOptionCode = 0x0;
				if(*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) > 0 && *(pAxis->CiA402Var.pQuickStopOptionCode0x605A) < 5)
				{
					pAxis->i16State = STATE_SWITCH_ON_DISABLED;    //continue state transition 12
					DBG_PRINT("Transition 12(pending), QUICK_STOP_ACTIVE->SWITCH_ON_DISABLED\r\n");
				}
				else if(*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) > 4 && *(pAxis->CiA402Var.pQuickStopOptionCode0x605A) < 9)
					*(pAxis->CiA402Var.pStatusword0x6041) |= STATUSWORD_TARGET_REACHED;
			}
		}
		break;
	case 0x605B:
		/*state transition 8 is pending analyse shutdown option code (0x605B)*/
		{
			if(CiA402_TransitionAction(*(pAxis->CiA402Var.pShutdownOptionCode0x605B),pAxis))
			{
				/*shutdown ramp is finished complete state transition*/
				pAxis->u16PendingOptionCode = 0x0;
				pAxis->i16State = STATE_READY_TO_SWITCH_ON;    //continue state transition 8
				DBG_PRINT("Transition 8(pending), OPERATION_ENABLED->READY_TO_SWITCH_ON\r\n");							
			}
		}
		break;
	case 0x605C:
		/*state transition 5 is pending analyse Disable operation option code (0x605C)*/
		{
			if(CiA402_TransitionAction(*(pAxis->CiA402Var.pDisableOperationOptionCode0x605C),pAxis))
			{
				/*disable operation ramp is finished complete state transition*/
				pAxis->u16PendingOptionCode = 0x0;
				pAxis->i16State = STATE_SWITCHED_ON;    //continue state transition 5
				DBG_PRINT("Transition 5(pending), OPERATION_ENABLED->SWITCHED_ON\r\n");
			}
		}
		break;
	case 0x605E:
		/*state transition 14 is pending analyse Fault reaction option code (0x605E)*/
		{
			if(CiA402_TransitionAction(*(pAxis->CiA402Var.pFaultReactionOptionCode0x605E),pAxis))
			{
				/*fault reaction ramp is finished complete state transition*/
				pAxis->u16PendingOptionCode = 0x0;
				pAxis->i16State = STATE_FAULT;    //continue state transition 14
//			DBG_PRINT("Transition 4(pending), FAULT_REACTION_ACTIVE->FAULT\r\n");
			}
		}
		break;
	default:
		//pending transition code is invalid => values from the master are used
		if (*(pAxis->CiA402Var.pModesOfOperationDisplay0x6061) != HOMING_MODE)
		{
			*(pAxis->CiA402Var.pStatusword0x6041) |= STATUSWORD_DRIVE_FOLLOWS_COMMAND;
		}
		break;
	}
		
#if 1		
    if(bDcSyncActive
        && (pAxis->u32CycleTime != 0)
        && ((*(pAxis->CiA402Var.pSupportedDriveModes0x6502) >> (*(pAxis->CiA402Var.pModesOfOperation0x6060) - 1)) & 0x1)) //Mode of Operation (0x6060) - 1 specifies the Bit within Supported Drive Modes (0x6502)
#else
    if( ((*(pAxis->CiA402Var.pSupportedDriveModes0x6502) >> (*(pAxis->CiA402Var.pModesOfOperation0x6060) - 1)) & 0x1)) //Mode of Operation (0x6060) - 1 specifies the Bit within Supported Drive Modes (0x6502)
#endif		
    {

    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    
*////////////////////////////////////////////////////////////////////////////////////////
UINT8 ModeRead(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	switch (index)
	{
	case 0x6060:
		pData[0] = ModesOfOperation0x6060;
		break;
	
	case 0x6061:
		pData[0] = ModesOfOperationDisplay0x6061;		
		break;	
	}

	return ABORT_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    
*////////////////////////////////////////////////////////////////////////////////////////
UINT8 ModeWrite(UINT16 index, UINT8 subindex, UINT32 dataSize, UINT16 MBXMEM * pData, UINT8 bCompleteAccess)
{
	switch (index)
	{
	case 0x6060:
		ModesOfOperation0x6060 = pData[0];
		break;
	
	}
	
	return ABORT_NOERROR;
}

/*-----------------------------------------------------------------------------------------
------
------    generic functions
------
-----------------------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when an error state was acknowledged by the master

*////////////////////////////////////////////////////////////////////////////////////////

void	APPL_AckErrorInd(UINT16 stateTrans)
{

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from INIT to PREOP when
             all general settings were checked to start the mailbox handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case this function will be called cyclically
            until a value unequal NOERROR_INWORK is returned

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from PREEOP to INIT
             to stop the mailbox handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pIntMask    pointer to the AL Event Mask which will be written to the AL event Mask
                        register (0x204) when this function is succeeded. The event mask can be adapted
                        in this function
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from PREOP to SAFEOP when
           all general settings were checked to start the input handler. This function
           informs the application about the state transition, the application can refuse
           the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartInputHandler(UINT16 *pIntMask)
{
	CiA402_AXIS_T *pAxis = &CiA402Axis;
	
	UINT32 Sync0CycleTime = 0;

	HW_EscReadDWord(Sync0CycleTime, ESC_DC_SYNC0_CYCLETIME_OFFSET);
	Sync0CycleTime = SWAPDWORD(Sync0CycleTime);

	/*Init CiA402 structure if the device is in SM Sync mode
		the CiA402 structure will be Initialized after calculation of the Cycle time*/
	if(bDcSyncActive == TRUE)
	{
		Sync0CycleTime = Sync0CycleTime / 1000; //get cycle time in us
		if(pAxis->bAxisIsActive)
			pAxis->u32CycleTime = Sync0CycleTime;
	}
		
	return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from SAFEOP to PREEOP
             to stop the input handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopInputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from SAFEOP to OP when
             all general settings were checked to start the output handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartOutputHandler(void)
{
	CiA402_AXIS_T *pAxis = &CiA402Axis;
	
//	if(pAxis->bAxisIsActive)
//	{
//		if (MC_GetFaultStatus(pAxis->pMC) != MCFS_NO_ERR || *(pAxis->CiA402Var.pErrorCode0x603F) != 0)
//		{
//			MC_FaultReset(pAxis->pMC);
//			*(pAxis->CiA402Var.pErrorCode0x603F) = 0;
//			pAxis->i16State = STATE_SWITCH_ON_DISABLED;
//			DBG_PRINT("Clear error due to ESC state transition from SAFEOP to OP\r\n");
//		}	
//	}
		
	return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from OP to SAFEOP
             to stop the output handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopOutputHandler(void)
{
	CiA402_AXIS_T *pAxis = &CiA402Axis;
	
//	if(pAxis->bAxisIsActive)
//	{
//		if (MC_GetState(pAxis->pMC) != MCSTE_IDLE)
//		{
//			MC_StopMotor(pAxis->pMC);
//			DBG_PRINT("Stop motor due to ESC state transition from OP to SAFEOP\r\n");
//		}
//		CiA402_LocalError(ERROR_COMMUNICATION);	
//	}
		
	return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0(ALSTATUSCODE_NOERROR), NOERROR_INWORK
\param      pInputSize  pointer to save the input process data length
\param      pOutputSize  pointer to save the output process data length

\brief    This function calculates the process data sizes from the actual SM-PDO-Assign
            and PDO mapping
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GenerateMapping(UINT16 *pInputSize,UINT16 *pOutputSize)
{
	UINT16 result = ALSTATUSCODE_NOERROR;
	UINT16 PDOAssignEntryCnt = 0;
	UINT16 u16cnt = 0;
	UINT16 InputSize = 0;
	UINT16 OutputSize = 0;
	CiA402_AXIS_T *pAxis = &CiA402Axis;
	UINT32 *pEntries;
	
	pAxis->bAxisIsActive = TRUE;

	/*Scan object 0x1C12 RXPDO assign*/
	for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
	{
		switch ((sRxPDOassign.aEntries[PDOAssignEntryCnt] & 0x000F))    //mask Axis type (supported modes)
		{
		case 0:
			/*drive mode supported    csv(cyclic sync velocity) : bit 8
                                csp (cyclic sync position) : bit 7*/
			*(pAxis->CiA402Var.pSupportedDriveModes0x6502) = 0x180;
			pEntries = &(pAxis->CiA402Var.pRxPdoMappingCspCsv0x1600->SI1);
			for(u16cnt =0 ; u16cnt < pAxis->CiA402Var.pRxPdoMappingCspCsv0x1600->u16SubIndex0;u16cnt++)
			{
				OutputSize +=(UINT16)(pEntries[u16cnt] & 0xFF);
			}
			break;
		case 1:
			/*drive mode supported    csp (cyclic sync position) : bit 7*/
			*(pAxis->CiA402Var.pSupportedDriveModes0x6502) = 0x80;
			pEntries = &(pAxis->CiA402Var.pRxPdoMappingCsp0x1601->SI1);		
			for(u16cnt =0 ; u16cnt < pAxis->CiA402Var.pRxPdoMappingCsp0x1601->u16SubIndex0;u16cnt++)
			{
				OutputSize +=(UINT16)(pEntries[u16cnt] & 0xFF);
			}
			break;
		case 2:
			/*drive mode supported    csv(cyclic sync velocity) : bit 8*/
			*(pAxis->CiA402Var.pSupportedDriveModes0x6502) = 0x100;
			pEntries = &(pAxis->CiA402Var.pRxPdoMappingCsv0x1602->SI1);			
			for(u16cnt =0 ; u16cnt < pAxis->CiA402Var.pRxPdoMappingCsv0x1602->u16SubIndex0;u16cnt++)
			{
				OutputSize += (UINT16)(pEntries[u16cnt] & 0xFF);;
			}
			break;
		}
	}
	OutputSize = OutputSize >> 3;

	if(result == 0)
	{
		/*Scan Object 0x1C13 TXPDO assign*/
		for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
		{
			switch ((sTxPDOassign.aEntries[PDOAssignEntryCnt] & 0x000F))    //mask Axis type (supported modes)
			{
			case 0: /*csp/csv*/
				pEntries = &(pAxis->CiA402Var.pTxPdoMappingCspCsv0x1A00->SI1);				
				for(u16cnt =0 ; u16cnt < pAxis->CiA402Var.pTxPdoMappingCspCsv0x1A00->u16SubIndex0;u16cnt++)
				{
					InputSize +=(UINT16)(pEntries[u16cnt] & 0xFF);
				}
				break;
			case 1: /*csp*/
				pEntries = &(pAxis->CiA402Var.pTxPdoMappingCsp0x1A01->SI1);				
				for(u16cnt =0 ; u16cnt < pAxis->CiA402Var.pTxPdoMappingCsp0x1A01->u16SubIndex0;u16cnt++)
				{
					InputSize +=(UINT16)(pEntries[u16cnt] & 0xFF);
				}
				break;
			case 2: /*csv*/
				pEntries = &(pAxis->CiA402Var.pTxPdoMappingCsv0x1A02->SI1);				
				for(u16cnt =0 ; u16cnt < pAxis->CiA402Var.pTxPdoMappingCsv0x1A02->u16SubIndex0;u16cnt++)
				{
					InputSize +=(UINT16)(pEntries[u16cnt] & 0xFF);
				}
				break;
			}
		}
		InputSize = InputSize >> 3;
	}

	*pInputSize = InputSize;
	*pOutputSize = OutputSize;
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to input process data

\brief      This function will copies the inputs from the local memory to the ESC memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_InputMapping(UINT16* pData)
{
	UINT16 j = 0;
	UINT8 *pTmpData = (UINT8 *)pData;
	CiA402_AXIS_T *pAxis = &CiA402Axis;
	
	for (j = 0; j < sTxPDOassign.u16SubIndex0; j++)
	{
		switch ((sTxPDOassign.aEntries[j]& 0x000F))
		{
		case 0://copy csp/csv TxPDO entries
			{
				TCiA402PDO1A00 *pInputs = (TCiA402PDO1A00 *)pTmpData;
                
                pInputs->ObjStatusword =                    SWAPWORD(*(pAxis->CiA402Var.pStatusword0x6041));
                pInputs->ObjPositionActualValue =           SWAPWORD(*(pAxis->CiA402Var.pPositionActualValue0x6064));
                pInputs->ObjVelocityActualValue =           SWAPWORD(*(pAxis->CiA402Var.pVelocityActualValue0x606C));
                pInputs->ObjTorqueActualValue =             SWAPWORD(*(pAxis->CiA402Var.pTorqueActualValue0x6077));
                pInputs->ObjENC =                           SWAPWORD(*(pAxis->CiA402Var.pENC0x607F));
                pInputs->ObjENCForAxis2 =                   SWAPWORD(*(pAxis->CiA402Var.pENCForAxis20x687F));  
                pInputs->ObjAnalogInput =                   SWAPWORD(*(pAxis->CiA402Var.pAnalogInput0x6891));
                pInputs->ObjDigitalInput =                  SWAPWORD(*(pAxis->CiA402Var.pDigitalInput0x6892));
                pInputs->ObjErrorMessage =                  SWAPWORD(*(pAxis->CiA402Var.pErrorMessage0x6893));
                pInputs->ObjHeartbeat =                     SWAPWORD(*(pAxis->CiA402Var.pHeartbeat0x68AA));
                pInputs->ObjFluxActualValue =               SWAPWORD(*(pAxis->CiA402Var.pFluxActualValue0x68AB));                      
                pInputs->ObjModesOfOperationDisplay =       SWAPWORD(0x00FF & *(pAxis->CiA402Var.pModesOfOperationDisplay0x6061));   				

				/*shift pointer PDO mapping object following*/
				if(j < (sTxPDOassign.u16SubIndex0 - 1))
					pTmpData += SIZEOF(TCiA402PDO1A00);
			}
			break;
		case 1://copy csp TxPDO entries
			{
				TCiA402PDO1A01 *pInputs = (TCiA402PDO1A01 *)pTmpData;

                pInputs->ObjStatusword =                    SWAPWORD(*(pAxis->CiA402Var.pStatusword0x6041));
                pInputs->ObjStatuswordForAxis2 =            SWAPWORD(*(pAxis->CiA402Var.pStatuswordForAxis20x6841));                
                pInputs->ObjPositionActualValue =           SWAPWORD(*(pAxis->CiA402Var.pPositionActualValue0x6064));
                pInputs->ObjVelocityActualValue =           SWAPWORD(*(pAxis->CiA402Var.pVelocityActualValue0x606C));
                pInputs->ObjTorqueActualValue =             SWAPWORD(*(pAxis->CiA402Var.pTorqueActualValue0x6077));
                pInputs->ObjENC =                           SWAPWORD(*(pAxis->CiA402Var.pENC0x607F));
                pInputs->ObjPositionActualValueForAxis2 =   SWAPWORD(*(pAxis->CiA402Var.pPositionActualValueForAxis20x6864));    
                pInputs->ObjVelocityActualValueForAxis2 =   SWAPWORD(*(pAxis->CiA402Var.pVelocityActualValueForAxis20x686C));
                pInputs->ObjTorqueActualValueForAxis2 =     SWAPWORD(*(pAxis->CiA402Var.pTorqueActualValueForAxis20x6877));
                pInputs->ObjENCForAxis2 =                   SWAPWORD(*(pAxis->CiA402Var.pENCForAxis20x687F));
                pInputs->ObjAnalogInput =                   SWAPWORD(*(pAxis->CiA402Var.pAnalogInput0x6891));
                pInputs->ObjDigitalInput =                  SWAPWORD(*(pAxis->CiA402Var.pDigitalInput0x6892));
                pInputs->ObjErrorMessage =                  SWAPWORD(*(pAxis->CiA402Var.pErrorMessage0x6893));
                pInputs->ObjHeartbeat =                     SWAPWORD(*(pAxis->CiA402Var.pHeartbeat0x68AA));
                pInputs->ObjFluxActualValue =               SWAPWORD(*(pAxis->CiA402Var.pFluxActualValue0x68AB));               
                pInputs->ObjFluxActualValueForAxis2 =       SWAPWORD(*(pAxis->CiA402Var.pFluxActualValueForAxis20x68AC));                
                pInputs->ObjModesOfOperationDisplay =       SWAPWORD(0x00FF & *(pAxis->CiA402Var.pModesOfOperationDisplay0x6061));   

				/*shift pointer PDO mapping object following*/
				if(j < (sTxPDOassign.u16SubIndex0 - 1))
					pTmpData += SIZEOF(TCiA402PDO1A01);
			}
			break;
		case 2://copy csv TxPDO entries
			{
				TCiA402PDO1A02 *pInputs = (TCiA402PDO1A02 *)pTmpData;
#if 0
				if (pInputs->ObjStatusWord != SWAPWORD(*(pAxis->CiA402Var.pStatusword0x6041))
				{
					DBG_PRINT("csv TxPDO: objStatusWord=0x%04x\r\n", *(pAxis->CiA402Var.pStatusword0x6041));
				}
							
				if (pInputs->ObjPositionActualValue != SWAPDWORD(*(pAxis->CiA402Var.pPositionActualValue0x6064))
				{
					DBG_PRINT("csv TxPDO: objPositionActualValue=%ld\r\n", *(pAxis->CiA402Var.pPositionActualValue0x6064));									
				}
#endif	                                           
                pInputs->ObjStatusword =                    SWAPWORD(*(pAxis->CiA402Var.pStatusword0x6041));
                pInputs->ObjPositionActualValue =           SWAPWORD(*(pAxis->CiA402Var.pPositionActualValue0x6064));
                pInputs->ObjVelocityActualValue =           SWAPWORD(*(pAxis->CiA402Var.pVelocityActualValue0x606C));
                pInputs->ObjTorqueActualValue =             SWAPWORD(*(pAxis->CiA402Var.pTorqueActualValue0x6077));
                pInputs->ObjENC =                           SWAPWORD(*(pAxis->CiA402Var.pENC0x607F));
                pInputs->ObjPositionActualValueForAxis2 =   SWAPWORD(*(pAxis->CiA402Var.pPositionActualValueForAxis20x6864));    
                pInputs->ObjVelocityActualValueForAxis2 =   SWAPWORD(*(pAxis->CiA402Var.pVelocityActualValueForAxis20x686C));
                pInputs->ObjTorqueActualValueForAxis2 =     SWAPWORD(*(pAxis->CiA402Var.pTorqueActualValueForAxis20x6877));
                pInputs->ObjENCForAxis2 =                   SWAPWORD(*(pAxis->CiA402Var.pENCForAxis20x687F));
                pInputs->ObjPositionActualValueForAxis3 =   SWAPWORD(*(pAxis->CiA402Var.pPositionActualValueForAxis30x7064));
                pInputs->ObjVelocityActualValueForAxis3 =   SWAPWORD(*(pAxis->CiA402Var.pVelocityActualValueForAxis30x706C));                   
                pInputs->ObjTorqueActualValueForAxis3 =     SWAPWORD(*(pAxis->CiA402Var.pTorqueActualValueForAxis30x7077));    
                pInputs->ObjENCForAxis3 =                   SWAPWORD(*(pAxis->CiA402Var.pENCForAxis30x707F));    
                pInputs->ObjAnalogInput =                   SWAPWORD(*(pAxis->CiA402Var.pAnalogInput0x6891));
                pInputs->ObjDigitalInput =                  SWAPWORD(*(pAxis->CiA402Var.pDigitalInput0x6892));
                pInputs->ObjErrorMessage =                  SWAPWORD(*(pAxis->CiA402Var.pErrorMessage0x6893));
                pInputs->ObjHeartbeat =                     SWAPWORD(*(pAxis->CiA402Var.pHeartbeat0x68AA));
                pInputs->ObjModesOfOperationDisplay =       SWAPWORD(0x00FF & *(pAxis->CiA402Var.pModesOfOperationDisplay0x6061));   
           
				/*shift pointer PDO mapping object following*/
				if(j < (sTxPDOassign.u16SubIndex0 - 1))
					pTmpData += SIZEOF(TCiA402PDO1A02);
			}
			break;
		}//switch TXPDO entry
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to output process data

\brief    This function will copies the outputs from the ESC memory to the local memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_OutputMapping(UINT16* pData)
{
	UINT16 j = 0;
	UINT8 *pTmpData = (UINT8 *)pData;
	CiA402_AXIS_T *pAxis = &CiA402Axis;

	for (j = 0; j < sRxPDOassign.u16SubIndex0; j++)
	{
		switch ((sRxPDOassign.aEntries[j] & 0x000F))
		{
		case 0://csp/csv RxPDO    entries
			{
				TCiA402PDO1600 *pOutputs = (TCiA402PDO1600 *)pTmpData;

				*(pAxis->CiA402Var.pControlword0x6040) = SWAPWORD(pOutputs->ObjControlWord);
				*(pAxis->CiA402Var.pTargetPosition0x607A) = SWAPWORD(pOutputs->ObjTargetPosition);             
				*(pAxis->CiA402Var.pTargetVelocity0x60FF) = SWAPWORD(pOutputs->ObjTargetVelocity);                
				*(pAxis->CiA402Var.pTargetTorque0x6071) = SWAPWORD(pOutputs->ObjTargetTorque);                
				*(pAxis->CiA402Var.pHostHeartbeat0x68BA) = SWAPWORD(pOutputs->ObjHostHeartbeat);
				*(pAxis->CiA402Var.pDigitalOutput0x68BB) = SWAPWORD(pOutputs->ObjDigitalOutput);
				*(pAxis->CiA402Var.pModesOfOperation0x6060) = SWAPWORD((pOutputs->ObjModesOfOperation & 0x00FF));
                
				/*shift pointer PDO mapping object following*/
				if(j < (sRxPDOassign.u16SubIndex0 - 1))
					pTmpData += SIZEOF(TCiA402PDO1600);
			}
			break;
		case 1://csp RxPDO    entries
			{
				TCiA402PDO1601 *pOutputs = (TCiA402PDO1601 *)pTmpData;

				*(pAxis->CiA402Var.pControlword0x6040) = SWAPWORD(pOutputs->ObjControlWord);
				*(pAxis->CiA402Var.pTargetPosition0x607A) = SWAPWORD(pOutputs->ObjTargetPosition);             
				*(pAxis->CiA402Var.pTargetVelocity0x60FF) = SWAPWORD(pOutputs->ObjTargetVelocity);                
				*(pAxis->CiA402Var.pTargetTorque0x6071) = SWAPWORD(pOutputs->ObjTargetTorque);        
				*(pAxis->CiA402Var.pControlwordForAxis20x6840) = SWAPWORD(pOutputs->ObjControlwordForAxis2);                
				*(pAxis->CiA402Var.pTargetPositionForAxis20x687A) = SWAPWORD(pOutputs->ObjTargetPositionForAxis2);                
				*(pAxis->CiA402Var.pTargetVelocityForAxis20x68FF) = SWAPWORD(pOutputs->ObjTargetVelocityForAxis2);                
				*(pAxis->CiA402Var.pTargetTorqueForAxis20x6871) = SWAPWORD(pOutputs->ObjTargetTorqueForAxis2);                
				*(pAxis->CiA402Var.pHostHeartbeat0x68BA) = SWAPWORD(pOutputs->ObjHostHeartbeat);
				*(pAxis->CiA402Var.pDigitalOutput0x68BB) = SWAPWORD(pOutputs->ObjDigitalOutput);
				*(pAxis->CiA402Var.pModesOfOperation0x6060) = SWAPWORD((pOutputs->ObjModesOfOperation & 0x00FF));

				/*shift pointer PDO mapping object following*/
				if(j < (sRxPDOassign.u16SubIndex0 - 1))
					pTmpData += SIZEOF(TCiA402PDO1601);
			}
			break;
		case 2://csv RxPDO    entries
			{
				TCiA402PDO1602 *pOutputs = (TCiA402PDO1602 *)pTmpData;

#if 0										
				if (LocalAxes[AxisIndex].Objects.objControlWord != SWAPWORD(pOutputs->ObjControlWord))
				{
					DBG_PRINT("csp RxPDO: objControlWord=0x%04x\r\n", SWAPWORD(pOutputs->ObjControlWord));
				}
				if (LocalAxes[AxisIndex].Objects.objTargetVelocity != SWAPDWORD(pOutputs->ObjTargetVelocity))
				{
					DBG_PRINT("csp RxPDO: objTargetVelocity=%ld\r\n", SWAPDWORD(pOutputs->ObjTargetVelocity));							
				}
#endif
				*(pAxis->CiA402Var.pControlword0x6040) = SWAPWORD(pOutputs->ObjControlWord);
				*(pAxis->CiA402Var.pTargetPosition0x607A) = SWAPWORD(pOutputs->ObjTargetPosition);             
				*(pAxis->CiA402Var.pTargetVelocity0x60FF) = SWAPWORD(pOutputs->ObjTargetVelocity);                
				*(pAxis->CiA402Var.pTargetTorque0x6071) = SWAPWORD(pOutputs->ObjTargetTorque);                
				*(pAxis->CiA402Var.pTargetPositionForAxis20x687A) = SWAPWORD(pOutputs->ObjTargetPositionForAxis2);                
				*(pAxis->CiA402Var.pTargetVelocityForAxis20x68FF) = SWAPWORD(pOutputs->ObjTargetVelocityForAxis2);                
				*(pAxis->CiA402Var.pTargetTorqueForAxis20x6871) = SWAPWORD(pOutputs->ObjTargetTorqueForAxis2);                
				*(pAxis->CiA402Var.pTargetPositionForAxis30x707A) = SWAPWORD(pOutputs->ObjTargetPositionForAxis3);                
				*(pAxis->CiA402Var.pTargetVelocityForAxis30x70FF) = SWAPWORD(pOutputs->ObjTargetVelocityForAxis3);                
				*(pAxis->CiA402Var.pTargetTorqueForAxis30x7071) = SWAPWORD(pOutputs->ObjTargetTorqueForAxis3);
				*(pAxis->CiA402Var.pHostHeartbeat0x68BA) = SWAPWORD(pOutputs->ObjHostHeartbeat);
				*(pAxis->CiA402Var.pDigitalOutput0x68BB) = SWAPWORD(pOutputs->ObjDigitalOutput);
				*(pAxis->CiA402Var.pModesOfOperation0x6060) = SWAPWORD((pOutputs->ObjModesOfOperation & 0x00FF));
				
				/*shift pointer PDO mapping object following*/
				if(j < (sRxPDOassign.u16SubIndex0 - 1))
					pTmpData += SIZEOF(TCiA402PDO1602);	
			}
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function will called from the synchronisation ISR 
            or from the mainloop if no synchronisation is supported
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_Application(void)
{
	CiA402_AXIS_T *pAxis = &CiA402Axis;

	if(bEcatInputUpdateRunning)
	{
		CiA402_StateMachine(pAxis);
	}
		
	if (pAxis->bAxisIsActive)
	{
		CiA402_Application(pAxis);         //电机保护
	}

}

#if EXPLICIT_DEVICE_ID
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    The Explicit Device ID of the EtherCAT slave

 \brief     Calculate the Explicit Device ID
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GetDeviceID()
{
	/* Explicit Device 5 is expected by Explicit Device ID conformance tests
	   Explicit Device from switch digit of AX58200-EXB-ADIO for general application
	*/
	return 5;
}
#endif

/** @} */

int flag_4k=0;
void task(void)
{
	CiA402_AXIS_T *pAxis = &CiA402Axis;
    if(flag_4k)
    {
        flag_4k=0;
        CiA402_MotionControl(pAxis);
    }
}
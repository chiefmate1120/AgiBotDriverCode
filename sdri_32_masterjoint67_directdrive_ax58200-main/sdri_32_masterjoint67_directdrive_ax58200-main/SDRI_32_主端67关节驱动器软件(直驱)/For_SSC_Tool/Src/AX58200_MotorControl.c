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
#include "SPI_Application.h"
#include "SPI_Config.h"
#include "StmProtocol.h"
#define _AX58200__MOTOR_CONTROL_ 1
#include "AX58200_MotorControl.h"
#undef _AX58200__MOTOR_CONTROL_

/*-----------------------------------------------------------------------------------------
------
------    global variables and constants
------
-----------------------------------------------------------------------------------------*/
unsigned int Alignment;
unsigned int MotorState;
unsigned char MotorEnFlag;
unsigned char Motor2EnFlag;
unsigned char datasave;
unsigned char position1st;
unsigned int Keyvalue;
unsigned int ENCValue;
unsigned char slave;
extern TOBJECT OBJMEM ApplicationObjDic[];
CiA402_AXIS_T  CiA402Axis;


extern struct
{
	unsigned int count;
	unsigned char loop;
	unsigned char mode;
}led;

extern unsigned int ENC2_raw;
extern unsigned char ENC2_counter;
unsigned char PID_FLAG;
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
void CiA402_StateMachine_AXIS2(CiA402_AXIS_T *pAxis);

void CiA402_Application(CiA402_AXIS_T *pAxis);
void CiA402_Application_AXIS2(CiA402_AXIS_T *pAxis);

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

	pAxis->bAxisIsActive_AXIS2 = FALSE;
	pAxis->bBrakeApplied_AXIS2 = TRUE;
	pAxis->bLowLevelPowerApplied_AXIS2  = TRUE;
	pAxis->bHighLevelPowerApplied_AXIS2 = FALSE;
	pAxis->bAxisFunctionEnabled_AXIS2   = FALSE;
	pAxis->bConfigurationAllowed_AXIS2  = TRUE;
	
	pAxis->i16State_AXIS2 = STATE_NOT_READY_TO_SWITCH_ON;
	pAxis->u16PendingOptionCode_AXIS2 = 0x00;    	
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
	pAxis->CiA402Var.pPID_Velocity_P0x6090 = &PID0x6090.PIDVelocityP;
	pAxis->CiA402Var.pPID_Velocity_I0x6090 = &PID0x6090.PIDVelocityI;
	pAxis->CiA402Var.pPID_Position_P0x6090 = &PID0x6090.PIDPositionP;
	pAxis->CiA402Var.pPID_Position_I0x6090 = &PID0x6090.PIDPositionI;
    
	pAxis->CiA402Var.pPID_Current_P0x6091 = &PID0x6091.PIDCurrentP;
	pAxis->CiA402Var.pPID_Current_I0x6091 = &PID0x6091.PIDCurrentI;
	pAxis->CiA402Var.pPID_Velocity_P0x6091 = &PID0x6091.PIDVelocityP;
	pAxis->CiA402Var.pPID_Velocity_I0x6091 = &PID0x6091.PIDVelocityI;
	pAxis->CiA402Var.pPID_Position_P0x6091 = &PID0x6091.PIDPositionP;
	pAxis->CiA402Var.pPID_Position_I0x6091 = &PID0x6091.PIDPositionI;    
    
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
	pAxis->CiA402Var.pModesOfOperationForAxis20x6860 = &ModesOfOperationForAxis20x6860;
	pAxis->CiA402Var.pModesOfOperationDisplayForAxis20x6861 = &ModesOfOperationDisplayForAxis20x6861;     
    
    
    /**                            AXIS 3                     **/          
    
	pAxis->CiA402Var.pPositionActualValueForAxis30x7064 = &PositionActualValueForAxis30x7064;
	pAxis->CiA402Var.pVelocityActualValueForAxis30x706C = &VelocityActualValueForAxis30x706C;
	pAxis->CiA402Var.pTorqueActualValueForAxis30x7077 = &TorqueActualValueForAxis30x7077;
	pAxis->CiA402Var.pENCForAxis30x707F = &ENCForAxis30x707F;
	pAxis->CiA402Var.pTargetPositionForAxis30x707A = &TargetPositionForAxis30x707A;
	pAxis->CiA402Var.pTargetVelocityForAxis30x70FF = &TargetVelocityForAxis30x70FF;
	pAxis->CiA402Var.pTargetTorqueForAxis30x7071 = &TargetTorqueForAxis30x7071;


    /**                            AXIS 4                    **/              
	pAxis->CiA402Var.pPositionActualValueForAxis40x7864 = &PositionActualValueForAxis40x7864;
	pAxis->CiA402Var.pVelocityActualValueForAxis40x786C = &VelocityActualValueForAxis40x786C;
	pAxis->CiA402Var.pTorqueActualValueForAxis40x7877 = &TorqueActualValueForAxis40x7877;
	pAxis->CiA402Var.pENCForAxis40x787F = &ENCForAxis40x787F;
	pAxis->CiA402Var.pTargetPositionForAxis40x787A = &TargetPositionForAxis40x787A;
	pAxis->CiA402Var.pTargetVelocityForAxis40x78FF = &TargetVelocityForAxis40x78FF;
	pAxis->CiA402Var.pTargetTorqueForAxis40x7871 = &TargetTorqueForAxis40x7871;

    /**                            AXIS 5                   **/              
	pAxis->CiA402Var.pPositionActualValueForAxis50x8064 = &PositionActualValueForAxis50x8064;
	pAxis->CiA402Var.pVelocityActualValueForAxis50x806C = &VelocityActualValueForAxis50x806C;
	pAxis->CiA402Var.pTorqueActualValueForAxis50x8077 = &TorqueActualValueForAxis50x8077;
	pAxis->CiA402Var.pENCForAxis50x807F = &ENCForAxis50x807F;
	pAxis->CiA402Var.pTargetPositionForAxis50x807A = &TargetPositionForAxis50x807A;
	pAxis->CiA402Var.pTargetVelocityForAxis50x80FF = &TargetVelocityForAxis50x80FF;
	pAxis->CiA402Var.pTargetTorqueForAxis50x8071 = &TargetTorqueForAxis50x8071;
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
StmProtocol_t protocol_6[6];
StmProtocol_t protocol_7[6];    


int CiA402_MotionControl(CiA402_AXIS_T *pAxis)
{
	int TargetPosition;
	int TargetVelocity;
	int TargetTorque;
	unsigned int ControlMode;
	static int actualposition,actualposition_pre,TargetPositionTemp;
	int temp;
	short speed;
	
	static uint16_t ErrCode1 = 0;
	static uint16_t ErrCode2 = 0;

    
    static int ActualHall=0,ActualHallA=0,ActualHallB=0;
/************************  AXIS 6�������ݴ���****************************************/ 	
	if(HW_GPIO_ReadPin(PF,BIT_4))
	{  
        if((PID_Motor1.Current_P!=*(pAxis->CiA402Var.pPID_Current_P0x6090))    ||  (PID_Motor1.Current_I!=*(pAxis->CiA402Var.pPID_Current_I0x6090))   ||      \
           (PID_Motor1.Velocity_P!=*(pAxis->CiA402Var.pPID_Velocity_P0x6090))  ||  (PID_Motor1.Velocity_I!=*(pAxis->CiA402Var.pPID_Velocity_I0x6090)) ||      \
           (PID_Motor1.Position_P!=*(pAxis->CiA402Var.pPID_Position_P0x6090))  ||  (PID_Motor1.Position_I!=*(pAxis->CiA402Var.pPID_Position_I0x6090)))
        {
            if((*(pAxis->CiA402Var.pModesOfOperation0x6060) == 20) )
            {
                PID_Motor1.Current_P=*(pAxis->CiA402Var.pPID_Current_P0x6090);
                PID_Motor1.Current_I=*(pAxis->CiA402Var.pPID_Current_I0x6090);
                PID_Motor1.Velocity_P=*(pAxis->CiA402Var.pPID_Velocity_P0x6090);
                PID_Motor1.Velocity_I=*(pAxis->CiA402Var.pPID_Velocity_I0x6090);
                PID_Motor1.Position_P=*(pAxis->CiA402Var.pPID_Position_P0x6090);
                PID_Motor1.Position_I=*(pAxis->CiA402Var.pPID_Position_I0x6090);          
                
                PID_FLAG=1;
            }   
            else  
            {
                *(pAxis->CiA402Var.pPID_Current_P0x6090) = PID_Motor1.Current_P;
                *(pAxis->CiA402Var.pPID_Current_I0x6090) = PID_Motor1.Current_I;
                *(pAxis->CiA402Var.pPID_Velocity_P0x6090)= PID_Motor1.Velocity_P;
                *(pAxis->CiA402Var.pPID_Velocity_I0x6090)= PID_Motor1.Velocity_I;
                *(pAxis->CiA402Var.pPID_Position_P0x6090)= PID_Motor1.Position_P;  
                *(pAxis->CiA402Var.pPID_Position_I0x6090)= PID_Motor1.Position_I;         
            }            
        }        
 		if(PID_FLAG)
		{			
			PID[0]=*(pAxis->CiA402Var.pPID_Current_P0x6090);				
			PID[1]=*(pAxis->CiA402Var.pPID_Current_I0x6090);				
			PID[2]=*(pAxis->CiA402Var.pPID_Velocity_P0x6090);				
			PID[3]=*(pAxis->CiA402Var.pPID_Velocity_I0x6090);			
			PID[4]=*(pAxis->CiA402Var.pPID_Position_P0x6090);				
			PID[5]=55;		
            PID[6]=*(pAxis->CiA402Var.pPID_Position_I0x6090);
			PID[7]=*(pAxis->CiA402Var.pPID_Current_P0x6091);			
			PID[8]=*(pAxis->CiA402Var.pPID_Current_I0x6091);			
			PID[9]=*(pAxis->CiA402Var.pPID_Velocity_P0x6091);		
			PID[10]=*(pAxis->CiA402Var.pPID_Velocity_I0x6091);		
			PID[11]=*(pAxis->CiA402Var.pPID_Position_P0x6091);		
			PID[12]=*(pAxis->CiA402Var.pPID_Position_I0x6091);

			//Updata TxBuff Data
			protocol_6[0].tx_protocol.cmd = TMC4671_PID_TORQUE_P_TORQUE_I;
			protocol_6[1].tx_protocol.cmd = TMC4671_PID_FLUX_P_FLUX_I;
			protocol_6[2].tx_protocol.cmd = TMC4671_PID_VELOCITY_P_VELOCITY_I;
			protocol_6[3].tx_protocol.cmd = TMC4671_PID_POSITION_P_POSITION_I;
			protocol_6[4].tx_protocol.cmd = 0;           
			protocol_6[5].tx_protocol.cmd = VERITY_TX_FLAG;

			protocol_6[0].tx_protocol.data = PID_Motor1.Current_P<<16|PID_Motor1.Current_I;
			protocol_6[1].tx_protocol.data = PID_Motor1.Current_P<<16|PID_Motor1.Current_I;
			protocol_6[2].tx_protocol.data = PID_Motor1.Velocity_P<<16|PID_Motor1.Velocity_I;
			protocol_6[3].tx_protocol.data = PID_Motor1.Position_P<<16|PID_Motor1.Position_I;
			protocol_6[4].tx_protocol.data = 0;                   
			protocol_6[5].tx_protocol.data = TxVerifyCalculation(protocol_6,sizeof(protocol_6)/sizeof(protocol_6[0]));

			for(int i=0; i<sizeof(protocol_6)/sizeof(protocol_6[0]); i++)
			{
				STM_WriteRead(SPI1, protocol_6[i].tx_protocol.cmd, protocol_6[i].tx_protocol.data, (uint8_t *)&protocol_6[i].rx_protocol);
			}

			PID_FLAG--;
//			if(!PID_FLAG)
//			Fmc_PID_Write();	
		}    
        else
        {
/************************���ݴ���****************************************/    
			//Updata Send Data
			TargetPosition = *(pAxis->CiA402Var.pTargetPosition0x607A);
			TargetTorque = *(pAxis->CiA402Var.pTargetTorque0x6071);
			TargetVelocity = *(pAxis->CiA402Var.pTargetVelocity0x60FF);
			ControlMode=*(pAxis->CiA402Var.pModesOfOperation0x6060)|MotorEnFlag<<16;
            
            //Send  protocol_6 Ready
			protocol_6[0].tx_protocol.cmd = POSITION_TX_FLAG;
			protocol_6[0].tx_protocol.data = TargetPosition;
			protocol_6[1].tx_protocol.cmd = TORQUE_TX_FLAG;
			protocol_6[1].tx_protocol.data = TargetTorque;
			protocol_6[2].tx_protocol.cmd = VELOCTITY_TX_FLAG;
			protocol_6[2].tx_protocol.data = TargetVelocity;
			protocol_6[3].tx_protocol.cmd = CONTROL_TX_FLAG;
			protocol_6[3].tx_protocol.data = ControlMode;
			protocol_6[4].tx_protocol.cmd = HALL_RX_FLAG;
			protocol_6[4].tx_protocol.data = 0;			
          
			protocol_6[5].tx_protocol.cmd = VERITY_TX_FLAG;
			protocol_6[5].tx_protocol.data = TxVerifyCalculation(protocol_6, sizeof(protocol_6)/sizeof(protocol_6[0]));            
            
			//Send All protocol_6
			for(int i=0; i<sizeof(protocol_6)/sizeof(protocol_6[0]); i++)
			{
				STM_WriteRead(SPI1, protocol_6[i].tx_protocol.cmd, protocol_6[i].tx_protocol.data, (uint8_t *)&protocol_6[i].rx_protocol);
			}                    

            
			//check data and updata data
			if(RxVerifyCheck(protocol_6, sizeof(protocol_6)/sizeof(protocol_6[0])))
			{
				if(protocol_6[0].rx_protocol.cmd == POSITION_RX_FLAG)
				{
					temp = protocol_6[0].rx_protocol.data;
					*(pAxis->CiA402Var.pPositionActualValue0x6064) =temp;
				}
				
				if(protocol_6[1].rx_protocol.cmd == TORQUE_RX_FLAG)
				{
					temp = protocol_6[1].rx_protocol.data;
					*(pAxis->CiA402Var.pTorqueActualValue0x6077) =temp;
				}
				
				if(protocol_6[2].rx_protocol.cmd == VELOCTITY_RX_FLAG)
				{
					temp = protocol_6[2].rx_protocol.data;
					*(pAxis->CiA402Var.pVelocityActualValue0x606C) =((temp>>16) & 0xffff);
					*(pAxis->CiA402Var.pENC0x607F)=temp & 0xffff; 
                    
				}
				
				if(protocol_6[3].rx_protocol.cmd == CONTROL_RX_FLAG)
				{
					uint8_t NaSingle,HeadState,EncHeadSigle;
					uint16_t VersionState;
					uint16_t ErrMsg;
					MotorState = protocol_6[3].rx_protocol.data;
					
					NaSingle = (MotorState >> 24) & 0xF;
					HeadState = (MotorState >> 28) & 0x1;
					EncHeadSigle = (MotorState >> 20) & 0x01;
					VersionState = (MotorState) & 0xFFFF; 
					ErrMsg = (MotorState >> 21) & 0x7;
					ErrCode1 = ErrMsg;

					
					*(pAxis->CiA402Var.pHeartbeat0x68AA) = (*(pAxis->CiA402Var.pHeartbeat0x68AA) & 0x0000FFFF) |
															((HeadState & 0xFFFF) << 16);
				
					*(pAxis->CiA402Var.pQuickStopDeceleration0x6085) = 	(*(pAxis->CiA402Var.pQuickStopDeceleration0x6085) & 0x0000FFFF) |
																		((VersionState & 0xFFFF) << 16);
					
					*(pAxis->CiA402Var.pFluxActualValue0x68AB) = (*(pAxis->CiA402Var.pFluxActualValue0x68AB) & 0x0000FFFF) ;
					*(pAxis->CiA402Var.pErrorMessage0x6893) = (*(pAxis->CiA402Var.pErrorMessage0x6893) & ~(7<<6) & ~(0x01<<18)) | (ErrCode1 << 6) | (EncHeadSigle << 18);
				}               
			}             
		}    
	}        
    

    
/************************  AXIS 7�������ݴ���****************************************/ 
	if(HW_GPIO_ReadPin(PE,BIT_6))
    {
        if((PID_Motor2.Current_P!=*(pAxis->CiA402Var.pPID_Current_P0x6091))    ||  (PID_Motor2.Current_I!=*(pAxis->CiA402Var.pPID_Current_I0x6091))   ||      \
           (PID_Motor2.Velocity_P!=*(pAxis->CiA402Var.pPID_Velocity_P0x6091))  ||  (PID_Motor2.Velocity_I!=*(pAxis->CiA402Var.pPID_Velocity_I0x6091)) ||      \
           (PID_Motor2.Position_P!=*(pAxis->CiA402Var.pPID_Position_P0x6091))  ||  (PID_Motor2.Position_I!=*(pAxis->CiA402Var.pPID_Position_I0x6091)) )
        {
            if((*(pAxis->CiA402Var.pModesOfOperationForAxis20x6860) == 20))
            {
                PID_Motor2.Current_P=*(pAxis->CiA402Var.pPID_Current_P0x6091);
                PID_Motor2.Current_I=*(pAxis->CiA402Var.pPID_Current_I0x6091);
                PID_Motor2.Velocity_P=*(pAxis->CiA402Var.pPID_Velocity_P0x6091);
                PID_Motor2.Velocity_I=*(pAxis->CiA402Var.pPID_Velocity_I0x6091);
                PID_Motor2.Position_P=*(pAxis->CiA402Var.pPID_Position_P0x6091);
                PID_Motor2.Position_I=*(pAxis->CiA402Var.pPID_Position_I0x6091);             
                
                PID_FLAG=1;
            }   
            else  
            {

                *(pAxis->CiA402Var.pPID_Current_P0x6091) = PID_Motor2.Current_P;
                *(pAxis->CiA402Var.pPID_Current_I0x6091) = PID_Motor2.Current_I;
                *(pAxis->CiA402Var.pPID_Velocity_P0x6091)= PID_Motor2.Velocity_P;
                *(pAxis->CiA402Var.pPID_Velocity_I0x6091)= PID_Motor2.Velocity_I;
                *(pAxis->CiA402Var.pPID_Position_P0x6091)= PID_Motor2.Position_P;  
                *(pAxis->CiA402Var.pPID_Position_I0x6091)= PID_Motor2.Position_I;            
            }            
        } 
        if(PID_FLAG)
        {
			PID[0]=*(pAxis->CiA402Var.pPID_Current_P0x6090);				
			PID[1]=*(pAxis->CiA402Var.pPID_Current_I0x6090);				
			PID[2]=*(pAxis->CiA402Var.pPID_Velocity_P0x6090);				
			PID[3]=*(pAxis->CiA402Var.pPID_Velocity_I0x6090);				
			PID[4]=*(pAxis->CiA402Var.pPID_Position_P0x6090);				
			PID[5]=55;			
            PID[6]=*(pAxis->CiA402Var.pPID_Position_I0x6090);
			PID[7]=*(pAxis->CiA402Var.pPID_Current_P0x6091);				
			PID[8]=*(pAxis->CiA402Var.pPID_Current_I0x6091);				
			PID[9]=*(pAxis->CiA402Var.pPID_Velocity_P0x6091);				
			PID[10]=*(pAxis->CiA402Var.pPID_Velocity_I0x6091);				
			PID[11]=*(pAxis->CiA402Var.pPID_Position_P0x6091);				
			PID[12]=*(pAxis->CiA402Var.pPID_Position_I0x6091);


			//Updata TxBuff Data
			protocol_7[0].tx_protocol.cmd = TMC4671_PID_TORQUE_P_TORQUE_I;
			protocol_7[1].tx_protocol.cmd = TMC4671_PID_FLUX_P_FLUX_I;
			protocol_7[2].tx_protocol.cmd = TMC4671_PID_VELOCITY_P_VELOCITY_I;
			protocol_7[3].tx_protocol.cmd = TMC4671_PID_POSITION_P_POSITION_I;
			protocol_7[4].tx_protocol.cmd = 0;           
			protocol_7[5].tx_protocol.cmd = VERITY_TX_FLAG;

			protocol_7[0].tx_protocol.data = PID_Motor2.Current_P<<16|PID_Motor2.Current_I;
			protocol_7[1].tx_protocol.data = PID_Motor2.Current_P<<16|PID_Motor2.Current_I;
			protocol_7[2].tx_protocol.data = PID_Motor2.Velocity_P<<16|PID_Motor2.Velocity_I;
			protocol_7[3].tx_protocol.data = PID_Motor2.Position_P<<16|PID_Motor2.Position_I;
			protocol_7[4].tx_protocol.data = 0;                   
			protocol_7[5].tx_protocol.data = TxVerifyCalculation(protocol_7,sizeof(protocol_7)/sizeof(protocol_7[0]));


			for(int i=0; i<sizeof(protocol_7)/sizeof(protocol_7[0]); i++)
			{
					STM_WriteRead(SPI3, protocol_7[i].tx_protocol.cmd, protocol_7[i].tx_protocol.data, (uint8_t *)&protocol_7[i].rx_protocol);
			}

            
			PID_FLAG--;
//			if(!PID_FLAG)
//			Fmc_PID_Write(); 
        }
        else
        {
/************************���ݴ���****************************************/    
			//Updata Send Data
			TargetPosition = *(pAxis->CiA402Var.pTargetPositionForAxis20x687A);
			TargetTorque = *(pAxis->CiA402Var.pTargetTorqueForAxis20x6871);
			TargetVelocity = *(pAxis->CiA402Var.pTargetVelocityForAxis20x68FF);
			ControlMode=*(pAxis->CiA402Var.pModesOfOperationForAxis20x6860)|Motor2EnFlag<<16;
            
            			//Send  protocol_7 Ready
			protocol_7[0].tx_protocol.cmd = POSITION_TX_FLAG;
			protocol_7[0].tx_protocol.data = TargetPosition;
			protocol_7[1].tx_protocol.cmd = TORQUE_TX_FLAG;
			protocol_7[1].tx_protocol.data = TargetTorque;
			protocol_7[2].tx_protocol.cmd = VELOCTITY_TX_FLAG;
			protocol_7[2].tx_protocol.data = TargetVelocity;
			protocol_7[3].tx_protocol.cmd = CONTROL_TX_FLAG;
			protocol_7[3].tx_protocol.data = ControlMode;
			protocol_7[4].tx_protocol.cmd = HALL_RX_FLAG;
			protocol_7[4].tx_protocol.data = 0;			
          
			protocol_7[5].tx_protocol.cmd = VERITY_TX_FLAG;
			protocol_7[5].tx_protocol.data = TxVerifyCalculation(protocol_7, sizeof(protocol_7)/sizeof(protocol_7[0]));            
            
			//Send All protocol_7
			for(int i=0; i<sizeof(protocol_7)/sizeof(protocol_7[0]); i++)
			{
				STM_WriteRead(SPI3, protocol_7[i].tx_protocol.cmd, protocol_7[i].tx_protocol.data, (uint8_t *)&protocol_7[i].rx_protocol);
			}                    

            
			//check data and updata data
			if(RxVerifyCheck(protocol_7, sizeof(protocol_7)/sizeof(protocol_7[0])))
			{
				if(protocol_7[0].rx_protocol.cmd == POSITION_RX_FLAG)
				{
					temp = protocol_7[0].rx_protocol.data;
					*(pAxis->CiA402Var.pPositionActualValueForAxis20x6864) =temp;
				}
				
				if(protocol_7[1].rx_protocol.cmd == TORQUE_RX_FLAG)
				{
					temp = protocol_7[1].rx_protocol.data;
					*(pAxis->CiA402Var.pTorqueActualValueForAxis20x6877) =temp;
				}
				
				if(protocol_7[2].rx_protocol.cmd == VELOCTITY_RX_FLAG)
				{
					temp = protocol_7[2].rx_protocol.data;
                    
					*(pAxis->CiA402Var.pVelocityActualValueForAxis20x686C) =((temp>>16) & 0xffff);
					*(pAxis->CiA402Var.pENCForAxis20x687F)=temp & 0xffff; 
                    
				}
				
				if(protocol_7[3].rx_protocol.cmd == CONTROL_RX_FLAG)
				{
					
					uint8_t KeyState,HeadState,EncHeadSigle;
					uint16_t VersionState;
					uint16_t ErrMsg;
					MotorState = protocol_7[3].rx_protocol.data;
			
					KeyState = (MotorState >> 24) & 0xF;
					HeadState = (MotorState >> 28) & 0x1;
					EncHeadSigle = (MotorState >>20) & 0x01;
					VersionState = (MotorState) & 0xFFFF; 
					ErrMsg = (MotorState >> 21) & 0x7;
					ErrCode2 = ErrMsg;

					
					*(pAxis->CiA402Var.pDigitalInput0x6892) = KeyState;
					*(pAxis->CiA402Var.pHeartbeat0x68AA) =  (*(pAxis->CiA402Var.pHeartbeat0x68AA) & 0xFFFF0000) | 
																									(HeadState & 0xFFFF);
					*(pAxis->CiA402Var.pQuickStopDeceleration0x6085) = 	(*(pAxis->CiA402Var.pQuickStopDeceleration0x6085) & 0xFFFF0000) | 
																															(VersionState & 0xFFFF);      
					*(pAxis->CiA402Var.pFluxActualValue0x68AB) = (*(pAxis->CiA402Var.pFluxActualValue0x68AB) & 0xFFFF0000);   
					*(pAxis->CiA402Var.pErrorMessage0x6893) = (*(pAxis->CiA402Var.pErrorMessage0x6893) & ~(7<<9) & ~(1<<19)) | (ErrCode2 << 9) | (EncHeadSigle << 19);
							
				}
                
				if(protocol_7[4].rx_protocol.cmd == HALL_RX_FLAG)
				{
					 ActualHall =  protocol_7[4].rx_protocol.data;
					 ActualHallA = (ActualHall>>16 & 0xFFFF);
					 ActualHallB = (ActualHall & 0xFFFF);
					 *(pAxis->CiA402Var.pAnalogInput0x6891) = ActualHall;  
				}
			}             
     }   
    }



	
	
//	if(nAlStatus != STATE_OP)
//	{     
//        MotorEnFlag=0;
//        brake.flag=0; 

//        Motor2EnFlag=0;
//        brake2.flag=0;       
//        ErrorMessage_joint4.EtherCATOPError =1<<29 ;         
//	}
//    else
//    {
//        ErrorMessage_joint4.EtherCATOPError =0 ; 
//    }  
    
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
            MotorEnFlag=1;
            brake.flag=1;
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
			MotorEnFlag=0;
            brake.flag=0;            
		}
		else if ((ControlWord & CONTROLWORD_COMMAND_QUICKSTOP_MASK) == CONTROLWORD_COMMAND_QUICKSTOP)
		{
            /* Stop motor */			
			Servo_Ready_Flag=0;
			stop=1; 
			MotorEnFlag=0;
            brake.flag=0; 
            
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
			MotorEnFlag=0;
            brake.flag=0; 
            
		}
		else if ((ControlWord & CONTROLWORD_COMMAND_DISABLEVOLTAGE_MASK) == CONTROLWORD_COMMAND_DISABLEVOLTAGE)
		{
			pAxis->i16State = STATE_SWITCH_ON_DISABLED; // Transition 9
			DBG_PRINT("Transition 9, OPERATION_ENABLED->SWITCH_ON_DISABLED\r\n");
				
			/* Stop motor */
			
			Servo_Ready_Flag=0;
			stop=1;
			MotorEnFlag=0;
            brake.flag=0;   
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



/**
 \brief    CiA402-Statemachine
        This function handles the state machine for devices using the CiA402 profile.
        called cyclic from MainLoop()
        All described transition numbers are referring to the document
        "ETG Implementation Guideline for the CiA402 Axis Profile" located on the EtherCAT.org download section

*////////////////////////////////////////////////////////////////////////////////////////
void CiA402_StateMachine_AXIS2(CiA402_AXIS_T *pAxis)
{
	UINT16 StatusWord_AXIS2 = 0;
	UINT16 ControlWord_AXIS2 = 0;

	/* Skip unavailable axis */
	if(pAxis->bAxisIsActive_AXIS2 == 0)
	{
		return;
	}

	StatusWord_AXIS2 = *(pAxis->CiA402Var.pStatuswordForAxis20x6841);
	ControlWord_AXIS2 = *(pAxis->CiA402Var.pControlwordForAxis20x6840);

	/* Clear statusword state and controlword processed complete bits */
	StatusWord_AXIS2 &= ~(STATUSWORD_STATE_MASK | STATUSWORD_REMOTE);

	/* Skip state state transition if the previous transition is pending */
	if(pAxis->u16PendingOptionCode_AXIS2!= 0x0)
	{
		return;
	}
		
	/* Skip transition 1 and 2 if Slave has been under OP mode */
	if(pAxis->i16State_AXIS2 < STATE_READY_TO_SWITCH_ON && nAlStatus == STATE_OP)
	{
		pAxis->i16State_AXIS2 = STATE_READY_TO_SWITCH_ON;
	}
				
	switch(pAxis->i16State_AXIS2)
	{
	case STATE_NOT_READY_TO_SWITCH_ON:
		StatusWord_AXIS2 |= (STATUSWORD_STATE_NOTREADYTOSWITCHON);
		if(nAlStatus == STATE_OP)
		{
			// Automatic transition -> Communication shall be activated
			pAxis->i16State_AXIS2 = STATE_SWITCH_ON_DISABLED; // Transition 1
			DBG_PRINT("Transition 1, NOT_READY_TO_SWITCH_ON->SWITCH_ON_DISABLED\r\n");
		}
		else
		{
			/* CiA402 statemachine shall stay in "STATE_NOT_READY_TO_SWITCH_ON" if EtherCAT state is not OP. */
			pAxis->i16State_AXIS2 = STATE_NOT_READY_TO_SWITCH_ON; // stay in current state
		}
		break;
			
	case STATE_SWITCH_ON_DISABLED:
		StatusWord_AXIS2 |= (STATUSWORD_STATE_SWITCHEDONDISABLED);
		if ((ControlWord_AXIS2 & CONTROLWORD_COMMAND_SHUTDOWN_MASK) == CONTROLWORD_COMMAND_SHUTDOWN)
		{
			pAxis->i16State_AXIS2 = STATE_READY_TO_SWITCH_ON; // Transition 2
			DBG_PRINT("Transition 2, SWITCH_ON_DISABLED->READY_TO_SWITCH_ON\r\n");
		}
		break;
			
	case STATE_READY_TO_SWITCH_ON:
		StatusWord_AXIS2 |= (STATUSWORD_STATE_READYTOSWITCHON);
		if (((ControlWord_AXIS2 & CONTROLWORD_COMMAND_QUICKSTOP_MASK) == CONTROLWORD_COMMAND_QUICKSTOP)
			|| ((ControlWord_AXIS2 & CONTROLWORD_COMMAND_DISABLEVOLTAGE_MASK) == CONTROLWORD_COMMAND_DISABLEVOLTAGE))
		{
			pAxis->i16State_AXIS2 = STATE_SWITCH_ON_DISABLED; // Transition 7
			if (nAlStatus != STATE_OP)
			{
				DBG_PRINT("Transition 7, READY_TO_SWITCH_ON->SWITCH_ON_DISABLED(0x%04x)\r\n", ControlWord);							
			}
		}
		else if (((ControlWord_AXIS2 & CONTROLWORD_COMMAND_SWITCHON_MASK) == CONTROLWORD_COMMAND_SWITCHON) &&
							((ControlWord_AXIS2 & CONTROLWORD_COMMAND_SWITCHON_ENABLEOPERATION_MASK) != CONTROLWORD_COMMAND_SWITCHON_ENABLEOPERATION))
		{
			pAxis->i16State_AXIS2 = STATE_SWITCHED_ON;        // Transition 3
			DBG_PRINT("Transition 3, READY_TO_SWITCH_ON->SWITCHED_ON\r\n");
		}
		break;
			
	case STATE_SWITCHED_ON:
		StatusWord_AXIS2 |= (STATUSWORD_STATE_SWITCHEDON);
		if ((ControlWord_AXIS2 & CONTROLWORD_COMMAND_SHUTDOWN_MASK) == CONTROLWORD_COMMAND_SHUTDOWN)
		{
			pAxis->i16State_AXIS2 = STATE_READY_TO_SWITCH_ON; // Transition 6
			DBG_PRINT("Transition 6, SWITCHED_ON->READY_TO_SWITCH_ON\r\n");
		} 
		else if (((ControlWord_AXIS2 & CONTROLWORD_COMMAND_QUICKSTOP_MASK) == CONTROLWORD_COMMAND_QUICKSTOP
						|| (ControlWord_AXIS2 & CONTROLWORD_COMMAND_DISABLEVOLTAGE_MASK) == CONTROLWORD_COMMAND_DISABLEVOLTAGE))
		{
			pAxis->i16State_AXIS2 = STATE_SWITCH_ON_DISABLED; // Transition 10
			DBG_PRINT("Transition 10, SWITCHED_ON->SWITCH_ON_DISABLED\r\n");
		}
		else if ((ControlWord_AXIS2 & CONTROLWORD_COMMAND_ENABLEOPERATION_MASK) == CONTROLWORD_COMMAND_ENABLEOPERATION)
		{
			/* Start motor */			
			Servo_Ready_Flag_for_aix2=1;
			stop_for_aix2=0;
            Motor2EnFlag=1;
            brake2.flag=1;
			pAxis->i16State_AXIS2 = STATE_OPERATION_ENABLED;  // Transition 4
			DBG_PRINT("Transition 4, SWITCHED_ON->OPERATION_ENABLED\r\n");
			//The Axis function shall be enabled and all internal set-points cleared.
		}
		break;
			
	case STATE_OPERATION_ENABLED:
		StatusWord_AXIS2 |= (STATUSWORD_STATE_OPERATIONENABLED);
		if ((ControlWord_AXIS2 & CONTROLWORD_COMMAND_DISABLEOPERATION_MASK) == CONTROLWORD_COMMAND_DISABLEOPERATION)
		{
			if(*(pAxis->CiA402Var.pDisableOperationOptionCode0x605C)!= DISABLE_DRIVE)
			{
				/* Disable operation pending */
				pAxis->u16PendingOptionCode_AXIS2 = 0x605C; //STATE_TRANSITION (STATE_OPERATION_ENABLED,STATE_SWITCHED_ON);
//				return;
			}
			pAxis->i16State_AXIS2 = STATE_SWITCHED_ON;           // Transition 5
			DBG_PRINT("Transition 5, OPERATION_ENABLED->SWITCHED_ON\r\n");

			/* Stop motor */			
			Servo_Ready_Flag_for_aix2=0;
			stop_for_aix2=1;
            Motor2EnFlag=0;
            brake2.flag=0;  
		}
		else if ((ControlWord_AXIS2 & CONTROLWORD_COMMAND_QUICKSTOP_MASK) == CONTROLWORD_COMMAND_QUICKSTOP)
		{		
			/* Stop motor */			
			Servo_Ready_Flag_for_aix2=0;
			stop_for_aix2=1;
            Motor2EnFlag=0;
            brake2.flag=0;            
			pAxis->i16State_AXIS2 = STATE_QUICK_STOP_ACTIVE;  // Transition 11
			DBG_PRINT("Transition 11, OPERATION_ENABLED->QUICK_STOP_ACTIVE\r\n");
		}
		else if ((ControlWord_AXIS2 & CONTROLWORD_COMMAND_SHUTDOWN_MASK) == CONTROLWORD_COMMAND_SHUTDOWN)
		{
			if (*(pAxis->CiA402Var.pShutdownOptionCode0x605B) != DISABLE_DRIVE)
			{
				/* Shutdown operation required */
				pAxis->u16PendingOptionCode_AXIS2 = 0x605B; //STATE_TRANSITION (STATE_OPERATION_ENABLED,STATE_READY_TO_SWITCH_ON);
//				return;
			}
			pAxis->i16State_AXIS2 = STATE_READY_TO_SWITCH_ON; // Transition 8
			DBG_PRINT("Transition 8, OPERATION_ENABLED->READY_TO_SWITCH_ON\r\n");
				
			Servo_Ready_Flag_for_aix2=0;
			stop_for_aix2=1;
            Motor2EnFlag=0;
            brake2.flag=0;  
		}
		else if ((ControlWord_AXIS2 & CONTROLWORD_COMMAND_DISABLEVOLTAGE_MASK) == CONTROLWORD_COMMAND_DISABLEVOLTAGE)
		{
			pAxis->i16State_AXIS2 = STATE_SWITCH_ON_DISABLED; // Transition 9
			DBG_PRINT("Transition 9, OPERATION_ENABLED->SWITCH_ON_DISABLED\r\n");
				
			/* Stop motor */			
			Servo_Ready_Flag_for_aix2=0;
			stop_for_aix2=1;
            Motor2EnFlag=0;
            brake2.flag=0;  
		}
		break;
			
	case STATE_QUICK_STOP_ACTIVE:
		StatusWord_AXIS2 |= STATUSWORD_STATE_QUICKSTOPACTIVE;
		if ((*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) != DISABLE_DRIVE) &&
				((*(pAxis->CiA402Var.pStatuswordForAxis20x6841) & STATUSWORD_STATE_MASK)!= STATUSWORD_STATE_QUICKSTOPACTIVE))
		{
			/* Only execute quick stop action in state transition 11 */
			pAxis->u16PendingOptionCode_AXIS2 = 0x605A;//STATE_TRANSITION (STATE_OPERATION_ENABLED,STATE_QUICK_STOP_ACTIVE);
			DBG_PRINT("Set u16PendingOptionCode = 0x605A\r\n");
			return;
		}

		if ((ControlWord_AXIS2 & CONTROLWORD_COMMAND_DISABLEVOLTAGE_MASK) == CONTROLWORD_COMMAND_DISABLEVOLTAGE)
		{
			pAxis->i16State_AXIS2 = STATE_SWITCH_ON_DISABLED; // Transition 12
			DBG_PRINT("Transition 12, QUICK_STOP_ACTIVE->SWITCH_ON_DISABLED\r\n");
		}
		/* NOTE: it is not recommend to support transition 16 */
		break;
			
	case STATE_FAULT_REACTION_ACTIVE:
		StatusWord_AXIS2 |= (STATUSWORD_STATE_FAULTREACTIONACTIVE);
		if (*(pAxis->CiA402Var.pFaultReactionOptionCode0x605E)!= DISABLE_DRIVE)
		{
			/* Fault reaction pending */
			pAxis->u16PendingOptionCode_AXIS2 = 0x605E;
			return;
		}

		// Automatic transition
		pAxis->i16State_AXIS2 = STATE_FAULT;// Transition 14
//			DBG_PRINT("Transition 14, FAULT_REACTION_ACTIVE->FAULT\r\n");
		break;
			
	case STATE_FAULT:
		StatusWord_AXIS2 |= (STATUSWORD_STATE_FAULT);

		if ((ControlWord_AXIS2 & CONTROLWORD_COMMAND_FAULTRESET_MASK) == CONTROLWORD_COMMAND_FAULTRESET)
		{
			/* Clear motor driver fault until state change */
//			if (MC_GetState(pAxis->pMC) == MCSTE_FAULT)
//			{
//				MC_FaultReset(pAxis->pMC);
//				return;
//			}
			*(pAxis->CiA402Var.pErrorCode0x603F) = 0;
			DBG_PRINT("Fault reset\r\n");				

			pAxis->i16State_AXIS2 = STATE_SWITCH_ON_DISABLED;// Transition 15
			DBG_PRINT("Transition 15, FAULT->SWITCH_ON_DISABLED\r\n");							
		}
		break;

	default:    //the sate variable is set to in invalid value => rest Axis
		StatusWord_AXIS2 = (STATUSWORD_STATE_NOTREADYTOSWITCHON);
		pAxis->i16State_AXIS2 = STATE_NOT_READY_TO_SWITCH_ON;
		break;

	}// switch(current state)

	/* Update operational functions (the low level power supply is always TRUE */
	switch(pAxis->i16State_AXIS2)
	{
	case STATE_NOT_READY_TO_SWITCH_ON:
	case STATE_SWITCH_ON_DISABLED:
	case STATE_READY_TO_SWITCH_ON:
		pAxis->bBrakeApplied_AXIS2 = TRUE;
		pAxis->bHighLevelPowerApplied_AXIS2 =  FALSE;
		pAxis->bAxisFunctionEnabled_AXIS2 = FALSE;
		pAxis->bConfigurationAllowed_AXIS2 = TRUE;
		break;
	case STATE_SWITCHED_ON:
		pAxis->bBrakeApplied_AXIS2 = TRUE;
		pAxis->bHighLevelPowerApplied_AXIS2 =  TRUE;
		pAxis->bAxisFunctionEnabled_AXIS2 = FALSE;
		pAxis->bConfigurationAllowed_AXIS2 = TRUE;
		break;
	case STATE_OPERATION_ENABLED:
	case STATE_QUICK_STOP_ACTIVE:
	case STATE_FAULT_REACTION_ACTIVE:
		pAxis->bBrakeApplied_AXIS2 = FALSE;
		pAxis->bHighLevelPowerApplied_AXIS2 =  TRUE;
		pAxis->bAxisFunctionEnabled_AXIS2 = TRUE;
		pAxis->bConfigurationAllowed_AXIS2 = FALSE;
		break;
	case STATE_FAULT:
		pAxis->bBrakeApplied_AXIS2 = TRUE;
		pAxis->bHighLevelPowerApplied_AXIS2 =  FALSE;
		pAxis->bAxisFunctionEnabled_AXIS2 = FALSE;
		pAxis->bConfigurationAllowed_AXIS2 = TRUE;
		break;
	default:
		pAxis->bBrakeApplied_AXIS2 = TRUE;
		pAxis->bHighLevelPowerApplied_AXIS2 =  FALSE;
		pAxis->bAxisFunctionEnabled_AXIS2 = FALSE;
		pAxis->bConfigurationAllowed_AXIS2 = TRUE;
		break;
	}

	if (pAxis->bHighLevelPowerApplied_AXIS2 == TRUE)
	{
		StatusWord_AXIS2 |= STATUSWORD_VOLTAGE_ENABLED;
	}
	else
	{
		StatusWord_AXIS2 &= ~STATUSWORD_VOLTAGE_ENABLED;
	}
		
	/* State transition finished set controlword complete bit and update status object 0x6041 */
	*(pAxis->CiA402Var.pStatuswordForAxis20x6841) = (StatusWord_AXIS2 | STATUSWORD_REMOTE);

	if(nAlStatus != STATE_OP)
	{     
		MotorEnFlag = 0;
		Motor2EnFlag = 0;
		pAxis->i16State = STATE_NOT_READY_TO_SWITCH_ON;		
	}
	
	/* Motor control driver */
    CiA402_MotionControl(pAxis);
	

	
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
 \brief    CiA402-Application
 \brief check if a state transition is pending and pass desired ramp-code to CiA402TransitionAction()
 \brief if this functions returns true the state transition is finished.
*////////////////////////////////////////////////////////////////////////////////////////
void CiA402_Application_AXIS2(CiA402_AXIS_T *pAxis)
{
	/*clear "Drive follows the command value" flag if the target values from the master overwritten by the local application*/
	if(pAxis->u16PendingOptionCode_AXIS2 != 0 &&
			(*(pAxis->CiA402Var.pModesOfOperationDisplay0x6061) == CYCLIC_SYNC_POSITION_MODE ||
			 *(pAxis->CiA402Var.pModesOfOperationDisplay0x6061) == CYCLIC_SYNC_VELOCITY_MODE))
	{
        *(pAxis->CiA402Var.pStatuswordForAxis20x6841) &= ~ STATUSWORD_DRIVE_FOLLOWS_COMMAND;        
	}
  else
	{
        *(pAxis->CiA402Var.pStatuswordForAxis20x6841) |= STATUSWORD_DRIVE_FOLLOWS_COMMAND;        
	}

	switch(pAxis->u16PendingOptionCode_AXIS2)
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
				pAxis->u16PendingOptionCode_AXIS2 = 0x0;
				if(*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) > 0 && *(pAxis->CiA402Var.pQuickStopOptionCode0x605A) < 5)
				{
					pAxis->i16State_AXIS2 = STATE_SWITCH_ON_DISABLED;    //continue state transition 12
					DBG_PRINT("Transition 12(pending), QUICK_STOP_ACTIVE->SWITCH_ON_DISABLED\r\n");
				}
				else if(*(pAxis->CiA402Var.pQuickStopOptionCode0x605A) > 4 && *(pAxis->CiA402Var.pQuickStopOptionCode0x605A) < 9)
                    *(pAxis->CiA402Var.pStatuswordForAxis20x6841) |= STATUSWORD_TARGET_REACHED;                
			}
		}
		break;
	case 0x605B:
		/*state transition 8 is pending analyse shutdown option code (0x605B)*/
		{
			if(CiA402_TransitionAction(*(pAxis->CiA402Var.pShutdownOptionCode0x605B),pAxis))
			{
				/*shutdown ramp is finished complete state transition*/
				pAxis->u16PendingOptionCode_AXIS2 = 0x0;
				pAxis->i16State_AXIS2 = STATE_READY_TO_SWITCH_ON;    //continue state transition 8
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
				pAxis->u16PendingOptionCode_AXIS2 = 0x0;
				pAxis->i16State_AXIS2 = STATE_SWITCHED_ON;    //continue state transition 5
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
				pAxis->u16PendingOptionCode_AXIS2 = 0x0;
				pAxis->i16State_AXIS2 = STATE_FAULT;    //continue state transition 14
//			DBG_PRINT("Transition 4(pending), FAULT_REACTION_ACTIVE->FAULT\r\n");
			}
		}
		break;
	default:
		//pending transition code is invalid => values from the master are used
		if (*(pAxis->CiA402Var.pModesOfOperationDisplay0x6061) != HOMING_MODE)
		{
            *(pAxis->CiA402Var.pStatuswordForAxis20x6841) |= STATUSWORD_DRIVE_FOLLOWS_COMMAND;            
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
	pAxis->bAxisIsActive_AXIS2 = TRUE;
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
                pInputs->ObjModesOfOperationDisplayForAxis2 =       SWAPWORD(0x00FF & *(pAxis->CiA402Var.pModesOfOperationDisplayForAxis20x6861));   

                
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
                pInputs->ObjStatuswordForAxis2 =            SWAPWORD(*(pAxis->CiA402Var.pStatuswordForAxis20x6841));       
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
                pInputs->ObjPositionActualValueForAxis4 =   SWAPWORD(*(pAxis->CiA402Var.pPositionActualValueForAxis40x7864));
                pInputs->ObjVelocityActualValueForAxis4 =   SWAPWORD(*(pAxis->CiA402Var.pVelocityActualValueForAxis40x786C));                   
                pInputs->ObjTorqueActualValueForAxis4 =     SWAPWORD(*(pAxis->CiA402Var.pTorqueActualValueForAxis40x7877));    
                pInputs->ObjENCForAxis4 =                   SWAPWORD(*(pAxis->CiA402Var.pENCForAxis40x787F));                
                pInputs->ObjPositionActualValueForAxis5 =   SWAPWORD(*(pAxis->CiA402Var.pPositionActualValueForAxis50x8064));
                pInputs->ObjVelocityActualValueForAxis5 =   SWAPWORD(*(pAxis->CiA402Var.pVelocityActualValueForAxis50x806C));                   
                pInputs->ObjTorqueActualValueForAxis5 =     SWAPWORD(*(pAxis->CiA402Var.pTorqueActualValueForAxis50x8077));    
                pInputs->ObjENCForAxis5 =                   SWAPWORD(*(pAxis->CiA402Var.pENCForAxis50x807F));                
                pInputs->ObjAnalogInput =                   SWAPWORD(*(pAxis->CiA402Var.pAnalogInput0x6891));
                pInputs->ObjDigitalInput =                  SWAPWORD(*(pAxis->CiA402Var.pDigitalInput0x6892));
                pInputs->ObjErrorMessage =                  SWAPWORD(*(pAxis->CiA402Var.pErrorMessage0x6893));
                pInputs->ObjHeartbeat =                     SWAPWORD(*(pAxis->CiA402Var.pHeartbeat0x68AA));
                pInputs->ObjModesOfOperationDisplay =       SWAPWORD(0x00FF & *(pAxis->CiA402Var.pModesOfOperationDisplay0x6061));   
                pInputs->ObjModesOfOperationDisplayForAxis2 =       SWAPWORD(0x00FF & *(pAxis->CiA402Var.pModesOfOperationDisplayForAxis20x6861));   
           
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
				*(pAxis->CiA402Var.pModesOfOperationForAxis20x6860) = SWAPWORD((pOutputs->ObjModesOfOperationForAxis2 & 0x00FF));
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
				*(pAxis->CiA402Var.pControlwordForAxis20x6840) = SWAPWORD(pOutputs->ObjControlwordForAxis2);                  
				*(pAxis->CiA402Var.pTargetPositionForAxis30x707A) = SWAPWORD(pOutputs->ObjTargetPositionForAxis3);                
				*(pAxis->CiA402Var.pTargetVelocityForAxis30x70FF) = SWAPWORD(pOutputs->ObjTargetVelocityForAxis3);                
				*(pAxis->CiA402Var.pTargetTorqueForAxis30x7071) = SWAPWORD(pOutputs->ObjTargetTorqueForAxis3);
				*(pAxis->CiA402Var.pTargetPositionForAxis40x787A) = SWAPWORD(pOutputs->ObjTargetPositionForAxis4);                
				*(pAxis->CiA402Var.pTargetVelocityForAxis40x78FF) = SWAPWORD(pOutputs->ObjTargetVelocityForAxis4);                
				*(pAxis->CiA402Var.pTargetTorqueForAxis40x7871) = SWAPWORD(pOutputs->ObjTargetTorqueForAxis4);
				*(pAxis->CiA402Var.pTargetPositionForAxis50x807A) = SWAPWORD(pOutputs->ObjTargetPositionForAxis5);                
				*(pAxis->CiA402Var.pTargetVelocityForAxis50x80FF) = SWAPWORD(pOutputs->ObjTargetVelocityForAxis5);                
				*(pAxis->CiA402Var.pTargetTorqueForAxis50x8071) = SWAPWORD(pOutputs->ObjTargetTorqueForAxis5);                
				*(pAxis->CiA402Var.pHostHeartbeat0x68BA) = SWAPWORD(pOutputs->ObjHostHeartbeat);
				*(pAxis->CiA402Var.pDigitalOutput0x68BB) = SWAPWORD(pOutputs->ObjDigitalOutput);
				*(pAxis->CiA402Var.pModesOfOperation0x6060) = SWAPWORD((pOutputs->ObjModesOfOperation & 0x00FF));
				*(pAxis->CiA402Var.pModesOfOperationForAxis20x6860) = SWAPWORD((pOutputs->ObjModesOfOperationForAxis2 & 0x00FF));
				
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
        CiA402_StateMachine_AXIS2(pAxis);
	}
		
	if (pAxis->bAxisIsActive)
	{
		CiA402_Application(pAxis);         //????
	}
    
    
	if (pAxis->bAxisIsActive_AXIS2)
	{
		CiA402_Application_AXIS2(pAxis);         //????
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

unsigned char flag_4k;
void task(void)
{
	CiA402_AXIS_T *pAxis = &CiA402Axis;
	if(flag_4k)
	{
//        if(nAlStatus != STATE_OP)
//        {
//            CiA402_MotionControl(pAxis);            
//        }

		flag_4k=0;
	}
}

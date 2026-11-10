#include "stm32g4xx_hal.h"
#include "motor_control.h"
#include "LinearRamp.h"
#include "mc_type.h"
#include "mc_math.h"
#include "motorcontrol.h"
#include "regular_conversion_manager.h"
#include "mc_interface.h"
#include "mc_tuning.h"
#include "digital_output.h"
#include "state_machine.h"
#include "pwm_common.h"

#include "mc_tasks.h"
#include "parameters_conversion.h"
#include "spi_encoder.h"
#include "stmflash.h"

unsigned char EAngleSave;
short MA600Offsettemp;
short MA600Offset,EangleOffset;
short enc_angle;

extern SPI_HandleTypeDef hspi3;

extern EncAlign_Handle_t EncAlignCtrlM1;
extern unsigned char ENCAlignment;
extern unsigned int ActualState;
void delay()
{
	unsigned char i;
	for(i=0;i<250;i++);
}

short AS5047P_send(short TXD)
{
	int RXD;
	unsigned char TXDtemp,RXDtemp;
	TXDtemp=TXD>>8;
	HAL_SPI_TransmitReceive(&hspi3, &TXDtemp, &RXDtemp, 1,1000);
	RXD=RXDtemp<<8;
	TXDtemp=TXD>>8;
	HAL_SPI_TransmitReceive(&hspi3, &TXDtemp, &RXDtemp, 1,1000);
	RXD|=RXDtemp;
	return RXD;
}

void MT6835_REG_WRITE(unsigned char reg,unsigned char value)
{
	unsigned char TXDtemp,RXDtemp;
	unsigned int temp;
	temp=6<<12|reg;
	SPI1_CS_L;
	TXDtemp=temp>>8;
	HAL_SPI_TransmitReceive(&hspi3, &TXDtemp, &RXDtemp, 1,1000);
	TXDtemp=temp>>0;
	HAL_SPI_TransmitReceive(&hspi3, &TXDtemp, &RXDtemp, 1,1000);
	TXDtemp=value;
	HAL_SPI_TransmitReceive(&hspi3, &TXDtemp, &RXDtemp, 1,1000);
	SPI1_CS_H;
}

int AS5047P_Read(void)
{
	int temp;
	SPI1_CS_L;
	AS5047P_send(4000);
	SPI1_CS_H;
//	delay();
//	SPI1_CS_L;
//	temp=AS5047P_send(0);	
//	temp&=0x3FFF;
//	SPI1_CS_H;
	return temp;
}

void set_Mangle(void)
{
	MT6835_REG_WRITE(0x07,0x1F);
	MT6835_REG_WRITE(0x08,0x40);
	MT6835_REG_WRITE(0x0B,1<<5);
}

int MT6835_ANGLE(void)
{
	unsigned char TXDtemp,RXDtemp;
	int res;
	SPI1_CS_L;
	TXDtemp=0xA0;
	HAL_SPI_TransmitReceive(&hspi3, &TXDtemp, &RXDtemp, 1,1000);
	TXDtemp=0x03;
	HAL_SPI_TransmitReceive(&hspi3, &TXDtemp, &RXDtemp, 1,1000);
	HAL_SPI_TransmitReceive(&hspi3, &TXDtemp, &RXDtemp, 1,1000);
	res=RXDtemp<<16;
	HAL_SPI_TransmitReceive(&hspi3, &TXDtemp, &RXDtemp, 1,1000);
	res|=RXDtemp<<8;
	HAL_SPI_TransmitReceive(&hspi3, &TXDtemp, &RXDtemp, 1,1000);
	res|=RXDtemp;
	res>>=3;
	SPI1_CS_H;
	return res;
}

short MT6835_EANGLE(void)
{
	short angle;
	unsigned int temp;
	float temp1;
	temp=MT6835_ANGLE();
//	temp1=(float)(temp/2097151);
	temp*=0.4375;
	temp%=65536;
	angle=temp;
	angle-=3812;
	angle+=16384;
	
	return angle;
}

void MA600_Wtite_Command_1st(unsigned short cmd)
{
	unsigned char Txtemp,Rxtemp;
	SPI1_CS_L;
	Txtemp=cmd>>8;
	HAL_SPI_TransmitReceive(&hspi3, &Txtemp, &Rxtemp, 1,1000);
	Txtemp=cmd;
	HAL_SPI_TransmitReceive(&hspi3, &Txtemp, &Rxtemp, 1,1000);
	SPI1_CS_H;
}

void MA600_Write_Register_2nd(unsigned char addr,unsigned char value)
{
	unsigned char Txtemp,Rxtemp;
	SPI1_CS_L;
	HAL_SPI_TransmitReceive(&hspi3, &addr, &Rxtemp, 1,1000);
	HAL_SPI_TransmitReceive(&hspi3, &value, &Rxtemp, 1,1000);
	SPI1_CS_H;
}

unsigned char MA600_Read_Register_3rd(void)
{
	unsigned char Txtemp=0,Rxtemp;
	SPI1_CS_L;
	HAL_SPI_TransmitReceive(&hspi3, &Txtemp, &Rxtemp, 1,1000);
	HAL_SPI_TransmitReceive(&hspi3, &Txtemp, &Rxtemp, 1,1000);
	SPI1_CS_H;
	return Rxtemp;
}

unsigned char MA600_Write_Register(unsigned short cmd,unsigned char addr,unsigned char value)
{
	unsigned char temp;
	MA600_Wtite_Command_1st(cmd);
	MA600_Write_Register_2nd(addr,value);
	temp=MA600_Read_Register_3rd();
	
	return temp;
}

void MA600_Init(void)
{
	MA600_Write_Register(0xEA54,0x04,0xE0);
	MA600_Write_Register(0xEA54,0x05,0x7F);
	MA600_Write_Register(0xEA54,0x0E,0x80);
}



short GetMA600Angle(void)
{
	unsigned char temp1,temp2,txtemp=0;
	SPI1_CS_L;
	HAL_SPI_TransmitReceive(&hspi3, &txtemp, &temp1, 1,1000);
	HAL_SPI_TransmitReceive(&hspi3, &txtemp, &temp2, 1,1000);
	SPI1_CS_H;
	
	return (temp1<<8|temp2)-MA600Offset;
}
short c;
short GetMA600EAngle(void)
{
	short temp1;
	float temp2;

	temp1=GetMA600Angle();
	temp1*=3;
	temp1%=32767;
	
	return temp1;
}

void WriteMA600NVM(void)
{
	MA600_Wtite_Command_1st(0xEA55);
	MA600_Write_Register_2nd(0xEA,0x00);
	MA600_Read_Register_3rd();
}
short D;
unsigned char E,F;
void SetMA600EAngle90(void)
{
	EAngleSave=1;
	
	EangleOffset=16384-positon_angle_task();
	ENCAlignment=1;
	ActualState|=ENCAlignment;
}

bool HAL_SPI_TransmitReceive2(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size,
                                          uint32_t Timeout)
{
  uint16_t             initial_TxXferCount;
  uint16_t             initial_RxXferCount;
  uint32_t             tmp_mode;
  HAL_SPI_StateTypeDef tmp_state;
  uint32_t             tickstart;

  /* Variable used to alternate Rx and Tx during transfer */
  uint32_t             txallowed = 1U;
  HAL_StatusTypeDef    errorcode = HAL_OK;

  /* Check Direction parameter */


  /* Process Locked */
  __HAL_LOCK(hspi);

  /* Init tickstart for timeout management*/


  /* Init temporary variables */
  tmp_state           = hspi->State;
  tmp_mode            = hspi->Init.Mode;
  initial_TxXferCount = Size;
  initial_RxXferCount = Size;




  /* Don't overwrite in case of HAL_SPI_STATE_BUSY_RX */
  if (hspi->State != HAL_SPI_STATE_BUSY_RX)
  {
    hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
  }

  /* Set the transaction information */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pRxBuffPtr  = (uint8_t *)pRxData;
  hspi->RxXferCount = Size;
  hspi->RxXferSize  = Size;
  hspi->pTxBuffPtr  = (uint8_t *)pTxData;
  hspi->TxXferCount = Size;
  hspi->TxXferSize  = Size;

  /*Init field not used in handle to zero */
  hspi->RxISR       = NULL;
  hspi->TxISR       = NULL;


   SET_BIT(hspi->Instance->CR2, SPI_RXFIFO_THRESHOLD);

  /* Check if the SPI is already enabled */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);
  }

  
    if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))
    {
      if (hspi->TxXferCount > 1U)
      {
        hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);
        hspi->pTxBuffPtr += sizeof(uint16_t);
        hspi->TxXferCount -= 2U;
      }
      else
      {
        *(__IO uint8_t *)&hspi->Instance->DR = (*hspi->pTxBuffPtr);
        hspi->pTxBuffPtr++;
        hspi->TxXferCount--;
      }
    }
    while ((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U))
    {
      /* Check TXE flag */
      if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)) && (hspi->TxXferCount > 0U))
      {
        if (hspi->TxXferCount > 1U)
        {
          hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);
          hspi->pTxBuffPtr += sizeof(uint16_t);
          hspi->TxXferCount -= 2U;
        }
        else
        {
          *(__IO uint8_t *)&hspi->Instance->DR = (*hspi->pTxBuffPtr);
          hspi->pTxBuffPtr++;
          hspi->TxXferCount--;
        }
        /* Next Data is a reception (Rx). Tx not allowed */
        txallowed = 0U;

      }

      /* Wait until RXNE flag is reset */
      if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) && (hspi->RxXferCount > 0U))
      {
        if (hspi->RxXferCount > 1U)
        {
          *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)hspi->Instance->DR;
          hspi->pRxBuffPtr += sizeof(uint16_t);
          hspi->RxXferCount -= 2U;
          if (hspi->RxXferCount <= 1U)
          {
            /* Set RX Fifo threshold before to switch on 8 bit data size */
            SET_BIT(hspi->Instance->CR2, SPI_RXFIFO_THRESHOLD);
          }
        }
        else
        {
          (*(uint8_t *)hspi->pRxBuffPtr) = *(__IO uint8_t *)&hspi->Instance->DR;
          hspi->pRxBuffPtr++;
          hspi->RxXferCount--;
        }
      }
    }
  
  hspi->State = HAL_SPI_STATE_READY;
  __HAL_UNLOCK(hspi);
  return errorcode;
}

unsigned short GetMA600Angle2(void)
{
	unsigned char temp1,temp2,txtemp=0;
	SPI1_CS_L;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp1, 1,1000);
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp2, 1,1000);
	SPI1_CS_H;
	
	return (temp1<<8|temp2);
}

#define WREN	0x06
#define	WRDI	0x04
#define WRITE	0x02
#define READ	0x03
#define RDSR	0x05

unsigned char state1,state2,state3,state4,state5;

void M95640_WREN()
{
	unsigned char temp,txtemp;
	SPI2_CS_L;
	txtemp=WREN;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp, 1,1000);
	SPI2_CS_H;
}

void M95640_WRDI()
{
	unsigned char temp,txtemp;
	SPI2_CS_L;
	txtemp=WRDI;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp, 1,1000);
	SPI2_CS_H;
}

void M95640_WRITE_BYTE(unsigned short addr,unsigned short data)
{
	unsigned char temp,txtemp;
	SPI2_CS_L;
	txtemp=WRITE;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp, 1,1000);
	txtemp=addr>>8;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp, 1,1000);
	txtemp=addr;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp, 1,1000);
	txtemp=data>>8;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp, 1,1000);
	txtemp=data;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp, 1,1000);
	SPI2_CS_H;
}



unsigned short M95640_READ_BYTE(unsigned short addr)
{
	unsigned char temp1,temp2,txtemp;
	SPI2_CS_L;
	txtemp=READ;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp1, 1,1000);
	txtemp=addr>>8;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp1, 1,1000);
	txtemp=addr;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp1, 1,1000);
	txtemp=0;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp1, 1,1000);
	txtemp=0;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp2, 1,1000);
	SPI2_CS_H;
	
	
	return temp1<<8|temp2;
}

unsigned short M95640_STATUS(void)
{
	unsigned char temp,txtemp;
	SPI2_CS_L;
	txtemp=RDSR;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp, 1,1000);
	txtemp=0;
	HAL_SPI_TransmitReceive2(&hspi3, &txtemp, &temp, 1,1000);
	SPI2_CS_H;
	
	return temp;
}

void M95640_WRITE_DATA(unsigned short addr,unsigned short data)
{
	state1=M95640_STATUS();
	M95640_WREN();
	delay_us(4000);
	state2=M95640_STATUS();
	M95640_WRITE_BYTE(addr,data);
	state3=M95640_STATUS();
	delay_us(4000);
	state4=M95640_STATUS();
	M95640_WRDI();
}
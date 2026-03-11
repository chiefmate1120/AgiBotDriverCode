#ifndef _spi_Simulation_h_
#define _spi_Simulation_h_
#include "AX58200_Hw.h"
#define		true	1
#define 	false   0 

/*
CS:     PB13
CLK:    PA2
MOSI:   PB12
MISO:   PA1
*/



//NSS
#define SPI_CS_GPIO_H		HW_GPIO_WritePin(PE, BIT_5, 1)
#define SPI_CS_GPIO_L		HW_GPIO_WritePin(PE, BIT_5, 0)

//SCK
#define SPI_SCLK_GPIO_H	    HW_GPIO_WritePin(PE, BIT_4, 1)
#define SPI_SCLK_GPIO_L	    HW_GPIO_WritePin(PE, BIT_4, 0)

//MOSI
#define SPI_MOSI_GPIO_H	    HW_GPIO_WritePin(PE, BIT_2, 1)
#define SPI_MOSI_GPIO_L	    HW_GPIO_WritePin(PE, BIT_2, 0)

//MISO
#define SPI_MOSI_GPIO       HW_GPIO_ReadPin(PE,BIT_3)



typedef struct
{
    BOOL FR_CRC_Flag;           //CRC校验成功标志位
    int  FR_CRCData;            //获取的CRC数据
    int  FR_CRCVerify;          //CRC校验结果
    int  FR_ErrorFlag;          //编码器数据报错位
    unsigned int FR_Motor1Data1; //结果
    unsigned int FR_Motor1Data2; //结果    
    unsigned int FR_Motor1Data3; //结果  
    
    //ENC1报错
    int  FR_CRCVerifyErrorTimes;          //编码器连续多次校验失败次数
    int  FR_CRCVerifyErrorTimes_Flag;     //编码器连续多次校验失败标志位
    int  FR_ErrorFlagTimes;               //编码器数据报错次数  
    int  FR_ErrorFlagTimes_Flag;          //编码器数据报错次数标志位
    int  FR_DataBlank_Times;              //编码器无数据反馈次数    
    int  FR_DataBlank_Flag;               //编码器无数据反馈标志位

    
    //ENC2报错    
    int  FR_CRCVerifyErrorTimes_2;          //编码器连续多次校验失败次数
    int  FR_CRCVerifyErrorTimes_Flag_2;     //编码器连续多次校验失败标志位
    int  FR_ErrorFlagTimes_2;               //编码器数据报错次数  
    int  FR_ErrorFlagTimes_Flag_2;          //编码器数据报错次数标志位
    int  FR_DataBlank_Times_2;              //编码器无数据反馈次数    
    int  FR_DataBlank_Flag_2;               //编码器无数据反馈标志位
    
    
    //ENC3报错
    int  FR_CRCVerifyErrorTimes_3;          //编码器连续多次校验失败次数
    int  FR_CRCVerifyErrorTimes_Flag_3;     //编码器连续多次校验失败标志位
    int  FR_ErrorFlagTimes_3;               //编码器数据报错次数  
    int  FR_ErrorFlagTimes_Flag_3;          //编码器数据报错次数标志位
    int  FR_DataBlank_Times_3;              //编码器无数据反馈次数    
    int  FR_DataBlank_Flag_3;          //编码器无数据反馈标志位
    
    int  FR_ErrorFeedback;                //报错反馈      
    //编码器模式
    int  FR_Mode_1;
    int  FR_Mode_2;   
    int  FR_Mode_3;  
}FR_ENCClass;
extern FR_ENCClass FR_ENC;

void SimulationSPI_init(void);
unsigned int SimulationSPI_FRENCRead(void);
void SimulationSPI_TransmitReceive(UINT8 *pTxData, UINT8 *pRxData);
uint8_t biss_crc32_lookup(uint32_t in_data32);
extern int FRReadVersion(void);
extern void FRWriteJZ(void);




#endif

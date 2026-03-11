#ifndef _spi_ENC3_h
#define _spi_ENC3_h
#include "AX58200_Hw.h"
#define		ENC3_spi_cs_H	HW_GPIO_WritePin(PB, BIT_13, 1)
#define		ENC3_spi_cs_L	HW_GPIO_WritePin(PB, BIT_13, 0)



//typedef struct
//{
//    BOOL FR_CRC_Flag;           //CRC校验成功标志位
//    int  FR_CRCData;            //获取的CRC数据
//    int  FR_CRCVerify;          //CRC校验结果
//    int  FR_ErrorFlag;          //编码器数据报错位
//    unsigned int FR_Motor1Data1; //结果
//    unsigned int FR_Motor1Data2; //结果    
//    unsigned int FR_Motor1Data3; //结果  
//    
//    //ENC1报错
//    int  FR_CRCVerifyErrorTimes;          //编码器连续多次校验失败次数
//    int  FR_CRCVerifyErrorTimes_Flag;     //编码器连续多次校验失败标志位
//    int  FR_ErrorFlagTimes;               //编码器数据报错次数  
//    int  FR_ErrorFlagTimes_Flag;          //编码器数据报错次数标志位
//    int  FR_DataBlank_Times;              //编码器无数据反馈次数    
//    int  FR_DataBlank_Flag;               //编码器无数据反馈标志位

//    
//    //ENC2报错    
//    int  FR_CRCVerifyErrorTimes_2;          //编码器连续多次校验失败次数
//    int  FR_CRCVerifyErrorTimes_Flag_2;     //编码器连续多次校验失败标志位
//    int  FR_ErrorFlagTimes_2;               //编码器数据报错次数  
//    int  FR_ErrorFlagTimes_Flag_2;          //编码器数据报错次数标志位
//    int  FR_DataBlank_Times_2;              //编码器无数据反馈次数    
//    int  FR_DataBlank_Flag_2;               //编码器无数据反馈标志位
//    
//    
//    //ENC3报错
//    int  FR_CRCVerifyErrorTimes_3;          //编码器连续多次校验失败次数
//    int  FR_CRCVerifyErrorTimes_Flag_3;     //编码器连续多次校验失败标志位
//    int  FR_ErrorFlagTimes_3;               //编码器数据报错次数  
//    int  FR_ErrorFlagTimes_Flag_3;          //编码器数据报错次数标志位
//    int  FR_DataBlank_Times_3;              //编码器无数据反馈次数    
//    int  FR_DataBlank_Flag_3;          //编码器无数据反馈标志位
//    
//    int  FR_ErrorFeedback;                //报错反馈      
//    //编码器模式
//    int  FR_Mode_1;
//    int  FR_Mode_2;   
//    int  FR_Mode_3;  
//}FR_ENCClass;
//extern FR_ENCClass FR_ENC;



extern void ENC3_spi_init(void);
extern unsigned int ENC3_read(void);
extern int32_t FR_ENC1_readInt(uint8_t motor, uint8_t address);
extern void ENC3_spi_init(void);
extern int32_t ICPZENC_readInt(uint8_t motor, uint8_t address);
//uint8_t biss_crc32_lookup(uint32_t in_data32);
//extern int FRReadVersion(void);
//extern void FRWriteJZ(void);
#endif
//#include <stdio.h> 
//#include <stdlib.h>
//#include <stdarg.h> 
#include "stm32G4xx_hal.h"
#include "stm32g4xx_hal_flash_ex.h"
#include "stm32g4xx_hal_flash.h"
#include "spi_encoder.h"

//FLASH_BASE = 0x8000000 (~0x0801FFFF)   
//FLASH_PAGE_SIZE = 2048(2KB)

#define FLASH_USER_Page 63 //STM32G431=128KB，页数最大63                                                                         
#define FLASH_USER_START_ADDR (FLASH_BASE + (FLASH_USER_Page * FLASH_PAGE_SIZE)) /* 用户Flash区域的起始@ */
#define FLASH_USER_END_ADDR   (FLASH_BASE + FLASH_SIZE - 1)                      /* 使用者Flash区域的结束@ */       

#define FLASH_USER_START_ADDR2 (FLASH_BASE + (54 * FLASH_PAGE_SIZE))
#define FLASH_USER_END_ADDR2   (FLASH_BASE + (63 * FLASH_PAGE_SIZE) - 1)

uint32_t FirstPage = 0, NbOfPages = 0, Address = 0, PageError = 0;                                                                   
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;                                                    

extern void Test_Flash(void);
extern void Flash_Read(uint32_t *pBuffer, uint32_t NumToRead); 
extern void Flash_Write(uint32_t *pBuffer, uint32_t  NumToWrite);                                                                                                                                                                                         
extern void Error_Handler(void);
extern uint32_t GetPage(uint32_t Addr);                                                     
extern void Flash_Erase(void);

extern signed char IQ[4100];
extern unsigned char ENCAlignment;
extern unsigned int ActualState;

uint8_t Flash_Buffer[10]={1,2,3,4,5,6,7,8,9,10};//要写入到STM32 FLASH的字符串数组
uint8_t Read_Buffer[sizeof(Flash_Buffer)];      //Flash读取缓存数组                                                                                            
void Test_Flash(void)                                                                                         
{                                                                                                                          
  Flash_Write((uint32_t *)Flash_Buffer, sizeof(Flash_Buffer));  
  Flash_Read((uint32_t *)Read_Buffer, sizeof(Flash_Buffer));                                                                                                                                                                                                                                                                           
}   

static uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
} 

static FLASH_EraseInitTypeDef EraseInitStruct = {0};                                                                                                 
void Flash_Erase(void)                                                                            
{                                                                                                    
  FirstPage = GetPage(FLASH_USER_START_ADDR);                                                                                                                                                     
  /* 取得从第一页开始擦除的页数 */                                             
  NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;                                                                                                                                              
  /* 填满 EraseInit 结构*/                                                                     
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES; //页擦除                                               
  EraseInitStruct.Page      = FirstPage;             //擦除地址                                              
  EraseInitStruct.NbPages   = NbOfPages;             //擦除页数     
  //printf("%d %d %d\r\n", FLASH_TYPEERASE_PAGES, FirstPage, NbOfPages);                                                                                                                                            
  /* 注意：如果快闪记忆体中的擦除操作也涉及资料或指令快取中的数据，
      您必须确保这些资料在程式码期间被存取之前被重写
      执行。 如果不能安全地完成此操作，建议透过设定刷新快取
      FLASH_CR 暂存器中的 DCRST 和 ICRST 位元。 */ 
  
  //擦除FLASH                                                  
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)                                       
  {                                                                                                                                                              
    while(1){Error_Handler();} /*擦除时发生错误*/                                                                                                       
  }                                                                                          
}                                                                                                 

//FLASH_TYPEPROGRAM_BYTE       0x00000000U 1Byte(8bit) 
//FLASH_TYPEPROGRAM_HALFWORD   0x00000001U 2Byte(16bit) 
//FLASH_TYPEPROGRAM_WORD       0x00000002U 4Byte(32bit) 
//FLASH_TYPEPROGRAM_DOUBLEWORD 0x00000003U 8Byte(64bit)                          
void Flash_Write(uint32_t *pBuffer, uint32_t  NumToWrite)
{
  uint16_t  i = 0;
  uint32_t Address = FLASH_USER_START_ADDR;
  //强制转换后写入数组长度向上取整(SIZE+3)/4
  NumToWrite = (NumToWrite + 3) / 4;
  HAL_FLASH_Unlock();    //解锁
  Flash_Erase();      //先擦除
  //写入
  while((Address<FLASH_USER_END_ADDR) && (i<NumToWrite))     
  {
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, pBuffer[i]) == HAL_OK)
    {
      Address = Address + 8; 
      i++;
    }
    else
      {  
      Error_Handler( );            
      }
  }
  HAL_FLASH_Lock();   //上锁 
}  

void Flash_Read(uint32_t *pBuffer, uint32_t NumToRead)                    
{                                                                                       
  uint16_t i = 0;                                                    
  uint32_t Address = FLASH_USER_START_ADDR; 
  //强制转换后写入数组长度向上取整(SIZE+3)/4
  NumToRead = (NumToRead + 3) / 4;                                                                                      
  while((Address<FLASH_USER_END_ADDR)&&(i<NumToRead))  
  {                                                              
    pBuffer[i++]= *(__IO uint32_t *)Address;                          
    Address = Address + 8;                        
  }                                                                                                                           
}                                                                

uint32_t Flash_Address_Read(uint32_t address)
{    
  return *(uint32_t*)address; 
}

void Flash_Erase2(void)                                                                            
{                                                                                                    
  FirstPage = GetPage(FLASH_USER_START_ADDR2);                                                                                                                                                     
  /* 取得从第一页开始擦除的页数 */                                             
  NbOfPages = GetPage(FLASH_USER_END_ADDR2) - FirstPage + 1;                                                                                                                                              
  /* 填满 EraseInit 结构*/                                                                     
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES; //页擦除                                               
  EraseInitStruct.Page      = FirstPage;             //擦除地址                                              
  EraseInitStruct.NbPages   = NbOfPages;             //擦除页数     
  //printf("%d %d %d\r\n", FLASH_TYPEERASE_PAGES, FirstPage, NbOfPages);                                                                                                                                            
  /* 注意：如果快闪记忆体中的擦除操作也涉及资料或指令快取中的数据，
      您必须确保这些资料在程式码期间被存取之前被重写
      执行。 如果不能安全地完成此操作，建议透过设定刷新快取
      FLASH_CR 暂存器中的 DCRST 和 ICRST 位元。 */ 
  
  //擦除FLASH                                                  
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)                                       
  {                                                                                                                                                              
    while(1){Error_Handler();} /*擦除时发生错误*/                                                                                                       
  }                                                                                          
}

void Flash_Write2(uint32_t *pBuffer, uint32_t  NumToWrite)
{
  uint16_t  i = 0;
  uint32_t Address = FLASH_USER_START_ADDR2;
  //强制转换后写入数组长度向上取整(SIZE+3)/4
  NumToWrite = (NumToWrite + 3) / 4;
  HAL_FLASH_Unlock();    //解锁
  Flash_Erase2();      //先擦除
  //写入
  while((Address<FLASH_USER_END_ADDR2) && (i<NumToWrite))     
  {
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, pBuffer[i]) == HAL_OK)
    {
      Address = Address + 8; 
      i++;
    }
    else
      {  
      Error_Handler( );            
      }
  }
  HAL_FLASH_Lock();   //上锁 
}  

void Flash_Read2(uint32_t *pBuffer, uint32_t NumToRead)                    
{                                                                                       
  uint16_t i = 0;                                                    
  uint32_t Address = FLASH_USER_START_ADDR2; 
  //强制转换后写入数组长度向上取整(SIZE+3)/4
  NumToRead = (NumToRead + 3) / 4;                                                                                      
  while((Address<FLASH_USER_END_ADDR2)&&(i<NumToRead))  
  {                                                              
    pBuffer[i++]= *(__IO uint32_t *)Address;                          
    Address = Address + 8;                        
  }                                                                                                                           
}

void DataRead(void)
{
	Flash_Read2((uint32_t *)IQ, sizeof(IQ));
}

void IQ_save(void)
{
	Flash_Write2((uint32_t *)IQ, sizeof(IQ));
}

#include <stdio.h>
#include "AX58200_Hw.h"
#include "FMC_DATA.h"
#include "fmc.h"
#include "task_tmc4671.h"

#define APROM_TEST_BASE             0x20000      /* APROM test start address                 */
#define DATA_FLASH_TEST_BASE        0x30000      /* Data Flash test start address            */
#define DATA_FLASH_TEST_END         0x30004      /* Data Flash test end address              */
#define TEST_PATTERN                0x5A5A5A5A   /* Test pattern                             */

unsigned char fmc_test_data;

unsigned int PID[20];




static int  set_data_flash_base(uint32_t u32DFBA)
{
    uint32_t   au32Config[2];          /* User Configuration */

    /* Read User Configuration 0 & 1 */
    if (FMC_ReadConfig(au32Config, 2) < 0)
    {
        printf("\nRead User Config failed!\n");       /* Error message */
        return -1;                     /* failed to read User Configuration */
    }

    /* Check if Data Flash is enabled and is expected address. */
    if ((!(au32Config[0] & 0x1)) && (au32Config[1] == u32DFBA))
        return 0;                      /* no need to modify User Configuration */

    FMC_ENABLE_CFG_UPDATE();           /* Enable User Configuration update. */

    au32Config[0] &= ~0x1;             /* Clear CONFIG0 bit 0 to enable Data Flash */
    au32Config[1] = u32DFBA;           /* Give Data Flash base address  */

    /* Update User Configuration settings. */
    if (FMC_WriteConfig(au32Config, 2) < 0)
        return -1;                     /* failed to write user configuration */

    printf("\nSet Data Flash base as 0x%x.\n", DATA_FLASH_TEST_BASE);  /* debug message */

    /* Perform chip reset to make new User Config take effect. */
    SYS->IPRST0 = SYS_IPRST0_CHIPRST_Msk;
    return 0;                          /* success */
}

int32_t fill_data_pattern(uint32_t u32StartAddr, uint32_t u32EndAddr, uint32_t u32Pattern)
{
    uint32_t u32Addr,i=0;                  /* flash address */

    /* Fill flash range from u32StartAddr to u32EndAddr with data word u32Pattern. */
	for (u32Addr = u32StartAddr; u32Addr <u32StartAddr+4*14; u32Addr += 4)
    {
        FMC_Write(u32Addr, PID[i++]);
    }
    return 0;
}

int32_t  verify_data(uint32_t u32StartAddr, uint32_t u32EndAddr, uint32_t u32Pattern)
{
    uint32_t    u32Addr;               /* flash address */
    uint32_t    u32data;               /* flash data    */

    /* Verify if each data word from flash u32StartAddr to u32EndAddr be u32Pattern.  */
    for (u32Addr = u32StartAddr; u32Addr < u32EndAddr; u32Addr += 4)
    {
        u32data = FMC_Read(u32Addr);   /* Read a flash word from address u32Addr. */

//        if (g_FMC_i32ErrCode != 0)
//        {
//            printf("FMC_Read address 0x%x failed!\n", u32Addr);
//            return -1;
//        }

        if (u32data != u32Pattern)     /* Verify if data matched. */
        {
            printf("\nFMC_Read data verify failed at address 0x%x, read=0x%x, expect=0x%x\n", u32Addr, u32data, u32Pattern);
            return -1;                 /* data verify failed */
        }
    }
    return 0;                          /* data verify OK */
}


int32_t  flash_test(uint32_t u32StartAddr, uint32_t u32EndAddr, uint32_t u32Pattern)
{
    uint32_t    u32Addr;               /* flash address */

	u32Addr=u32StartAddr;
	if (FMC_Erase(u32Addr) != 0)            /* Erase page */
    {
         printf("FMC_Erase address 0x%x failed!\n", u32Addr);
         return -1;
    }
	if (fill_data_pattern(u32Addr, u32Addr + u32EndAddr, u32Pattern) != 0)
            return -1;
    
    printf("\r    Flash Test Passed.          \n");                  /* information message */
    return 0;      /* flash test passed */
}

void fmc_tset(void)
{
	SYS_UnlockReg();                   /* Unlock register lock protect */

    FMC_Open();                        /* Enable FMC ISP function */
	
	set_data_flash_base(DATA_FLASH_TEST_BASE);

//	FMC_ENABLE_LD_UPDATE();
//	flash_test(FMC_LDROM_BASE, FMC_LDROM_END, 88);
//	fmc_test_data=FMC_Read(FMC_LDROM_BASE);
//	FMC_DISABLE_LD_UPDATE();
	
	FMC_ENABLE_AP_UPDATE(); 
	flash_test(APROM_TEST_BASE, APROM_TEST_BASE+1000, 55);
	FMC_DISABLE_AP_UPDATE();
	fmc_test_data=FMC_Read(APROM_TEST_BASE+4);
	FMC_Close();
	SYS_LockReg();
}

void Fmc_PID_Write(void)
{
	SYS_UnlockReg();                   /* Unlock register lock protect */

    FMC_Open();                        /* Enable FMC ISP function */
	
	set_data_flash_base(DATA_FLASH_TEST_BASE);
	
	FMC_ENABLE_AP_UPDATE(); 
	flash_test(APROM_TEST_BASE, APROM_TEST_BASE+80, 0);
	FMC_DISABLE_AP_UPDATE();
	FMC_Close();
	SYS_LockReg();
}

void FMC_PID_Read(void)
{
	SYS_UnlockReg();                   /* Unlock register lock protect */

    FMC_Open();                        /* Enable FMC ISP function */
	
	set_data_flash_base(DATA_FLASH_TEST_BASE);

	
	fmc_test_data=FMC_Read(APROM_TEST_BASE+4*5);
	if(FMC_Read(APROM_TEST_BASE+4*5)==55)
	{
		
		PID_Motor1.Current_P=FMC_Read(APROM_TEST_BASE+4*0);
		PID_Motor1.Current_I=FMC_Read(APROM_TEST_BASE+4*1);
		PID_Motor1.Velocity_P=FMC_Read(APROM_TEST_BASE+4*2);
		PID_Motor1.Velocity_I=FMC_Read(APROM_TEST_BASE+4*3);
		PID_Motor1.Position_P=FMC_Read(APROM_TEST_BASE+4*4);
		PID_Motor1.Position_I=FMC_Read(APROM_TEST_BASE+4*6);
		
		PID_Motor2.Current_P=FMC_Read(APROM_TEST_BASE+4*7);
		PID_Motor2.Current_I=FMC_Read(APROM_TEST_BASE+4*8);
		PID_Motor2.Velocity_P=FMC_Read(APROM_TEST_BASE+4*9);
		PID_Motor2.Velocity_I=FMC_Read(APROM_TEST_BASE+4*10);
		PID_Motor2.Position_P=FMC_Read(APROM_TEST_BASE+4*11);
		PID_Motor2.Position_I=FMC_Read(APROM_TEST_BASE+4*12);
		
	}
	FMC_Close();
	SYS_LockReg();
}

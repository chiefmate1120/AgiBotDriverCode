#ifndef _FMC_DATA_H
#define _FMC_DATA_H




extern void fmc_tset(void);
extern unsigned char fmc_test_data;
extern unsigned int PID[20];

extern void Fmc_PID_Write(void);
extern void FMC_PID_Read(void);

#endif
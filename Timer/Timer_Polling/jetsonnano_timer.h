#ifndef __JETSONNANO_TIMER_H
#define __JETSONNANO_TIMER_H

#include "jetsonnano.h"

typedef enum
{
	CLK_M_12MHZ = 0,
	CLK_M_38_4MHZ
}CLK_M_TypeDef;

typedef enum
{
	TMR_Mode_Normal = 0,
	TMR_Mode_Periodic
}TMRMode_TypeDef;

typedef struct
{
	TMRMode_TypeDef TMR_Mode;
	
	uint32_t TMR_PTV;
	
}TMR_InitTypeDef;

void TMR_Init(uint32_t TMRx, TMR_InitTypeDef* TMR_InitStruct);
void TMR_Cmd(uint32_t TMRx, FunctionalState NewState);
void TMR_SetCounter(uint32_t TMRx, uint32_t _tmr_ptv);
uint32_t TMR_GetCounter(uint32_t TMRx);
void TMR_ConfigTMRUS(CLK_M_TypeDef CLK_M);
uint32_t TMR_GetTMRUS_1US(void);
void TMR_ClearITPending(uint32_t TMRx);

#endif
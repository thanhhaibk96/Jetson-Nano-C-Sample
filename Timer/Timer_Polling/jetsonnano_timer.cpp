#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>

#include "jetsonnano_timer.h"

static int tmr_fd = 0;
static int tmr_pagesize = 0;
static int tmr_pagemask = 0;
static void *tmr_base;

void TMR_Init(uint32_t TMRx, TMR_InitTypeDef* TMR_InitStruct)
{
	uint32_t tmp = 0;
	//  read physical memory (needs root)
	tmr_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (tmr_fd < 0) {
		perror("/dev/mem");
		fprintf(stderr, "please run this program as root (for example with sudo)\n");
		exit(1);
	}
	
	//  map a particular physical address into our address space
	tmr_pagesize = getpagesize();
	tmr_pagemask = tmr_pagesize-1;
	
	tmr_base = mmap(0, tmr_pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, tmr_fd, (TMR_BASE & ~tmr_pagemask));
	if (tmr_base == NULL) {
		perror("mmap()");
		exit(1);
	}
	
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile TMR_TypeDef  *TMRx_Map = (TMR_TypeDef volatile *)((char *)tmr_base + (TMRx & tmr_pagemask));
	
	tmp = ((uint32_t) TMR_InitStruct->TMR_Mode << 30) | TMR_InitStruct -> TMR_PTV;
	TMRx_Map->PTV = tmp;
}

void TMR_Cmd(uint32_t TMRx, FunctionalState NewState)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile TMR_TypeDef  *TMRx_Map = (TMR_TypeDef volatile *)((char *)tmr_base + (TMRx & tmr_pagemask));
	
	TMRx_Map->PTV |= (uint32_t) NewState << 31;
	//printf("%x \n",TMRx_Map->PTV);
}

void TMR_SetCounter(uint32_t TMRx, uint32_t _tmr_ptv)
{
	uint32_t tmp = 0;
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile TMR_TypeDef  *TMRx_Map = (TMR_TypeDef volatile *)((char *)tmr_base + (TMRx & tmr_pagemask));
	tmp = TMRx_Map->PTV & 0xC0000000;
	tmp |= _tmr_ptv;
	TMRx_Map->PTV = tmp;
}

uint32_t TMR_GetCounter(uint32_t TMRx)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile TMR_TypeDef  *TMRx_Map = (TMR_TypeDef volatile *)((char *)tmr_base + (TMRx & tmr_pagemask));
	
	return (TMRx_Map->PCR & 0x1FFFFFFF);
}

void TMR_ConfigTMRUS(CLK_M_TypeDef CLK_M)
{
	uint32_t USEC_DIVIEND = 0, USEC_DIVISOR = 0;
	
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile TMRUS_TypeDef  *TMRUS_Map = (TMRUS_TypeDef volatile *)((char *)tmr_base + (TMRUS & tmr_pagemask));
	
	if(CLK_M == CLK_M_12MHZ)
	{
		USEC_DIVIEND = 0x00;
		USEC_DIVISOR = 0x0B;
		TMRUS_Map->USEC_CFG = (USEC_DIVIEND << 8) | USEC_DIVISOR;
	}
	else if(CLK_M == CLK_M_38_4MHZ)
	{
		USEC_DIVIEND = 0x04;
		USEC_DIVISOR = 0xBF;
		TMRUS_Map->USEC_CFG = (USEC_DIVIEND << 8) | USEC_DIVISOR;
	}
}

uint32_t TMR_GetTMRUS_1US(void)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile TMRUS_TypeDef  *TMRUS_Map = (TMRUS_TypeDef volatile *)((char *)tmr_base + (TMRUS & tmr_pagemask));
	
	return TMRUS_Map->CNTR_1US;
}

void TMR_ClearITPending(uint32_t TMRx)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile TMR_TypeDef  *TMRx_Map = (TMR_TypeDef volatile *)((char *)tmr_base + (TMRx & tmr_pagemask));
	
	TMRx_Map->PCR &= 0xBFFFFFFF;
}

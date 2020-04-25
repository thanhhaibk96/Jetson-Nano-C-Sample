#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>

#include "jetsonnano_gpio.h"

static int gpio_fd = 0;
static int gpio_pagesize = 0;
static int gpio_pagemask = 0;
static void *gpio_base;

void GPIO_Init(uint32_t GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
	//  read physical memory (needs root)
	gpio_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (gpio_fd < 0) {
		perror("/dev/mem");
		fprintf(stderr, "please run this program as root (for example with sudo)\n");
		exit(1);
	}
	
	//  map a particular physical address into our address space
	gpio_pagesize = getpagesize();
	gpio_pagemask = gpio_pagesize-1;
	
	gpio_base = mmap(0, gpio_pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, gpio_fd, (GPIO_1_BASE & ~gpio_pagemask));
	if (gpio_base == NULL) {
		perror("mmap()");
		exit(1);
	}
	
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile GPIO_TypeDef  *GPIOx_Map = (GPIO_TypeDef volatile *)((char *)gpio_base + (GPIOx & gpio_pagemask));
	
	//printf("Print: %x \n", GPIOx_Map);
	
	uint32_t pinpos = 0x00, pos = 0x00, currentpin = 0x00;
	
	for(pinpos = 0x00; pinpos < 0x10; pinpos++)
	{
		pos = ((uint8_t) 0x01) << pinpos;
		
		currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
		
		if(currentpin == pos)
		{
			//printf("Print1: %x \n", (((uint32_t) GPIO_InitStruct->GPIO_Mode) & 0x01) << pinpos);
			//printf("Print2: %x \n", ((((uint32_t) GPIO_InitStruct->GPIO_Mode) & 0x02) >> 1) << pinpos);
			GPIOx_Map->CNF |= (((uint32_t) GPIO_InitStruct->GPIO_Mode) & 0x01) << pinpos;
			
			GPIOx_Map->OE |= ((((uint32_t) GPIO_InitStruct->GPIO_Mode) & 0x02) >> 1) << pinpos;
		}
	}
}

void GPIO_SetBits(uint32_t GPIOx, uint8_t GPIO_Pin)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile GPIO_TypeDef  *GPIOx_Map = (GPIO_TypeDef volatile *)((char *)gpio_base + (GPIOx & gpio_pagemask));
	
	GPIOx_Map->OUT = GPIO_Pin;
}

void GPIO_ResetBits(uint32_t GPIOx, uint8_t GPIO_Pin)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile GPIO_TypeDef  *GPIOx_Map = (GPIO_TypeDef volatile *)((char *)gpio_base + (GPIOx & gpio_pagemask));
	
	GPIOx_Map->OUT = GPIOx_Map->OUT & (~GPIO_Pin);
}

void GPIO_ToggleBits(uint32_t GPIOx, uint8_t GPIO_Pin)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile GPIO_TypeDef  *GPIOx_Map = (GPIO_TypeDef volatile *)((char *)gpio_base + (GPIOx & gpio_pagemask));
	
	GPIOx_Map->OUT ^= GPIO_Pin;
}

uint8_t GPIO_ReadInputDataBit(uint32_t GPIOx, uint8_t GPIO_Pin)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile GPIO_TypeDef  *GPIOx_Map = (GPIO_TypeDef volatile *)((char *)gpio_base + (GPIOx & gpio_pagemask));
	
	uint8_t bitstatus = 0x00;
	if((GPIOx_Map->IN & GPIO_Pin) != (uint8_t)Bit_RESET)
	{
		bitstatus = (uint8_t)Bit_SET;
	}
	else
	{
		bitstatus = (uint8_t)Bit_RESET;
	}
	
	return bitstatus;
}

uint8_t GPIO_ReadIntputData(uint32_t GPIOx)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile GPIO_TypeDef  *GPIOx_Map = (GPIO_TypeDef volatile *)((char *)gpio_base + (GPIOx & gpio_pagemask));
	
	return GPIOx_Map->IN;
}

void GPIO_InterruptCmd(uint32_t GPIOx, uint8_t GPIO_Pin, FunctionalState NewState)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile GPIO_TypeDef  *GPIOx_Map = (GPIO_TypeDef volatile *)((char *)gpio_base + (GPIOx & gpio_pagemask));
	
	if(NewState) GPIOx_Map->INT_ENB |=  GPIO_Pin;
	else GPIOx_Map->INT_ENB &=  ~GPIO_Pin;
}

void GPIO_InterruptInit(uint32_t GPIOx, uint8_t GPIO_Pin, EXTIMode_TypeDef EXTI_Mode)
{
	//  set up a pointer for convenient access -- this pointer is to the selected GPIO controller
	volatile GPIO_TypeDef  *GPIOx_Map = (GPIO_TypeDef volatile *)((char *)gpio_base + (GPIOx & gpio_pagemask));
	
	//printf("Print2: %x \n", ((EXTI_Mode & 0x01) << GPIO_Pin) | ((EXTI_Mode & 0x02) << (8 - 1 + GPIO_Pin -1)) |  ((EXTI_Mode & 0x04) << (16 - 2 + GPIO_Pin - 1)));
	GPIOx_Map->INT_LVL = ((EXTI_Mode & 0x01) << GPIO_Pin) | ((EXTI_Mode & 0x02) << (8 - 1 + GPIO_Pin)) |  ((EXTI_Mode & 0x04) << (16 - 2 + GPIO_Pin));
	
}
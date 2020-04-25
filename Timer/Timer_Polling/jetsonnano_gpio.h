#ifndef __JETSONNANO_GPIO_H
#define __JETSONNANO_GPIO_H

#include "jetsonnano.h"

/** @defgroup GPIO_pins_define
   * @{
   */
#define GPIO_Pin_0				((uint8_t)0x01)
#define GPIO_Pin_1				((uint8_t)0x02)
#define GPIO_Pin_2				((uint8_t)0x04)
#define GPIO_Pin_3				((uint8_t)0x08)
#define GPIO_Pin_4				((uint8_t)0x10)
#define GPIO_Pin_5				((uint8_t)0x20)
#define GPIO_Pin_6				((uint8_t)0x40)
#define GPIO_Pin_7				((uint8_t)0x80)
#define GPIO_Pin_ALL			((uint8_t)0xFF)

typedef enum
{
	GPIO_Mode_SPIO = 0x00,
	
	GPIO_Mode_OUT = 0x03,
	
	GPIO_Mode_IN = 0x01
	
}GPIOMode_TypeDef;

typedef enum
{
	Bit_RESET = 0,
	Bit_SET
}BitAction;

typedef enum
{
	EXTI_Mode_Level_LOW = 0x00,
	EXTI_Mode_Level_HIGH = 0x01,
	EXTI_Mode_Trigger_RISING = 0x03,
	EXTI_Mode_Trigger_FALLING = 0x02,
	EXTI_Mode_Trigger_BOTH = 0x06
}EXTIMode_TypeDef;

typedef struct
{
	uint8_t GPIO_Pin;
	
	GPIOMode_TypeDef GPIO_Mode;
	
}GPIO_InitTypeDef;

void GPIO_Init(uint32_t GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_SetBits(uint32_t GPIOx, uint8_t GPIO_Pin);
void GPIO_ResetBits(uint32_t GPIOx, uint8_t GPIO_Pin);
void GPIO_ToggleBits(uint32_t GPIOx, uint8_t GPIO_Pin);
uint8_t GPIO_ReadInputDataBit(uint32_t GPIOx, uint8_t GPIO_Pin);
uint8_t GPIO_ReadIntputData(uint32_t GPIOx);
void GPIO_InterruptCmd(uint32_t GPIOx, uint8_t GPIO_Pin, FunctionalState NewState);
void GPIO_InterruptInit(uint32_t GPIOx, uint8_t GPIO_Pin, EXTIMode_TypeDef EXTI_Mode);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>

#include "jetsonnano.h"
#include "jetsonnano_gpio.h"
#include "jetsonnano_timer.h"

using namespace std;

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	
	GPIO_Init(GPIO_BB, &GPIO_InitStruct);
	
	GPIO_InterruptCmd(GPIO_BB, GPIO_Pin_0, DISABLE);
	GPIO_ResetBits(GPIO_BB, GPIO_Pin_0);	
	
	TMR_InitTypeDef TMR_InitStruct;
	TMR_InitStruct.TMR_Mode = TMR_Mode_Periodic;
	TMR_InitStruct.TMR_PTV = 1000000;
	TMR_Init(TMR0, &TMR_InitStruct);
	TMR_Cmd(TMR0, ENABLE);
	
	while(true)
	{		
		GPIO_ToggleBits(GPIO_BB, GPIO_Pin_0);
		//printf("Timer 0: %d \n", TMR_GetCounter(TMR0));
		printf("%s \n","Hello!!!, My name is Hai");
		//sleep(1);
		while(TMR_GetCounter(TMR0));
	}
	return 0;
}
/*
 * main.c
 *
 *  Created on: Feb 28, 2021
 *      Author: MykolaV
 */


#include "stm32f10x.h"
#include "main.h"

#ifdef SECOND_WAY

void delayMs(uint32_t ms);

int main()
{
	initGPIO();

	while(1)
	{
		PWM(50, 25, 20);															//50% GPIOA1, 25% GPIOB9, 20 ms period
	}
}

void PWM(float dutyCyclePercent1, float dutyCyclePercent2, int period)
{
	int GPIOAWorkTime = period/(100/dutyCyclePercent1);						//Calculate first delay
	int GPIOBWorkTime = period/(100/dutyCyclePercent2);						//Calculate second delay
	GPIOA->BSRR |= GPIO_BSRR_BS1;											//Set GPIOA 1 pin
	GPIOB->BSRR |= GPIO_BSRR_BS9;											//Set GPIOB 9 pin
	if(dutyCyclePercent1 > dutyCyclePercent2)
	{
		delayMs(GPIOBWorkTime);
		GPIOB->BRR |= GPIO_BRR_BR9;											//Reset GPIOB 9 pin
		delayMs(GPIOAWorkTime-GPIOBWorkTime);
		GPIOA->BRR |= GPIO_BRR_BR1;											//Reset GPIOA 1 pin
		delayMs(period-GPIOAWorkTime);
	}
	else
	{
		delayMs(GPIOAWorkTime);
		GPIOA->BRR |= GPIO_BRR_BR1;											//Reset GPIOA 1 pin
		delayMs(GPIOBWorkTime-GPIOAWorkTime);
		GPIOB->BRR |= GPIO_BRR_BR9;											//Reset GPIOB 9 pin
		delayMs(period-GPIOBWorkTime);
	}
}

void initGPIO(void){
	RCC->APB2ENR|= RCC_APB2ENR_IOPAEN;										//Enable GPIOA clock
	RCC->APB2ENR|= RCC_APB2ENR_IOPBEN;										//Enable GPIOB clock

	GPIOA->CRL |= (GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1);					//50 MHz out mode
	GPIOA->CRL &= ~GPIO_CRL_CNF1;											//Push&Pull

	GPIOB->CRH |= (GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1);					//50 MHz out mode
	GPIOB->CRH &= ~GPIO_CRH_CNF9;											//Push&Pull
}

void delayMs(uint32_t ms)
{
	for(uint32_t i=0; i<ms; i++)
		for(uint32_t j=0; j<9550; j++) {}
}

#endif
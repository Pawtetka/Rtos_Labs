#include "stm32f10x.h"
#include "main.h"

#ifdef THIRD_WAY

void _delay_ms(uint32_t ms)
{
	for(uint32_t i=0; i<ms; i++)
		for(uint32_t j=0; j<9550; j++) {}
}

void PWM(float dutyCyclePercent1, float dutyCyclePercent2, int period)
{
	int GPIOAWorkTime = period/(100/dutyCyclePercent1);
	int GPIOBWorkTime = period/(100/dutyCyclePercent2);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);												//Set GPIOA 1 pin
	GPIO_SetBits(GPIOB, GPIO_Pin_9);												//Set GPIOB 9 pin
	if(dutyCyclePercent1 > dutyCyclePercent2)
	{
		_delay_ms(GPIOBWorkTime);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);										//Reset GPIOB 9 pin
		_delay_ms(GPIOAWorkTime-GPIOBWorkTime);
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);										//Reset GPIOA 1 pin
		_delay_ms(period-GPIOAWorkTime);
	}
	else
	{
		_delay_ms(GPIOAWorkTime);
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);										//Reset GPIOA 1 pin
		_delay_ms(GPIOBWorkTime-GPIOAWorkTime);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);										//Reset GPIOB 9 pin
		_delay_ms(period-GPIOBWorkTime);
	}
}

int main()
{
	initGPIO();
	
	while(1)
	{
		PWM(50,25,20);																								//50% GPIOA1, 25% GPIOB9, 20 ms period
	}
}

void initGPIO(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//Enable GPIOB clock
	
	GPIO_InitTypeDef  GPIO_InitStructureA1;													//Init structure
	GPIO_InitTypeDef  GPIO_InitStructureB9;													//Init second structure

	
	GPIO_InitStructureA1.GPIO_Pin = GPIO_Pin_1;											//GPIOA pin1
	GPIO_InitStructureA1.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureA1.GPIO_Speed = GPIO_Speed_50MHz;							//Set structure field GPIO_Pin as GPIO_Pin_1
	
	GPIO_InitStructureB9.GPIO_Pin = GPIO_Pin_9;											//GPIOB pin9
	GPIO_InitStructureB9.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureB9.GPIO_Speed = GPIO_Speed_50MHz;							//50 MHz out mode
	
	GPIO_Init(GPIOA, &GPIO_InitStructureA1);												//Init GPIOA Pin1
	GPIO_Init(GPIOB, &GPIO_InitStructureB9);												//Init GPIOB Pin9
}

#endif
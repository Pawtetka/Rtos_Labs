#include "main.h"

volatile uint32_t ticks_delay;

void initGPIO(void);
void TIM2_Init(void);

void _delay_ms(uint32_t ms);
void PWM(uint16_t dutyCycle, uint16_t period, GPIO_TypeDef* port, uint16_t pin);

void pwm_1(void* argument);
void pwm_2(void* argument);

int main(void)
{
	initGPIO();
	TIM2_Init();
	
	osKernelInitialize();
	osThreadNew(pwm_1, GPIOA, NULL);
	osThreadNew(pwm_1, GPIOB, NULL);
	osThreadNew(pwm_2, GPIOA, NULL);
	osThreadNew(pwm_2, GPIOB, NULL);
	osKernelStart();
	
	while(1)
	{
		
	}
}

void initGPIO(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//Enable GPIOB clock
	
	GPIO_InitTypeDef  GPIO_InitStructure;														//Init pin initializaion structure
	
	GPIO_StructInit(&GPIO_InitStructure);														//Fill structure's fields with default values	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_9;					//GPIO pin1 & pin9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;								//Push&Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;								//2 MHz out mode
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);													//Init GPIOA Pin1 & Pin9
	GPIO_Init(GPIOB, &GPIO_InitStructure);													//Init GPIOB Pin1 & Pin9	
}

void _delay_ms(uint32_t ms)
{
	uint32_t start = ticks_delay;
  while((ticks_delay - start) < ms);
}

void PWM(uint16_t dutyCycle, uint16_t period, GPIO_TypeDef* port, uint16_t pin)
{
	GPIO_SetBits(port, pin);												
	_delay_ms(dutyCycle);
	GPIO_ResetBits(port, pin);
	_delay_ms(period - dutyCycle);
}

void pwm_1(void* argument)
{
	while(1){
		PWM(10, 20, argument, GPIO_Pin_1);
	}
}

void pwm_2(void* argument)
{
	while(1){
		PWM(5, 20, argument, GPIO_Pin_9);
	}
}

void TIM2_Init(void)
{
	TIM_TimeBaseInitTypeDef timer;																	//Time Base Init structure
	NVIC_InitTypeDef initNVIC;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);						//Enable TIM2 clock
		
	TIM_TimeBaseStructInit(&timer);																	//Fill structures's fields with default values
	timer.TIM_Prescaler = SystemCoreClock/10000 - 1;								
	timer.TIM_Period = 9;																						//Specifies the period value to be loaded into the active Auto-Reload Register					
	timer.TIM_ClockDivision = 1;																		//Specifies the clock division
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &timer);																	//Init TIM2
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);																					//Start TIM2
	
	initNVIC.NVIC_IRQChannel = TIM2_IRQn;
	initNVIC.NVIC_IRQChannelPreemptionPriority = 0x00;
	initNVIC.NVIC_IRQChannelSubPriority = 0x00;
	initNVIC.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&initNVIC);
}

void TIM2_IRQHandler()
{
	ticks_delay++;
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

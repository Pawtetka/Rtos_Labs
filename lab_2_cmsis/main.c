#include "main.h"

void TIM2_PWM_Init(uint16_t period);
void initGPIO(void);
void SysTick_Init(void);
void ChangeSysTickStatus(void);
void incrTIM2PulseWidth(void);

int main(void){
	//init peripherals
	initGPIO();
	SysTick_Init();
	TIM2_PWM_Init(199);
	
	while(1){
		if (checkButtonClick(GPIOB->IDR & GPIO_IDR_IDR0)){						//if btn on PB0 pressed
			incrTIM2PulseWidth();																				//increment TIM2 PWM pulse width
			_delay_ms(100);
		}
		if (checkButtonClick(GPIOB->IDR & GPIO_IDR_IDR1)){						//if btn on PB1 pressed
			ChangeSysTickStatus();																			//toggle SysTick
			_delay_ms(1000);
		}
	}
}

void initGPIO(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//Enable GPIOA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						//Enable GPIOB clock
	
	GPIO_InitTypeDef  GPIO_InitStructureA1;													//Init pin initializaion structures
	GPIO_InitTypeDef  GPIO_InitStructureB0_B1;											
	GPIO_InitTypeDef 	port;
	
	GPIO_StructInit(&GPIO_InitStructureA1);													//Fill structures's fields with default values
	GPIO_StructInit(&GPIO_InitStructureB0_B1);
	GPIO_StructInit(&port);
	
	GPIO_InitStructureA1.GPIO_Pin = GPIO_Pin_1;											//GPIOA pin1
	GPIO_InitStructureA1.GPIO_Mode = GPIO_Mode_Out_PP;							//Push&Pull
	GPIO_InitStructureA1.GPIO_Speed = GPIO_Speed_2MHz;							//2 MHz out mode
	
	GPIO_InitStructureB0_B1.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;			//GPIOB pin0 & pin1
	GPIO_InitStructureB0_B1.GPIO_Mode = GPIO_Mode_IPU;							//input pull-up
	GPIO_InitStructureB0_B1.GPIO_Speed = GPIO_Speed_2MHz;						//2 MHz in mode
	
	port.GPIO_Mode = GPIO_Mode_AF_PP;																//Alternate function Push&Pull
	port.GPIO_Pin = GPIO_Pin_0;																			//GPIOA pin0
	port.GPIO_Speed = GPIO_Speed_2MHz;															//2 MHz out mode
	
	GPIO_Init(GPIOA, &GPIO_InitStructureA1);												//Init GPIOA Pin1
	GPIO_Init(GPIOB, &GPIO_InitStructureB0_B1);											//Init GPIOB Pin0 & Pin1	
	GPIO_Init(GPIOA, &port);																				//Init GPIOA Pin0
}

void TIM2_PWM_Init(uint16_t period)
{
	TIM_TimeBaseInitTypeDef timer;																	//Time Base Init structure
	TIM_OCInitTypeDef timerPWM;																			//Output Compare Init structure
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);						//Enable TIM2 clock
		
	TIM_TimeBaseStructInit(&timer);																	//Fill structures's fields with default values
	timer.TIM_Prescaler = SystemCoreClock/40000U - 1;								//Specifies the prescaler value to one tick every 0.025 ms
	timer.TIM_Period = period;																			//Specifies the period value to be loaded into the active Auto-Reload Register					
	timer.TIM_ClockDivision = 1;																		//Specifies the clock division
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &timer);																	//Init TIM2
	
	TIM_OCStructInit(&timerPWM);																		//Fill structures's fields with default values
	timerPWM.TIM_OCMode = TIM_OCMode_PWM1;													//Specifies the TIM mode as PWM1
	timerPWM.TIM_OutputState = TIM_OutputState_Enable;
	timerPWM.TIM_Pulse = 120;																				//Set PWM pulse width (relative to period)
	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;									//Set PWM polarity to high (pulse is high signal)
	TIM_OC1Init(TIM2, &timerPWM);																		//init TIM2 output compare mode
	
	TIM_Cmd(TIM2, ENABLE);																					//Start TIM2
}

void incrTIM2PulseWidth(void)
{
	if (TIM2->CCR1 == ((TIM2->ARR+1)/100*60))												//if pulse width is already 60% (in TIM2->CCR1 value 120)
		TIM2->CCR1 = 1;																								//set pulse width to 0.5%
	else 
		TIM2->CCR1 += 1;																							//else increment pulse width by 0.5%
}

char checkButtonClick(uint32_t pin)
{
	if (!pin){
		_delay_ms(20);
		if (!pin)
			return TRUE;
	}
		
	return FALSE;
}

void _delay_ms(uint32_t ms)
{
	for(uint32_t i=0; i<ms; i++)
		for(uint32_t j=0; j<9550; j++) {}
}

void SysTick_Init(void)
{
	SysTick->LOAD = SystemCoreClock/100 - 1;																	//set SysTick reload period to 10 ms
	SysTick->VAL = SystemCoreClock/100 - 1;	
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk; 		//set SysTick clock source to internal and allow SysTick interrrupts
}

void ChangeSysTickStatus(void)
{
	if ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) == SysTick_CTRL_ENABLE_Msk) //if SysTick enabled,
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;															//disable it
	else
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;																//else enable
}

void SysTick_Handler(void) {
  GPIOA->ODR ^= GPIO_ODR_ODR1;																							//toggle GPIOA_Pin1
}
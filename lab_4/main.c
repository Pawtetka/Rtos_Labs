#include "main.h"

const osThreadAttr_t thread1_attr = {
  .priority = osPriorityAboveNormal                           
};

struct funcAttr{
	volatile uint16_t* counter;
	GPIO_TypeDef* port;
};

struct funcAttr func_attr1;
struct funcAttr func_attr2;

osThreadId_t idThread1, idThread2;
osTimerId_t idTimer1, idTimer2;

volatile uint16_t counter_1 = 0;
volatile uint16_t counter_2 = 0;

void pwm_thread_func(void *argument);
void timer_callback(void *argument);

int main(void)
{
	initGPIO();
	
	func_attr1.counter = &counter_1;
	func_attr1.port = GPIOA;
	
	func_attr2.counter = &counter_2;
	func_attr2.port = GPIOB;
	
	osKernelInitialize();
	
	idThread1 = osThreadNew(pwm_thread_func, GPIOA, &thread1_attr);
	idThread2 = osThreadNew(pwm_thread_func, GPIOB, NULL);
	
	idTimer1 = osTimerNew(timer_callback, osTimerPeriodic, &func_attr1, NULL);
	idTimer2 = osTimerNew(timer_callback, osTimerPeriodic, &func_attr2, NULL);
	
	osTimerStart(idTimer1, 5U);
	osTimerStart(idTimer2, 5U);
	
	osKernelStart();
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

void PWM(uint16_t dutyCycle, uint16_t period, GPIO_TypeDef* port, uint16_t pin)
{
	GPIO_SetBits(port, pin);												
	osDelay(dutyCycle);
	GPIO_ResetBits(port, pin);
	osDelay(period - dutyCycle);
}

void pwm_thread_func(void* argument)
{
	while(1){
		PWM(10, 20, argument, GPIO_Pin_1);
	}
}

void timer_callback(void *argument)
{
	struct funcAttr *attrPointer = argument;
	switch (*(attrPointer->counter))
	{
		case 0:
			GPIO_ResetBits(attrPointer->port, GPIO_Pin_9);
			++*(attrPointer->counter);
		break;
		case 1:
		case 2:
			++*(attrPointer->counter);
		break;
		case 3:
			GPIO_SetBits(attrPointer->port, GPIO_Pin_9);
			*(attrPointer->counter) = 0;
		break;		
	}
}

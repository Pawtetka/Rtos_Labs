#include "main.h"
#include "gpio.h"

osThreadId_t idThread1, idThread2, idBtnThread;

osEventFlagsId_t event_id;

void PWM(uint16_t dutyCycle, uint16_t period, GPIO_TypeDef* port, uint16_t pin);

void pwm_1(void* argument);
void pwm_2(void* argument);
void btn_thread(void* argument);

int main(void){
	initGPIO();
	
	event_id = osEventFlagsNew(NULL);
	
	osKernelInitialize();
	
	idThread1 = osThreadNew(pwm_1, NULL, NULL);
	idThread2 = osThreadNew(pwm_2, NULL, NULL);
	idBtnThread = osThreadNew(btn_thread, NULL, NULL);
	
	osKernelStart();
}

char checkButtonClick(uint32_t pin)
{
	if (!pin){
		osDelay(20);
		if (!pin)
			return TRUE;
	}
		
	return FALSE;
}


void PWM(uint16_t dutyCycle, uint16_t period, GPIO_TypeDef* port, uint16_t pin)
{
	GPIO_SetBits(port, pin);												
	osDelay(dutyCycle);
	GPIO_ResetBits(port, pin);
	osDelay(period - dutyCycle);
}

void btn_thread(void* argument){
	char isClicked;
	
	while(1){
		isClicked = checkButtonClick(GPIOC->IDR & GPIO_IDR_IDR14);
		if (isClicked){																					
			osEventFlagsSet(event_id, BTN_CLICKED_FLAG_1);
			osEventFlagsSet(event_id, BTN_CLICKED_FLAG_2);
			osDelay(1000);
		}
	}	
}

void pwm_1(void* argument){
	uint16_t period = 20;
	uint16_t width = 10;
	uint32_t flags;
	
	while(1){
		PWM(width, period, GPIOA, GPIO_Pin_1);
		
		flags = osEventFlagsGet(event_id);
		if (flags & BTN_CLICKED_FLAG_1){
			if(period > 500) {
				period = 10;
				width = 5;
			}
			period *= 2;
			width *= 2;
			osEventFlagsClear(event_id, BTN_CLICKED_FLAG_1);
		}
	}
}

void pwm_2(void* argument){
	uint16_t period = 20;
	uint16_t width = 5;
	uint32_t flags;
	
	while(1){
		PWM(width, period, GPIOB, GPIO_Pin_9);
		
		flags = osEventFlagsGet(event_id);
		if (flags & BTN_CLICKED_FLAG_2){
			if(width == 1) {
				period = 40;
				width = 10;
			}
			period /= 2;
			width /= 2;
			osEventFlagsClear(event_id, BTN_CLICKED_FLAG_2);
		}		
	}
}
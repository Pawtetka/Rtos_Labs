#ifndef MAIN_H
#define MAIN_H

#include "stm32f10x.h"
#include "cmsis_os2.h"

void initGPIO(void);
void PWM(uint16_t dutyCycle, uint16_t period, GPIO_TypeDef* port, uint16_t pin);

#endif

#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"

#define TRUE 0x01
#define FALSE 0x00

char checkButtonClick(uint32_t pin);
void _delay_ms(uint32_t ms);

#endif /*_MAIN_H*/
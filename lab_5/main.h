#ifndef MAIN_H
#define MAIN_H

#include "stm32f10x.h"
#include "cmsis_os2.h"

#define TRUE 0x01
#define FALSE 0x00
#define BTN_CLICKED_FLAG_1 0x01U
#define BTN_CLICKED_FLAG_2 0x02U

char checkButtonClick(uint32_t pin);

#endif

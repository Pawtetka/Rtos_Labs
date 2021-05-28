/*
 * main.h
 *
 *  Created on: Feb 28, 2021
 *      Author: MykolaV
 */

//#define SECOND_WAY
#define THIRD_WAY

#ifndef CORE_INC_MAIN_H_
#define CORE_INC_MAIN_H_

void initGPIO(void);
void PWM(float dutyCyclePercent1, float dutyCyclePercent2, int period);

#endif /* CORE_INC_MAIN_H_ */

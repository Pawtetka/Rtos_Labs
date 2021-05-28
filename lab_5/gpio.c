#include "gpio.h"

void initGPIO(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);						
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);						

	GPIO_InitTypeDef  GPIO_InitStructure_C14;														
	GPIO_InitTypeDef GPIO_InitStructure_A1;
	GPIO_InitTypeDef GPIO_InitStructure_B9;
	
	GPIO_InitStructure_C14.GPIO_Pin = GPIO_Pin_14;			
	GPIO_InitStructure_C14.GPIO_Mode = GPIO_Mode_IPU;							
	GPIO_InitStructure_C14.GPIO_Speed = GPIO_Speed_2MHz;						
	
	GPIO_Init(GPIOC, &GPIO_InitStructure_C14);
	
	GPIO_InitStructure_A1.GPIO_Pin = GPIO_Pin_1;											
	GPIO_InitStructure_A1.GPIO_Mode = GPIO_Mode_Out_PP;							
	GPIO_InitStructure_A1.GPIO_Speed = GPIO_Speed_2MHz;							
	
	GPIO_InitStructure_B9.GPIO_Pin = GPIO_Pin_9;											
	GPIO_InitStructure_B9.GPIO_Mode = GPIO_Mode_Out_PP;							
	GPIO_InitStructure_B9.GPIO_Speed = GPIO_Speed_2MHz;							

	GPIO_Init(GPIOA, &GPIO_InitStructure_A1);	
	GPIO_Init(GPIOB, &GPIO_InitStructure_B9);
}
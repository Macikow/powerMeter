/*
 * led.c
 *
 *  Created on: Jul 3, 2016
 *      Author: Macikow
 */


#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"

//PC9 - GEEEN LED
//PC8 - BLUE LED
#define LED_BLUE GPIO_Pin_8
#define LED_GREEN GPIO_Pin_9

void ledInit(void){

	 GPIO_InitTypeDef        GPIO_InitStructure;

	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);


	 GPIO_InitStructure.GPIO_Pin = LED_BLUE | LED_GREEN;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void ledGreenOn(void)
{
	GPIOC->BSRR = LED_GREEN;
}
void ledGreenOff(void)
{
	GPIOC->BRR = LED_GREEN;
}
void ledBlueOn(void)
{
	GPIOC->BSRR = LED_BLUE;
}
void ledBlueOff(void)
{
	GPIOC->BRR = LED_BLUE;
}

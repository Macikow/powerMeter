/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**
**  Distribution: The file is distributed "as is", without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include <stddef.h>
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "led.h"
#include "uart.h"
#include <inttypes.h>
#include <stdio.h>
/* Private typedef */

/* Private define  */
/* STM32F0_DISCOVERY board definitions */
#define LED_PORT GPIOC
#define LED1 GPIO_Pin_9
#define LED2 GPIO_Pin_8

#define KEY_PORT GPIOA
#define KEY GPIO_Pin_0

#define SYS_ERROR 1
#define SYS_OK 0

/* Private macro */

/* Private variables */

/* Private function prototypes */

/* Private functions */

/* Global variables */
uint32_t timer=0;
uint8_t  timerFlag=0;
uint8_t wait =0;
/**
**===========================================================================
**
**  Abstract: SysTick interrupt handler
**
**===========================================================================
*/
unsigned char SysTickConfig(uint32_t ClockSource, uint32_t tick){

	assert_param(IS_SYSTICK_CLK_SOURCE(ClockSource));
	if(tick > SysTick_LOAD_RELOAD_Msk) return 1;
	SysTick->LOAD = (tick & SysTick_LOAD_RELOAD_Msk) -1; // wartosc po przeladowaniu
	NVIC_SetPriority(SysTick_IRQn,0);
	SysTick->VAL = 0; // Wartosc poczatkowa
	if(ClockSource == SysTick_CLKSource_HCLK){
		SysTick->CTRL |=  SysTick_CLKSource_HCLK;
	}
	else  SysTick->CTRL &=  SysTick_CLKSource_HCLK_Div8;

	SysTick->CTRL |= (1<<SysTick_CTRL_TICKINT_Pos) | (1<<SysTick_CTRL_ENABLE_Pos);
	return 0;
}

void SysTick_Handler(void)
{
	wait = 0;
	SysTick->CTRL &= (0<<SysTick_CTRL_TICKINT_Pos);
}

unsigned char SysTick_delay(char * time_type ,unsigned char delay_time){

	char *ms = "ms" ; /* m s \0   */
	char  *us = "us";
	unsigned char msCounter =0, usCounter=0;
	const uint16_t msFactor = 6000;
	const uint16_t usFactor = 6;
	SysTick->CTRL &= (0<< SysTick_CTRL_ENABLE_Pos);
	SysTick->CTRL &= (0<< SysTick_CTRL_TICKINT_Pos);
	SysTick->VAL =0;


	while (*time_type != '\0'){
		if(*(time_type) == *(ms++)) msCounter++;
		if(*(time_type++) == *(us++)) usCounter++;
	}
	if(msCounter == 2) //
	{
		uint32_t temp = delay_time * msFactor;
		if(temp > 0xffffff) return 1;
		SysTick->LOAD = delay_time * msFactor;
		SysTick->CTRL |= (1<<SysTick_CTRL_TICKINT_Pos) | (1<<SysTick_CTRL_ENABLE_Pos);
		wait = 1;

	}
	else if(usCounter == 2 )
	{
		uint32_t temp = delay_time * usFactor;
		if(temp > 0xffffff) return 1;
		SysTick->LOAD = delay_time * usFactor;
		SysTick->CTRL |= (1<<SysTick_CTRL_TICKINT_Pos) | (1<<SysTick_CTRL_ENABLE_Pos);
		wait = 1;
	}
	while(wait);
	return 0;
}

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
  uint32_t ii = 0;
  GPIO_InitTypeDef        GPIO_InitStructure;
  ledInit();
  uartInit();
  SystemCoreClockUpdate();

  /* TODO - Add your application code here */
  SysTick_Config(4800);  /* 0.1 ms = 100us if clock frequency 12 MHz */
  ii = SystemCoreClock;    /* This is a way to read the System core clock */
  ii = 0;
  printf("printf dziala");
  /* GPIOA-C Periph clock enable */


  /* Configure
   * PA0 USER Button input
   * */


  /* Configure PC8 and PC9 in output pushpull mode
   * PC8 = LD3 Green LED
   * PC9 = LD4 Blue LED
   * */

  while (1)
  {

	  if(SysTick_delay("ms" , 500)) ;
	  ledGreenOff();
	  SysTick_delay("ms" , 500);
	  ledGreenOn();

  }
  return 0;
}

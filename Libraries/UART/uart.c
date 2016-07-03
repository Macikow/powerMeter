/*
 * uart.c
 *
 *  Created on: Jul 3, 2016
 *      Author: Macikow
 */
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "led.h"
#include <inttypes.h>
struct {
	char txData[255];
	char rxData[255];
	unsigned char txCounter;
	unsigned char txCount;
	unsigned char rxCounter;
}sUSART1;


void uartInit()
{
	 USART_InitTypeDef USART_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;

	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

	  /* Configure USART1 pins:  Rx and Tx ----------------------------*/
	  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* Enable USART1 IRQ */
	  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	  USART_InitStructure.USART_BaudRate = 9600;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  USART_Init(USART1, &USART_InitStructure);

	  USART_Cmd(USART1,ENABLE);

	 // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void uartSimpleSend(char * data)
{
	while(*data != '\0'){
		while(!(USART1->ISR & (1<<7)));
		USART1->TDR = *data++;
	}
}


void ClearRxBUffer(void)
{
	unsigned char i;
	for( i = 0 ; i < 256 ; i++)
	{
		sUSART1.rxData[i] = '\0';
	}
}

void CleartxBuffer(void)
{
	unsigned char i;
		for( i = 0 ; i < 256 ; i++)
		{
			sUSART1.txData[i] = '\0';
		}
}

void USART1_IRQHandler(void)
{
	char temp ;
	if(USART1->ISR & (1<<5)){ //RXNE
		temp = USART1->RDR;
		if(temp == 0x0d)
		{
			 sUSART1.rxData[sUSART1.rxCounter] = '\0';
			 uartSimpleSend(&sUSART1.rxData[0]);
			 sUSART1.rxCounter=0;
			 while(!(USART1->ISR & (1<<7)));
			 USART1->TDR = '\n';
			 while(!(USART1->ISR & (1<<7)));
			 USART1->TDR = '\r';

		}
		else
		{
			sUSART1.rxData[sUSART1.rxCounter++] = temp;
			USART1->TDR = temp;
		}

	}
	if((USART1->ISR & (1<<7)) == 0) {

	}
}


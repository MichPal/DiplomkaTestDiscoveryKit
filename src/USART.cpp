/*
 * USART.cpp
 *
 *  Created on: 29. 3. 2017
 *      Author: Michal
 */

#include "USART.h"

USART::USART(USART_TypeDef * _USART, uint32_t _BaudRate)
{
	this->USARTx = _USART;
	this->BaudRate = _BaudRate;

	this->i = 0;
	this->receivedBytes = 0;

	InitGPIO();
	InitUSART();


}

void USART::InitGPIO()
{
	if(this->USARTx == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
		this->GPIOx = GPIOA;
		this->GPIO_RX = GPIO_PinSource10;
		this->GPIO_TX = GPIO_PinSource9;
	}
	else if(this->USARTx == USART2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
		this->GPIOx = GPIOA;
		this->GPIO_RX = GPIO_PinSource3;
		this->GPIO_TX = GPIO_PinSource2;

	}
	else if(this->USARTx == USART3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
		this->GPIOx = GPIOB;
		this->GPIO_RX = GPIO_PinSource11;
		this->GPIO_TX = GPIO_PinSource10;
	}
/*	else if(this->USARTx == USART4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART4,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
		this->GPIOx = GPIOA;
		this->GPIO_RX = GPIO_PinSource1;
		this->GPIO_TX = GPIO_PinSource0;
	}
	else if(this->USARTx == USART5)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART5,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
		this->GPIOx = GPIOB;
		this->GPIO_RX = GPIO_PinSource4;
		this->GPIO_TX = GPIO_PinSource3;
	}
	else if(this->USARTx == USART6)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
		this->GPIOx = GPIOA;
		this->GPIO_RX = GPIO_PinSource1;
		this->GPIO_TX = GPIO_PinSource0;
	}
*/


	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure USART pins */
	GPIO_InitStructure.GPIO_Pin = (uint16_t)(1 << this->GPIO_RX) | (uint16_t)(1 << this->GPIO_TX);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(this->GPIOx, &GPIO_InitStructure);

	GPIO_PinAFConfig(this->GPIOx,this->GPIO_RX,GPIO_AF_7);
	GPIO_PinAFConfig(this->GPIOx,this->GPIO_TX,GPIO_AF_7);
}

void USART::InitUSART()
{
	/* USART setup */
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_InitStructure.USART_BaudRate = this->BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(this->USARTx, &USART_InitStructure);

	/* Enable USART interrupt */
	USART_ITConfig(this->USARTx,USART_IT_RXNE,ENABLE);

	if(this->USARTx == USART1)	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	else if(this->USARTx == USART2) NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	else NVIC_InitStructure.NVIC_IRQChannel = USART3_6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(this->USARTx,ENABLE);
}

void USART::InterruptHandler()
{
	if(USART_GetFlagStatus(this->USARTx,USART_FLAG_RXNE))
	{
		this->received[this->receivedBytes] = USART_ReceiveData(this->USARTx);
		if(this->received[this->receivedBytes] == USART_START_BYTE || this->receivedBytes > 0)
		{
			if(this->received[this->receivedBytes] == USART_STOP_BYTE)
			{
				this->receivedBytes = 0;
				SendPacket(this->received,5);
			}
			else
				this->receivedBytes++;
		}
		USART_ClearITPendingBit(this->USARTx,USART_IT_RXNE);
	}

	else if(USART_GetFlagStatus(this->USARTx,USART_FLAG_TXE))
	{
		if(this->i < this->bufferLenght)
		{
			USART_SendData(this->USARTx,this->buffer[this->i++]);
		}
		else
		{
			USART_ITConfig(this->USARTx,USART_IT_TXE,DISABLE);
		}
	}
}

uint8_t USART::SendPacket(char *data ,uint8_t lenght)
{
	if (this->i >= this->bufferLenght)
	{
		this->i = 0;
		this->bufferLenght = lenght;
		memcpy(this->buffer,data,lenght);
		USART_ITConfig(this->USARTx,USART_IT_TXE,ENABLE);
		return 1;
	}
	else return 0; // busy

}

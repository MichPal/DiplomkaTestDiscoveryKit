/*
 * USART.h
 *
 *  Created on: 29. 3. 2017
 *      Author: Michal
 */

#ifndef USART_H_
#define USART_H_

//#include "stm32f0xx.h"
#include "stm32f30x.h"
#include <string.h>

#define USART_START_BYTE 0xE1
#define USART_STOP_BYTE	0xF1

class USART {
private:
	USART_TypeDef *USARTx;
	GPIO_TypeDef *GPIOx;
	uint8_t GPIO_RX;
	uint8_t GPIO_TX;
	uint32_t BaudRate;
	char buffer[30];
	char received[30];
	uint8_t receivedBytes;
	uint8_t i;
	uint8_t bufferLenght;


public:
	USART(USART_TypeDef * _USART, uint32_t _BaudRate);
	void InterruptHandler(void);
	uint8_t SendPacket(char *data ,uint8_t lenght);
private:
	void InitGPIO(void);
	void InitUSART(void);
};

#endif /* USART_H_ */

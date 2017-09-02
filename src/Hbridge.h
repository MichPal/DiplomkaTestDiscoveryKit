/*
 * Hbridge.h
 *
 *  Created on: 28. 3. 2017
 *      Author: Michal
 */

#ifndef HBRIDGE_H_
#define HBRIDGE_H_

//#include "stm32f0xx.h"
#include "stm32f30x.h"


class H_bridge {
private:
	uint8_t EnableLeft;
	uint8_t EnableRight;
	uint8_t ModePin1;
	uint8_t ModePin2;
	uint8_t ModePin3;
	uint8_t ModePin4;
	GPIO_TypeDef *ModeGPIO;
	TIM_OCInitTypeDef OCInit;

public:
	H_bridge(uint8_t _EnableLeft, uint8_t _EnableRight, GPIO_TypeDef *_ModeGPIO, uint8_t _ModePin1, uint8_t _ModePin2, uint8_t _ModePin3, uint8_t _ModePin4);
	void Move(int16_t speedL, int16_t speedR);
private:
	void InitGPIO(void);
	void InitPWM(void);
};

#endif /* HBRIDGE_H_ */

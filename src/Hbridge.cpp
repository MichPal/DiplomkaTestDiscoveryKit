/*
 * Hbridge.cpp
 *
 *  Created on: 28. 3. 2017
 *      Author: Michal
 */

#include "Hbridge.h"

H_bridge::H_bridge(uint8_t _EnableLeft, uint8_t _EnableRight, GPIO_TypeDef *_ModeGPIO, uint8_t _ModePin1, uint8_t _ModePin2, uint8_t _ModePin3, uint8_t _ModePin4)
{
	this->EnableLeft = _EnableLeft;
	this->EnableRight = _EnableRight;
	this->ModePin1 = _ModePin1;
	this->ModePin2 = _ModePin2;
	this->ModePin3 = _ModePin3;
	this->ModePin4 = _ModePin4;
	this->ModeGPIO = _ModeGPIO;

	InitGPIO();
	InitPWM();
}

void H_bridge::InitGPIO(void)
{
	if(ModeGPIO == GPIOA)	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	else if(ModeGPIO == GPIOB)	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	else if(ModeGPIO == GPIOC)	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	else if(ModeGPIO == GPIOD)	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD,ENABLE);
	else if(ModeGPIO == GPIOE)	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = (uint16_t)(1 << this->ModePin1) | (uint16_t)(1 << this->ModePin2) | (uint16_t)(1 << this->ModePin3) | (uint16_t)(1 << this->ModePin4);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ModeGPIO,&GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//(uint16_t)(1 << this->EnableLeft)| (uint16_t)(1 << this->EnableRight);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_1);

}

void H_bridge::InitPWM(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);

	//TODO: cele prerobit
	TIM_TimeBaseInitTypeDef TimeBase;

    uint16_t prescaler = (uint16_t) ((48000000 / 1000000) - 1); // Shooting for 1 MHz, (1us)

    uint32_t pwm_period = 20000;

	TimeBase.TIM_ClockDivision=0;
	TimeBase.TIM_CounterMode=TIM_CounterMode_Up;
	TimeBase.TIM_Period=100-1;
	TimeBase.TIM_Prescaler=12-1;
	TIM_TimeBaseInit(TIM2,&TimeBase);

	this->OCInit.TIM_OCMode=TIM_OCMode_PWM1;
	this->OCInit.TIM_OCPolarity=TIM_OCPolarity_Low;
	this->OCInit.TIM_OutputState=TIM_OutputState_Enable;
	this->OCInit.TIM_Pulse=50-1;
	TIM_OC2Init(TIM2,&this->OCInit);
	TIM_OC1Init(TIM2,&this->OCInit);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

void H_bridge::Move(int16_t speedL, int16_t speedR)
{
	// IN1	IN2
	//	0	1	-backward
	//	1	0	-forward
	//	1	1	-brake
	// IN3	IN4
	//	0	1	-backward
	//	1	0	-forward
	//	1	1	-brake
	if (speedL > 0)
	{
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin1),Bit_SET);
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin2),Bit_RESET);
	}
	else if(speedL < 0)
	{
		speedL = -speedL;
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin1),Bit_RESET);
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin2),Bit_SET);
	}
	else	//speedL=0
	{
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin1),Bit_SET);
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin2),Bit_SET);
	}

	if (speedR > 0)
	{
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin3),Bit_SET);
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin4),Bit_RESET);
	}
	else if(speedR < 0)
	{
		speedR = -speedR;
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin3),Bit_RESET);
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin4),Bit_SET);
	}
	else	//speedR=0
	{
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin3),Bit_SET);
		GPIO_WriteBit(ModeGPIO,(uint16_t)(1 << this->ModePin4),Bit_SET);
	}

	this->OCInit.TIM_Pulse=(uint32_t)speedL-1;
	TIM_OC1Init(TIM2,&this->OCInit);

	this->OCInit.TIM_Pulse=(uint32_t)speedR-1;
	TIM_OC2Init(TIM2,&this->OCInit);
}


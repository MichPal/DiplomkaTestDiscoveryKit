/*
 * Encoder.cpp
 *
 *  Created on: 6. 5. 2017
 *      Author: Michal
 */

#include "Encoder.h"

#define PI 3.14159265359
#define IRC_RESOLUTION 1600.0

Encoder::Encoder(double _razvor, double _polomer_kolesa)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);

	InitGPIO();
	InitTimer();

	this->X = 0;
	this->Y = 0;
	this->rotation = 0;

	this->axle_track = _razvor;
	this->radius = _polomer_kolesa;

	this->speedL = 0;
	this->speedR = 0;
}

void Encoder::InitGPIO(void)
{
	GPIO_InitTypeDef gpio;
	//left Encoder
	//tim4
	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB,&gpio);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_2);
	//right Encoder
	//tim3
	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode=GPIO_Mode_AF;
	gpio.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOB,&gpio);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_2);
}

void Encoder::InitTimer(void)
{
	TIM_TimeBaseInitTypeDef timTB;
	//left Encoder
	TIM_TimeBaseStructInit(&timTB);
	timTB.TIM_CounterMode=TIM_CounterMode_Up;
	timTB.TIM_Period=0xFFFF;
	timTB.TIM_Prescaler=0;
	TIM_TimeBaseInit(TIM4,&timTB);
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);	//kvadratura
	TIM_SetCounter(TIM4,0x7FFF);	//counter do polovice
	TIM_Cmd(TIM4,ENABLE);
	//right Encoder
	TIM_TimeBaseStructInit(&timTB);
	timTB.TIM_CounterMode=TIM_CounterMode_Up;
	timTB.TIM_Period=0xFFFF;
	timTB.TIM_Prescaler=0;
	TIM_TimeBaseInit(TIM3,&timTB);
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_SetCounter(TIM3,0x7FFF);
	TIM_Cmd(TIM3,ENABLE);
}

void Encoder::Timer(void)
{
	double angleL = (double)((double)0x7FFF - (double)TIM_GetCounter(TIM4));
	double angleR = (double)((double)0x7FFF - (double)TIM_GetCounter(TIM3));

	TIM_SetCounter(TIM4,0x7FFF);
	TIM_SetCounter(TIM3,0x7FFF);

	this->speedL = angleL * 1000.0;	// angle/0.001s
	this->speedR = angleR * 1000.0;

	double distanceL = 2.0 * PI * this->radius * (double)angleL / IRC_RESOLUTION;
	double distanceR = 2.0 * PI * this->radius * (double)angleR / IRC_RESOLUTION;

	this->X = this->X + (distanceL * cos(this->rotation) / 2.0 + distanceR * cos(this->rotation) / 2.0);
	this->Y = this->Y + (distanceL * sin(this->rotation) / 2.0 + distanceR * sin(this->rotation) / 2.0);

	this->rotation = this->rotation + (distanceR / this->axle_track - distanceL / this->axle_track);

}

void Encoder::GetSpeedAndPosition(double *_speedL, double *_speedR, double *_posX, double *_posY)
{
	*_speedL = 2.0 * PI * this->radius * this->speedL / IRC_RESOLUTION;
	*_speedR = 2.0 * PI * this->radius * this->speedR / IRC_RESOLUTION;

	*_posX = this->X;
	*_posY = this->Y;
}


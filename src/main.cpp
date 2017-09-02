/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f30x.h"
#include "stm32f3_discovery.h"
			

#include "Hbridge.h"
#include "USART.h"
#include "Encoder.h"

USART *usart;
H_bridge *Motor;
Encoder *IRCSnimac;

void InitClock(void);
void InitTimer1ms(void);

int state = 1;

// tim3-encoder
// tim4-encoder
// tim16-1msTimer
// tim2-PWM



int main(void)
{
	__disable_irq();
//	DBGMCU_Config(DBGMCU_STOP,ENABLE);
//	DBGMCU_Config(DBGMCU_STANDBY,ENABLE);
//	DBGMCU_Config(DBGMCU_SLEEP,ENABLE);

	InitClock();
	InitTimer1ms();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);
//
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure USART pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 |GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
//
//	while(1)
//	{
		GPIO_WriteBit(GPIOE,GPIO_Pin_10,Bit_RESET);
		GPIO_WriteBit(GPIOE,GPIO_Pin_10,Bit_SET);
//	}

	IRCSnimac = new Encoder(2.5, 20.1);
	Motor = new H_bridge(15,14,GPIOA,4,5,6,7);

	__enable_irq();


//	usart = new USART(USART1, 115200);
//	Motor->Move(20,5);
	Motor->Move(0,0);
	while(1)
	{
		GPIO_WriteBit(GPIOE,GPIO_Pin_10,Bit_SET);
		GPIO_WriteBit(GPIOE,GPIO_Pin_10,Bit_RESET);
		//usart->SendPacket("hello",5);
	}

}

void InitClock(void)
{
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)==RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

	RCC_PLLCmd(DISABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==SET);
	//RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_12);
	RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_12);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);

	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

//	RCC_HCLKConfig(RCC_SYSCLK_Div1);	//48MHz
//	RCC_PCLK1Config(RCC_SYSCLK_Div1);	//48MHz
//	RCC_PCLK2Config(RCC_SYSCLK_Div1);	//48MHz
}

void InitTimer1ms(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);
	TIM_TimeBaseInitTypeDef timTB;
	NVIC_InitTypeDef nvic;

	//Nastavenie csovaca
	TIM_TimeBaseStructInit(&timTB);
	timTB.TIM_CounterMode=TIM_CounterMode_Up;
	timTB.TIM_Period=1;
	timTB.TIM_Prescaler=48000-1;
	TIM_TimeBaseInit(TIM16,&timTB);
	TIM_ITConfig(TIM16,TIM_IT_Update,ENABLE);


	//Nastavenie prerusenia
	nvic.NVIC_IRQChannel=TIM1_UP_TIM16_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic);
	TIM_Cmd(TIM16,ENABLE);
}
extern "C" void USART1_IRQHandler()
{
	usart->InterruptHandler();
}

extern "C" void TIM1_UP_TIM16_IRQHandler()
{
	if(TIM_GetITStatus(TIM16,TIM_IT_Update))
	{
		IRCSnimac->Timer();
		TIM_ClearITPendingBit(TIM16,TIM_IT_Update);
	}
}

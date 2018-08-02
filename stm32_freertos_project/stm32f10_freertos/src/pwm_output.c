#include "pwm_output.h"
void rcc_init(void)
{
	//RCC for TIM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//TIM1挂在APB2上
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//RCC for GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
		RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
		RCC_APB2Periph_GPIOE, ENABLE);

}

void pwm_gpio_init(void)
{
	/*
	TIM1_CH4 PA11
	TIM2_CH4 PA3
	TIM3_CH4 PB1
	TIM4_CH4 PB9
	*/
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void TIMx_PWM_Config(TIM_TypeDef* TIMx, u16 ARR, u16 CCR4_VAL) //TIM1--4
{

	/*
	=> TIM3CLK = HCLK / 2 = SystemCoreClock /2
	To get TIM3 counter clock at 21 MHz, the prescaler is computed as follows:
	Prescaler = (TIM3CLK / TIM3 counter clock) - 1
	Prescaler = ((SystemCoreClock /2) /21 MHz) - 1  ==>stm32f4
	Prescaler = ((SystemCoreClock ) /21 MHz) - 1  ==>stm32f10x

	To get TIM3 output clock at 30 KHz, the period (ARR)) is computed as follows:
	Period = (TIM3 counter clock / TIM3 output clock) - 1

	TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
	TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
	TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
	TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%

	*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = ARR;//8000000/（ARR+1）==_?_hz
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 8000000) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIMx, ENABLE);

	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//#########################################################//

	TIM_OCInitStructure.TIM_Pulse = CCR4_VAL;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC4Init(TIMx, &TIM_OCInitStructure);

	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIMx, ENABLE);
	/* TIM4 enable counter */
	TIM_Cmd(TIMx, ENABLE);

	if (TIMx == TIM1){
		TIM_CtrlPWMOutputs(TIM1, ENABLE);	//定时器1、8必须用
	}

}

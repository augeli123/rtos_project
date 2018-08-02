#ifndef PWM_OUTPUT_H_INCLUDED
#define PWM_OUTPUT_H_INCLUDED
#include "stm32f10x_conf.h"
void rcc_init(void);
void pwm_gpio_init(void);
void TIMx_PWM_Config(TIM_TypeDef* TIMx, u16 ARR, u16 CCR4_VAL);


#endif /* PWM_OUTPUT_H_INCLUDED */

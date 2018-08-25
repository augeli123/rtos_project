#ifndef MY_PWM_H
#define MY_PWM_H
#include "driver/ledc.h"
#include "driver/mcpwm.h"
#define LEDC_IO_0 (15)
#define LEDC_IO_1 (32)
#define LEDC_IO_2 (33)
#define LEDC_IO_3 (34)
#define  GPIO_PWM0A_OUT 35
#define  GPIO_PWM1A_OUT 26
//2^13=8192    %5=410  %10=819 25%=2048 50%=4096 %75=6144
#define LEDC_TEST_DUTY         (410)
#define MCPWM_TEST_DUTY        (50)
#ifdef __cplusplus
extern "C" {
#endif
void LEDC_Init();
void LEDC_Duty_Set(float pulsewidth);//只设置单个通道 LEDC_Channel_0,用于单灯
void LEDC_Duty_4_Set(float pulse_0,float pulse_1,float pulse_2,float pulse_3);//LEDC的四通道占空比设置，0.00f-1.00f
void LEDC_Freq_Set(uint32_t freq_hz);
void LEDC_Freq_4_Set(uint32_t freq_0,uint32_t freq_1,uint32_t freq_2,uint32_t freq_3);//LEDC的四通道频率设置，1000代表1000hz

void MCPWM_Init();
void MCPWM_Duty_2_Set(float pulse_0,float pulse_1);//MCPWM的两通道占空比设置，0.00f-1.00f     0.01表示占空比10%
void MCPWM_Freq_2_Set(uint32_t freq_0,uint32_t freq_1);//MCPWM的两通道频率设置
#ifdef __cplusplus
}
#endif
#endif

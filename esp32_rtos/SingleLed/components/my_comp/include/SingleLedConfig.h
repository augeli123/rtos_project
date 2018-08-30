#ifndef SINGLELEDCONFIG_H
#define SINGLELEDCONFIG_H
#include"freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#define LEDC_IO_0 (15)  //单灯对应管脚
#define TB6612_AIN1 GPIO_NUM_16        	
#define TB6612_AIN2 GPIO_NUM_4
#define TB6612_STBY GPIO_NUM_17
void TB6612_Config();
void ledc_example_timer_config();
void ledc_example_gpio_config();
//void SetSignal(const uint8_t signal[16]);
//void SetFreq(const int freqhigh,const int freqmid,const int freqlow);
void SetPwm(const int freq_hz);
void SetDuty(const float duty);
void SendPwmStream(const float time,const uint8_t signal[16]);
#endif
        

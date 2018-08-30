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
typedef struct _SingleLedInfo{
	float _time;//信号时间间隔
	int	_freq[3];//信号对应的三种频率
	uint8_t _signal[16];//信号
}SingleLedInfo;


void TB6612_Config();
void ledc_example_timer_config();
void ledc_example_gpio_config();
void SetTime(SingleLedInfo *s1,const float time);
void SetFreq(SingleLedInfo *s1,const int freqlow,const int freqmid,const int freqhigh);
void SetSignal(SingleLedInfo *s1,const uint8_t signal[16]);
void SetPwm(const int freq_hz);
void SetDuty(const float duty);
void SendPwmStream(const float time,const uint8_t signal[16]);
void SendStream(const SingleLedInfo * s1 );
#endif
        

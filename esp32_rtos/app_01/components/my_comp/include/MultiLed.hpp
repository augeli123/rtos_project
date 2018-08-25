#ifndef MULTILED_H
#define MULTILED_H
#include"freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "my_gpio.h"
#include "my_pwm.h"
class MultiLed{
private:
        int  _frequency[6];
        float  _pulsewidth[6];//最多支持6通道不同频率的PWM
public:
        //提供给用户的接口，初始化外设，设置信号
        //SingleLed(int frequency,int pulsewidth=0.1);不支持
        MultiLed(const int frequency[6],const float pulsewidth[6]);
		void Set_6_Freq(const int freq[6]);
		void Set_6_PulseWidth(const float pulsewidth[6]);
        ~MultiLed()
        {
       
        };
};
#endif


#ifndef SINGLELED_H
#define SIGLELED_H
#include"freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "my_gpio.h"
#include "my_pwm.h"
class SingleLed{
private:
	    int _freqhigh, _freqlow, _frequency;
        float  _pulsewidth;
        bool  _signal[8];
public:
        //提供给用户的接口，初始化外设，设置信号
        //SingleLed(int frequency,int pulsewidth=0.1);不支持
        SingleLed(int frequency,float pulsewidth);
        void SetSignal(const bool  signal[8]);
        void SetFreq(const int & freqhigh,const  int & freqlow);
        //底层设置
        void SetPwm(const int &freq_hz, const float &pulsewidth );
        //遍历signal，发送pwm
        void SendPwmStream();
        //打印信息
        void PwmInfo();
        void SignalInfo();
        //得到当前频率和占空比
    	int GetFreq();
    	float GetPulse();
        ~SingleLed()
		{
		//	printf("finished\n\n");
		};
};
#endif

#include<mbed.h>
//todo，添加事件中断管理函数，添加rtos

class SingleLed{
private:
	char _signal[8];
    int _freqhigh, _freqlow, _frequency;
	float  _pulsewidth;
	void _PwmStream(int freq);
	static	PwmOut LD1;
public:
	//提供给用户的接口，初始化外设，设置信号
 //   SingleLed(int frequency,int pulsewidth=0.1);不支持
	 SingleLed(int frequency,float pulsewidth);
	void SetSignal(const char signal[8]);
	void SetFreq(int freqhigh, int freqlow);
	//底层设置
	void SetPwm(int frequency, float pulsewidth );
	//遍历signal，发送pwm
	void SendPwmStream();
	//打印信息
	void PwmInfo();
	//得到当前频率和占空比
    int GetFreq();
    float GetPulse();
	~SingleLed();
};
PwmOut SingleLed::LD1(PA_11);
//依据信号流设置高低信号,接口应该尽可能的少
//提供给用户的接口，初始化外设，设置信号
SingleLed::SingleLed(int frequency, float pulsewidth )

{
	//初始化外设
    DigitalOut IO1(PB_3);
    DigitalOut IO2(PB_5);
    DigitalOut IO3(PB_10);

    IO1=1;
    IO2=1;
    IO3=0;
	_frequency = frequency;
	_freqhigh = frequency;
	_freqlow = frequency;
	_pulsewidth=pulsewidth;
    LD1.period(1.0 / frequency);
    LD1.write(pulsewidth);
}
//设置信号流
void SingleLed::SetSignal(const char signal[8])
{
	strcpy(this->_signal, signal);//strcpy_s不能用
}
//设置信号的高低频率分别代表的含义
void SingleLed::SetFreq(int freqhigh,int freqlow)
{
	this->_freqhigh = freqhigh;
	this->_freqlow  = freqlow;
}
void SingleLed::SetPwm(int frequency, float pulsewidth )
{
	//如果只想单纯的让单灯闪烁，调用此函数
	LD1.period(1.0 / frequency);
	LD1.write(pulsewidth);
}
void SingleLed::_PwmStream(int freq)
{
	//  (0.333s)
	LD1.write(0);
	wait_ms(1 / 60);
	LD1.period(1.0 / freq);
	LD1.write(this->_pulsewidth);
	wait_ms(1 / 60);
}
void SingleLed::SendPwmStream()
{
	for (int i = 0; i < 8; i++)
	{
		if (this->_signal[i] == 0)
		{
			this->_frequency = this->_freqlow;
			this->_PwmStream(this->_freqlow);
		}
		else
		{
			this->_frequency = this->_freqhigh;
			this->_PwmStream(this->_freqhigh);
		}
	}

}
//打印信息
void SingleLed::PwmInfo()
{
	printf("freqhigh:%d ,", this->_freqhigh);
	printf("freqlow:%d ", this->_freqlow);
	printf("current freq:%d ,", this->_frequency);
	printf("pulsewidth:%3f \n", this->_pulsewidth);
}
//得到当前频率和占空比
int SingleLed::GetFreq()
{
	return this->_frequency; 
}
float SingleLed::GetPulse()
{
	return this->_pulsewidth;
}

SingleLed::~SingleLed(){
    
}
/*
	1、初始化设置
	2、多任务分配
		串口中断设置任务
		pwm循环任务
		...
*/
int main()
{
	SingleLed  s1(500,0.01);
	while (1)
	{
		s1.PwmInfo();
		wait_ms(1000);
		;
	}
}

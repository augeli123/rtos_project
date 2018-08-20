#include"SingleLed.hpp"
SingleLed::SingleLed(int freq,float pulsewidth)
{
	_frequency = freq;
    _freqhigh = freq;
    _freqlow = freq;
    _pulsewidth=pulsewidth;
	LEDC_Init();
	LEDC_Freq_Set(freq);
	LEDC_Duty_Set(pulsewidth);
}
//设置信号流
void SingleLed::SetSignal(const bool  signal[8])
{   
    for(int i=0;i<8;i++)
{   
    _signal[i]=signal[i];
}
}
//设置信号的高低频率分别代表的含义
void SingleLed::SetFreq(const int &freqhigh,const int &freqlow)
{
    _freqhigh = freqhigh;
    _freqlow  = freqlow;
}
void SingleLed::SetPwm(const int &frequency,const  float & pulsewidth )
{
    //如果只想单纯的让单灯闪烁，调用此函数
    _frequency=frequency;
    _pulsewidth=pulsewidth;
    LEDC_Freq_Set(frequency);
    LEDC_Duty_Set(pulsewidth);	
}
//bool形式，可以设置两种频率
void SingleLed::SendPwmStream()
{
	while(true)
{
	for(int i=0;i<8;i++)
{
	LEDC_Duty_Set(0);
	vTaskDelay(1000/50);    
	if(_signal[i]==0){
	  SetPwm(_freqlow,_pulsewidth);
	}else{
	 SetPwm(_freqlow,_pulsewidth);
	}
	vTaskDelay(1000/50);    
}
}
}
//打印信息
void SingleLed::PwmInfo()
{
    printf("freqhigh:%d ,", this->_freqhigh);
    printf("freqlow:%d ", this->_freqlow);
    printf("current freq:%d ,", this->_frequency);
    printf("pulsewidth:%3f \n\n\n", this->_pulsewidth);
}
void SingleLed::SignalInfo()
{
    printf("signal: ");
    for(int i=0;i<8;i++)
    {
        printf("%d",this->_signal[i]);
    }
    printf("\n\n\n");
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


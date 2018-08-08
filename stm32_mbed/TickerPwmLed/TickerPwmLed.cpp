#include<mbed.h>
//todo，添加事件中断管理函数，添加rtos
PwmOut LD1(PA_11);
Thread led_thread;
DigitalOut myled(LED2); //板子上指示灯灯闪烁

class SingleLed{
private:
	//char _signal[8];//内存对齐问题？
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
	void SetPwm(const int &frequency, const float &pulsewidth );
	//遍历signal，发送pwm
	void SendPwmStream();
	//打印信息
	void PwmInfo();
	void SignalInfo();
	//得到当前频率和占空比
    int GetFreq();
    float GetPulse();
	~SingleLed();
};
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
	LD1.period(1.0 / frequency);
	LD1.write(pulsewidth);
}

void SingleLed::SendPwmStream()
{
	while(true)
{
	for (int i = 0; i < 8; i++)
	{
	LD1.write(0);
	wait_ms(1000/60);
		if (_signal[i] == 0)
		{
		SetPwm(_freqlow,_pulsewidth);
		}
		else
		{
		SetPwm(_freqhigh,_pulsewidth);
		}
		wait_ms(1000/60);
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

SingleLed::~SingleLed(){
    
}
/*
	1、初始化设置
	2、多任务分配
		串口设置任务
		pwm输出任务
		...
*/

SingleLed  s1(500,0.05);

void StartLedStream()
{	
	s1.SendPwmStream();
}
int main()
{
	bool signal[8]={1,0,1,0,1,0,1,0};
	//int freqhigh,freqlow;
	s1.SetFreq(700,500);
	s1.SetSignal(signal);
//	s1.SetPwm(700,0.5);测试正常可用
	led_thread.start(StartLedStream);
	while (true)
	{
	s1.PwmInfo();
	s1.SignalInfo();
	myled=!myled;
//	s1.SendPwmStream();直接调用，测试正常
//	s1.SetPwm(700,0.5);
   // wait_ms(1000);
//	printf("input freqhigh, freqlow\n");
//    scanf("%d ,%d",&freqhigh,&freqlow);
//    s1.SetFreq(freqhigh,freqlow);
//    s1.PwmInfo();
//	  wait_ms(2000);
		;
	}
}

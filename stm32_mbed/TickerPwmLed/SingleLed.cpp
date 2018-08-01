#include"SingleLed.h"
SingleLed::SingleLed(int frequency,int pulsewidth)
{
	//初始化外设
	DigitalOut IO1(PB_3);
	DigitalOut IO2(PB_5);
	DigitalOut IO3(PB_10);
	PwmOut LD1(PA_11);
	IO1=1;
	IO2=1;
	IO3=0;
 	LD1.period(1.0 / frequency);
	LD1.write(pulsewidth);
}

SingleLed::~SingleLed(){
	;
}

int SingleLed::GetPwm()
{
	printf("freq:%d ,", &_frequency);
	printf(" pulsewidth:%d \n", &_pulsewidth);
	return _frequency;
}
int SingleLed::GetFreq()
{
	return _frequency;
}
int SingleLed::GetPulse()
{
	return _pulsewidth;
}

void SingleLed::SetPwm(int frequency, int pulsewidth)
{
	LD1.period(1.0 / frequency);
	LD1.write(pulsewidth);
}
void SingleLed::SetPwm(int frequency)
{
	LD1.period(1.0 / frequency);
}

void SingleLed::PwmStream()
{
//	(0.333s)
	LD1.write(0);
	wait_ms(1 / 60);
	LD1.period(1.0 / this.GetFreq());
	LD1.write(this.GetPulse());
	wait_ms(1 / 60);
}
void SingleLed::SetSignal()
{
	
}
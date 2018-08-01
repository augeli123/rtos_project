#include<mbed.h>
class SingleLed{
private:
	int _frequency, _pulsewidth;
	int freq_high, freq_low;
	char signal[8];
public:
	SingleLed(int frequency,int pulsewidth);
	int GetPwm();
	int GetFreq();
	int GetPulse();
	void SetPwm(int frequency, int pulsewidth);
	void SetPwm(int frequency);
	void SetSignal();
	void PwmStream();
	~SingleLed();
};

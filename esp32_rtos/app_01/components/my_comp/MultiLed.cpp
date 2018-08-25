#include"MultiLed.hpp"
MultiLed::MultiLed(const int freq[6],const float pulsewidth[6])
{
	for(int i=0;i<6;i++)
	{
		_frequency[i]=freq[i];
		_pulsewidth[i]=pulsewidth[i];
	}	
	LEDC_Init();//LEDC包含四组pwm通道
	MCPWM_Init();//MCPWM包含2组PWM通道
	LEDC_Freq_4_Set(_frequency[0],_frequency[1],_frequency[2],_frequency[3]);
	LEDC_Duty_4_Set(_pulsewidth[0],_pulsewidth[1],_pulsewidth[2],_pulsewidth[3]);
	MCPWM_Freq_2_Set(_frequency[4],_frequency[5]);
	MCPWM_Duty_2_Set(_pulsewidth[4],_pulsewidth[5]);
}
void MultiLed::Set_6_Freq(const int Freq[6])
{
	LEDC_Freq_4_Set(_frequency[0],_frequency[1],_frequency[2],_frequency[3]);
	MCPWM_Freq_2_Set(_frequency[4],_frequency[5]);
}
void MultiLed::Set_6_PulseWidth(const float pulsewidth[6])
{
	LEDC_Duty_4_Set(_pulsewidth[0],_pulsewidth[1],_pulsewidth[2],_pulsewidth[3]);
	MCPWM_Duty_2_Set(_pulsewidth[4],_pulsewidth[5]);
}









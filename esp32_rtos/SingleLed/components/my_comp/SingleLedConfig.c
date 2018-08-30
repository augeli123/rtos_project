#include"SingleLedConfig.h"
static uint8_t _signal[16]={0};
static uint8_t _freqhigh=0 , _freqmid=0 , _freqlow=0 ,_freqcur=1000;
float _duty =0;
float _time =1; //设置信号交替的时间 
void TB6612_Config(void)
{
	 //TB6612驱动板的的配置
    gpio_set_direction(TB6612_AIN1,GPIO_MODE_OUTPUT);
    gpio_set_level(TB6612_AIN1,1);
    gpio_set_direction(TB6612_AIN2,GPIO_MODE_OUTPUT);
    gpio_set_level(TB6612_AIN2,0);
    gpio_set_direction(TB6612_STBY,GPIO_MODE_OUTPUT);
    gpio_set_level(TB6612_STBY,1);
}
void ledc_example_timer_config()
{
 ledc_timer_config_t ledc_timer_0 = {
    .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
    .freq_hz = 1000,                      // frequency of PWM signal
    .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
    .timer_num = LEDC_TIMER_0            // timer index
   };
    ledc_timer_config(&ledc_timer_0);
}
void ledc_example_gpio_config()
{
	 ledc_channel_config_t ledc_channel_0={
    .channel= LEDC_CHANNEL_0,
    .duty=82, //2^13=8192    25%=2048 50%=4096 %75=6144这里设置初始化后默认占空比为1%
    .gpio_num= LEDC_IO_0,
    .speed_mode=LEDC_HIGH_SPEED_MODE,
    .timer_sel=LEDC_TIMER_0
};
    ledc_channel_config(&ledc_channel_0);
}

//用户调用函数
/*void SetSignal(const uint8_t signal[16])
{
	 for(int i=0;i<16;i++)
	{
		_signal[i]=signal[i];
	}
}
void SetFreq(const int freqhigh,const int freqmid,const int freqlow)
{
	_freqhigh=freqhigh;
	_freqmid=freqmid;
	_freqlow=freqlow;
}*/
void SetPwm(const int freq_hz)
{	//如果需要单灯以固定频率闪烁，调用此函数
	//_freqcur=freq_hz;a
//	ledc_example_timer_config(freq_hz);
	ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, freq_hz);
/*
     One of options is to call ledc_set_freq(). There is a corresponding function ledc_get_freq() to check what frequency is currently set.
     Another option to change the frequency, and the duty resolution as well, is by calling ledc_bind_channel_timer() to bind other timer to the channel.
     Finally the channel’s timer may be changed by calling ledc_channel_config().
     这说明了在ledc_set_freq后只有再次调用ledc_channel_config()才可以改变频率，这里封装成了ledc_example_gpio_config();
*/	
	
	ledc_example_gpio_config();
//   printf("channel 0 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0));
}
void SetDuty(const float duty) //专门用于占空比设置
{
	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty*8192);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_0);
}
void SendPwmStream(const float time,const uint8_t signal[16])
{	
	const TickType_t xDelay =(1000/portTICK_PERIOD_MS)*time;//例如_time = 1/50=0.02
    for(int i=0;i<16;i++)
	{   
    	SetDuty(0);
    	vTaskDelay(xDelay);    
    	if(signal[i]==0){
      	SetPwm(500);
   		 }else if(signal[i]==1) {
     	SetPwm(650);
   		 }else{
		SetPwm(800);
			}
   		vTaskDelay(xDelay);
	}
}

#include"my_pwm.h"

void LEDC_Init()
{
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
	ledc_timer_config_t ledc_timer_0 = {
		.duty_resolution = LEDC_TIMER_13_BIT,// resolution of PWM duty
		.freq_hz =1000,                     // frequency of PWM signal
		.speed_mode = LEDC_HIGH_SPEED_MODE,  // timer mode
		.timer_num = LEDC_TIMER_0            // timer index
   };
// Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer_0);	
ledc_timer_config_t ledc_timer_1 = {
    .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
    .freq_hz = 1000,                      // frequency of PWM signal
    .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
    .timer_num = LEDC_TIMER_1       // timer index
   };
// Set configuration of timer1 for high speed channels
    ledc_timer_config(&ledc_timer_1);
ledc_timer_config_t ledc_timer_2 = {
    .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
    .freq_hz = 1000,                      // frequency of PWM signal
    .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
    .timer_num = LEDC_TIMER_2            // timer index
   };
// Set configuration of timer2 for high speed channels
    ledc_timer_config(&ledc_timer_2);
 ledc_timer_config_t ledc_timer_3 = {
    .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
    .freq_hz = 1000,                      // frequency of PWM signal
    .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
    .timer_num = LEDC_TIMER_3       // timer index
   };
// Set configuration of timer3 for high speed channels
    ledc_timer_config(&ledc_timer_3);
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
	ledc_channel_config_t ledc_channel_0={
    .channel= LEDC_CHANNEL_0,
    .duty=0,
    .gpio_num= LEDC_IO_0,
    .speed_mode=LEDC_HIGH_SPEED_MODE,
    .timer_sel=LEDC_TIMER_0
};
    ledc_channel_config(&ledc_channel_0);
    ledc_channel_config_t ledc_channel_1={
    .channel= LEDC_CHANNEL_1,
    .duty=0,
    .gpio_num= LEDC_IO_1,
    .speed_mode=LEDC_HIGH_SPEED_MODE,
    .timer_sel=LEDC_TIMER_1
};
    ledc_channel_config(&ledc_channel_1);
  ledc_channel_config_t ledc_channel_2={
    .channel= LEDC_CHANNEL_2,
    .duty=0,
    .gpio_num= LEDC_IO_2,
    .speed_mode=LEDC_HIGH_SPEED_MODE,
    .timer_sel=LEDC_TIMER_2
};
    ledc_channel_config(&ledc_channel_2);
    ledc_channel_config_t ledc_channel_3={
    .channel= LEDC_CHANNEL_3,
    .duty=0,
    .gpio_num= LEDC_IO_3,
    .speed_mode=LEDC_HIGH_SPEED_MODE,
    .timer_sel=LEDC_TIMER_3
};
    ledc_channel_config(&ledc_channel_3);
}
void LEDC_Duty_Set(float pulsewidth)
{
	uint32_t LEDC_Duty=(uint32_t)pulsewidth*8196;
	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, LEDC_Duty);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_0);

}
void LEDC_Duty_4_Set(float pulse_0,float pulse_1,float pulse_2,float pulse_3)
{	
	uint32_t LEDC_Duty_0=(uint32_t)pulse_0*8196;
	uint32_t LEDC_Duty_1=(uint32_t)pulse_1*8196;
	uint32_t LEDC_Duty_2=(uint32_t)pulse_2*8196;
	uint32_t LEDC_Duty_3=(uint32_t)pulse_3*8196;
	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, LEDC_Duty_0);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_0);
	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, LEDC_Duty_1);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_1);
	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, LEDC_Duty_2);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_2);
	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, LEDC_Duty_3);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_3);
}
void LEDC_Freq_Set(uint32_t freq_hz)
{
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, freq_hz);
}
void LEDC_Freq_4_Set(uint32_t freq_0,uint32_t freq_1,uint32_t freq_2,uint32_t freq_3)
{
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, freq_0);
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_1, freq_1);
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_2, freq_2);
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_3, freq_3);
}
//void LEDC_Duty_Get();
//void LEDC_Freq_Get();

void MCPWM_Init()
{
	printf("initializing mcpwm gpio...\n");
	mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, GPIO_PWM1A_OUT);
	//for MCPWM_UNIT_0,
	printf("Configuring Initial Parameters of MCPWM_UNIT_0......\n");
	mcpwm_config_t pwm_config_0;
    pwm_config_0.frequency = 500;    //frequency = 500Hz, 
    pwm_config_0.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config_0.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config_0.counter_mode = MCPWM_UP_COUNTER;
    pwm_config_0.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config_0);    //Configure PWM0A & PWM0B with above settings
	//for MCPWM_UNIT_1,
	printf("Configuring Initial Parameters of MCPWM_UNIT_1......\n");
	mcpwm_config_t pwm_config_1;
    pwm_config_1.frequency = 600;    //frequency = 600Hz, 
    pwm_config_1.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config_1.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config_1.counter_mode = MCPWM_UP_COUNTER;
    pwm_config_1.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1,&pwm_config_1);    //Configure PWM0A & PWM0B with above settings  
}
void MCPWM_Duty_2_Set(float pulse_0,float pulse_1)
{
    mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0, MCPWM_OPR_A, pulse_0*100);
    mcpwm_set_duty(MCPWM_UNIT_1,MCPWM_TIMER_1, MCPWM_OPR_A, pulse_1*100);
}
void MCPWM_Freq_2_Set(uint32_t freq_0,uint32_t freq_1)
{
	mcpwm_set_frequency(MCPWM_UNIT_0,  MCPWM_TIMER_0, freq_0);
    mcpwm_start(MCPWM_UNIT_0,  MCPWM_TIMER_0);
    mcpwm_set_frequency(MCPWM_UNIT_1,  MCPWM_TIMER_1, freq_1);
    mcpwm_start(MCPWM_UNIT_1,  MCPWM_TIMER_1);
}


/*
TODO: MCPWM timer0~2 和LEDC的定时器有什么关系？是可以分开吗？
*/
#include<stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "driver/ledc.h"
#define  GPIO_PWM0A_OUT 2
#define  GPIO_PWM1A_OUT 4
#define LEDC_IO_0 (5)
#define LEDC_IO_1 (18)
#define LEDC_IO_2 (19)
#define LEDC_IO_3 (21)
//2^13=8192    25%=2048 50%=4096 %75=6144
#define LEDC_TEST_DUTY         (4000)
static void ledc_example_timer_config()
{
 ledc_timer_config_t ledc_timer_0 = {
    .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
    .freq_hz = 1000,                      // frequency of PWM signal
    .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
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
}
static void ledc_example_gpio_config()
{
 
    ledc_channel_config_t ledc_channel_0={
    .channel= LEDC_CHANNEL_0,
    .duty=0,
    .gpio_num= LEDC_IO_0,
    .speed_mode=LEDC_HIGH_SPEED_MODE,
    .timer_sel=LEDC_TIMER_0
};
    ledc_channel_config(&ledc_channel_0);
//
//
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
//
//
    ledc_channel_config_t ledc_channel_3={
    .channel= LEDC_CHANNEL_3,
    .duty=0,
    .gpio_num= LEDC_IO_3,
    .speed_mode=LEDC_HIGH_SPEED_MODE,
    .timer_sel=LEDC_TIMER_3
};
    ledc_channel_config(&ledc_channel_3);
}

static void mcpwm_example_gpio_initialize()
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, GPIO_PWM1A_OUT);
}
static void ledc_example_control()
{
    ledc_example_timer_config();  
    ledc_example_gpio_config();
    printf("channel 0 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0));
    printf("channel 1 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_1));
	printf("channel 2 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_2));
    printf("channel 3 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_3));

    vTaskDelay(4000 / portTICK_PERIOD_MS);
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, 500);
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_1, 1500);
	ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_2, 2500);
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_3, 3500);

/*
     One of options is to call ledc_set_freq(). There is a corresponding function ledc_get_freq() to check what frequency is currently set.
     Another option to change the frequency, and the duty resolution as well, is by calling ledc_bind_channel_timer() to bind other timer to the channel.
     Finally the channel’s timer may be changed by calling ledc_channel_config().
*/
    ledc_example_gpio_config();
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, LEDC_TEST_DUTY);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, LEDC_TEST_DUTY);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_1);
	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, LEDC_TEST_DUTY);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_2);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, LEDC_TEST_DUTY);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_3);
    printf("channel 0 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0));
    printf("channel 1 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_1));
    printf("channel 2 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_2));
    printf("channel 3 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_3));
}
    
/**
 * @brief Configure MCPWM module
 */
static void mcpwm_example_config()
{	//for MCPWM_UNIT_0,
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
    pwm_config_1.frequency = 600;    //frequency = 1000Hz, 
    pwm_config_1.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config_1.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config_1.counter_mode = MCPWM_UP_COUNTER;
    pwm_config_1.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1,&pwm_config_1);    //Configure PWM0A & PWM0B with above settings  
}
static void mcpwm_example_control()
{
	mcpwm_example_gpio_initialize();
    mcpwm_example_config();
    printf("set duty cycle  of each operator\n");
    mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0, MCPWM_OPR_A, 50);
    mcpwm_set_duty(MCPWM_UNIT_1,MCPWM_TIMER_1, MCPWM_OPR_A, 50);
    //set duty cycle in %(i.e for 62.3% duty cycle, duty = 62.3) of each operator
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    printf("now the frequency of PWM0A is %d \n" ,mcpwm_get_frequency(MCPWM_UNIT_0,MCPWM_TIMER_0));
    printf("now the frequency of PWM1A is %d \n" ,mcpwm_get_frequency(MCPWM_UNIT_1,MCPWM_TIMER_1));
    mcpwm_set_frequency(MCPWM_UNIT_0,  MCPWM_TIMER_0, 700);
    mcpwm_start(MCPWM_UNIT_0,  MCPWM_TIMER_0);
    mcpwm_set_frequency(MCPWM_UNIT_1,  MCPWM_TIMER_1, 600);
    mcpwm_start(MCPWM_UNIT_1,  MCPWM_TIMER_1);
    vTaskDelay(4000/ portTICK_PERIOD_MS);
    printf("now the frequency of PWM0A is %d \n" ,mcpwm_get_frequency(MCPWM_UNIT_0,MCPWM_TIMER_0));
    printf("now the frequency of PWM1A is %d \n" ,mcpwm_get_frequency(MCPWM_UNIT_1,MCPWM_TIMER_1));
	printf("channel 0 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0));
    printf("channel 1 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_1));
    printf("channel 2 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_2));
    printf("channel 3 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_3));

}
static void led_pwm()
{
	ledc_example_control();
	mcpwm_example_control();
	while(1)
	{
	vTaskDelay(4000/ portTICK_PERIOD_MS);
	}
}
void app_main()
{
	printf("Testing mcpwm.......\n");
    xTaskCreate(led_pwm, "led_pwm", 4096, NULL, 5, NULL);

}

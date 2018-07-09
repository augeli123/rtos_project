/*
		TODO:1、增加读取pwm频率和占空比的函数，利用该模板，可以设置四个灯。//Setting of the duty is done by first calling dedicated function ledc_set_duty() and then calling ledc_update_duty() to make the change effective. To check the value currently set, there is a corresponding _get_ function ledc_get_duty().
			2、还没有在程序运行中设置频率//
        One of options is to call ledc_set_freq(). There is a corresponding function ledc_get_freq() to check what frequency is currently set.
        Another option to change the frequency, and the duty resolution as well, is by calling ledc_bind_channel_timer() to bind other timer to the channel.
        Finally the channel’s timer may be changed by calling ledc_channel_config().
			3、占空比还没有换算成分数//The range of the duty value entered into functions depends on selected duty_resolution and should be from 0 to (2 ** duty_resolution) - 1. 
			2^13=8192    25%=2048 50%=4096 %75=6144
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#define LEDC_IO_1 (2)
#define LEDC_IO_2 (4)
#define LEDC_TEST_DUTY         (4000)

static void ledc_example_timer_config()
{
 ledc_timer_config_t ledc_timer_0 = {
    .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
    .freq_hz = 5000,                      // frequency of PWM signal
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
// Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer_1);
}
static void ledc_example_channel_config()
{
	/*
     * Prepare individual configuration
     * for each channel of LED Controller
     * by selecting:
     * - controller's channel number
     * - output duty cycle, set initially to 0
     * - GPIO number where LED is connected to
     * - speed mode, either high or low
     * - timer servicing selected channel
     *   Note: if different channels use one timer,
     *         then frequency and bit_num of these channels
     *         will be the same
     */
    ledc_channel_config_t ledc_channel_0={
    .channel= LEDC_CHANNEL_0,
    .duty=0,
    .gpio_num= LEDC_IO_1,
    .speed_mode=LEDC_HIGH_SPEED_MODE,
    .timer_sel=LEDC_TIMER_0
};
    ledc_channel_config(&ledc_channel_0);
    ledc_channel_config_t ledc_channel_1={
    .channel= LEDC_CHANNEL_2,
    .duty=0,
    .gpio_num= LEDC_IO_2,
    .speed_mode=LEDC_HIGH_SPEED_MODE,
    .timer_sel=LEDC_TIMER_1
};
    ledc_channel_config(&ledc_channel_1);
}

static void ledc_example_control()
{	
	
	ledc_example_timer_config();	
	ledc_example_channel_config();
	printf("channel 0 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0));
	printf("channel 1 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_1));
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, 500);
	ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_1, 1500);
/*
	 One of options is to call ledc_set_freq(). There is a corresponding function ledc_get_freq() to check what frequency is currently set.
     Another option to change the frequency, and the duty resolution as well, is by calling ledc_bind_channel_timer() to bind other timer to the channel.
     Finally the channel’s timer may be changed by calling ledc_channel_config().
*/
	ledc_example_channel_config();
	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, LEDC_TEST_DUTY);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, LEDC_TEST_DUTY);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_1);


	printf("channel 0 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0));
	printf("channel 1 at %d freq\n",ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_1));
	while(1)
	{
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void app_main()
{
	
	printf("Testing ledc.......\n");
	xTaskCreate(ledc_example_control, "ledc_example_control", 4096, NULL, 5, NULL);

}

/*
void app_main()
{
    ledc_timer_config_t ledc_timer = {
    .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
    .freq_hz = 5000,                      // frequency of PWM signal
    .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
    .timer_num = LEDC_TIMER_0            // timer index
   };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);
	ledc_channel_config_t ledc_channel={
	.channel= LEDC_CHANNEL_0,
	.duty=0,
	.gpio_num= LEDC_IO_0,
	.speed_mode=LEDC_HIGH_SPEED_MODE,
	.timer_sel=LEDC_TIMER_0
};
	ledc_channel_config(&ledc_channel);
    // Initialize fade service.
    ledc_fade_func_install(0);
    while(1)
	{
	 printf("1. LEDC fade up to duty = %d\n", LEDC_TEST_DUTY);
            ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE,
                    LEDC_CHANNEL_0, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
            ledc_fade_start(LEDC_HIGH_SPEED_MODE,
                     LEDC_CHANNEL_0, LEDC_FADE_NO_WAIT);   
        vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);
        printf("2. LEDC fade down to duty = 0\n");     
            ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE,
                     LEDC_CHANNEL_0, 0, LEDC_TEST_FADE_TIME);
            ledc_fade_start(LEDC_HIGH_SPEED_MODE,
                     LEDC_CHANNEL_0, LEDC_FADE_NO_WAIT);
        vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);
        printf("3. LEDC set duty = %d without fade\n", LEDC_TEST_DUTY);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, LEDC_TEST_DUTY);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE,  LEDC_CHANNEL_0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("4. LEDC set duty = 0 without fade\n");       
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
       }
}
*/

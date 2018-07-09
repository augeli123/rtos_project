/*
MCPWM timer0~2 和LEDC的定时器有什么关系？是可以分开吗？
1.mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 18);
初始化IO管脚号、类型（set MCPWM signals, each MCPWM unit has 6 output(MCPWMXA, MCPWMXB) and 9 input(SYNC_X, FAULT_X, CAP_X) ‘X’ is timer_num(0-2) ）、UNIT_0/1
2.initial mcpwm configuration
3. 设置pwm频率和占空比
*/
#include<stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#define  GPIO_PWM0A_OUT 2
#define  GPIO_PWM1A_OUT 4
//可以定义两组MCPWM_UNIT，每一组的PWM频率相同，TODO：如果两组的定时器相同？频率会怎么样？
//answer:好像不会影响
static void mcpwm_example_gpio_initialize()
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, GPIO_PWM1A_OUT);
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
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config_0);    //Configure PWM0A & PWM0B with above settings  (TODO:怎么知道是PWM0A ？  结论：同一组共享一个频率)
	//for MCPWM_UNIT_1,
	printf("Configuring Initial Parameters of MCPWM_UNIT_1......\n");
	mcpwm_config_t pwm_config_1;
    pwm_config_1.frequency = 1000;    //frequency = 1000Hz, 
    pwm_config_1.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config_1.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config_1.counter_mode = MCPWM_UP_COUNTER;
    pwm_config_1.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config_1);    //Configure PWM0A & PWM0B with above settings  
}
static void mcpwm_example_control()
{
	mcpwm_example_gpio_initialize();
    mcpwm_example_config();
    printf("set duty cycle  of each operator\n");
    mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0, MCPWM_OPR_A, 50);
    mcpwm_set_duty(MCPWM_UNIT_1,MCPWM_TIMER_0, MCPWM_OPR_A, 50);
    //set duty cycle in %(i.e for 62.3% duty cycle, duty = 62.3) of each operator
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    printf("now the frequency of PWM0A is %d \n" ,mcpwm_get_frequency(MCPWM_UNIT_0,MCPWM_TIMER_0));
    printf("now the frequency of PWM1A is %d \n" ,mcpwm_get_frequency(MCPWM_UNIT_1,MCPWM_TIMER_0));
    mcpwm_set_frequency(MCPWM_UNIT_0,  MCPWM_TIMER_0, 2000);
    mcpwm_start(MCPWM_UNIT_0,  MCPWM_TIMER_0);
    mcpwm_set_frequency(MCPWM_UNIT_1,  MCPWM_TIMER_0, 500);
    mcpwm_start(MCPWM_UNIT_1,  MCPWM_TIMER_0);
    vTaskDelay(4000/ portTICK_PERIOD_MS);
    printf("now the frequency of PWM0A is %d \n" ,mcpwm_get_frequency(MCPWM_UNIT_0,MCPWM_TIMER_0));
    printf("now the frequency of PWM1A is %d \n" ,mcpwm_get_frequency(MCPWM_UNIT_1,MCPWM_TIMER_0));

	 while(1)
{
     vTaskDelay(1000 / portTICK_PERIOD_MS);

}

}
void app_main()
{
	printf("Testing mcpwm.......\n");
    xTaskCreate(mcpwm_example_control, "mcpwm_example_control", 4096, NULL, 5, NULL);
}

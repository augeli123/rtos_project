#include"freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include"my_gpio.h"
#include"my_pwm.h"
#include"SingleLed.hpp"
extern "C" void app_main();
SingleLed s1(1000,0.1);
bool signal[8]={1,0,1,0,1,0,1,0};
static  void task1(void * arg)
{
	LED_Blink();
}
static void task3(void *arg)
{
	while(true)
	{
	//	printf("hello world!\n");
	//由于交互式设计
		vTaskDelay(1000);
	}
}
static void task2(void *arg)
{
	s1.SendPwmStream();
}

void app_main(void)
{
    s1.SetFreq(700,500);
    s1.SetSignal(signal);

	LED_Init();
	LEDC_Init();
	printf("LED will blink\n");
	xTaskCreate(task1, "LED_Blink_Task", 1024, NULL, 10, NULL);
	xTaskCreate(task3,"hello_world_task",5000,NULL,8,NULL);
	xTaskCreate(task2, "PWM_LED_Task",10240,NULL,5,NULL);
	while(true)
	{

	vTaskDelay(10000);  

    }
}
/*TODO:
关于freertos的线程，优先级的大小，之前设置的堆栈太小，并且一些变量没有初始化，导致进程调用时失败
先是单灯的PWM波形测试，然后补充5通道的设置函数
结合联网API进行交互式设计,当然最好给出使用串口调节mbed平台的代码
*/


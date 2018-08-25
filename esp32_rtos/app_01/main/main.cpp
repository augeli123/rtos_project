#include"freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include"my_gpio.h"
#include"my_pwm.h"
#include"my_wifi.h"
#include"SingleLed.hpp"
extern "C" void app_main();
SingleLed s1(1000,0.01);
bool signal[8]={1,0,1,0,1,0,1,0};
static  void task1(void * arg)
{
	LED_Blink();
}
static void task3(void *arg)
{  
	
	while(true)
	{
	//    printf("hello world!\n");
	 printf("hello ,%s !\n",(const char *)arg);

	//由于交互式设计
		vTaskDelay(4000/portTICK_PERIOD_MS);
	}
}
static void task2(void *arg)
{
	s1.SendPwmStream();
}

void app_main(void)
{
 	//Initialize NVS
    nvs_flash_init();	
	WIFI_Init_STA();
    
	s1.SetFreq(700,500);
    s1.SetSignal(signal);
	const   char * param="logic";

	LED_Init();
	LEDC_Init();
	vTaskDelay(5000/portTICK_PERIOD_MS);
	printf("LED will blink\n");
	xTaskCreate(task1, //任务函数
				"LED_Blink_Task",//任务名称
				 2048,//任务堆栈大小
				 NULL,//传递给任务函数的参数
				 10,//任务优先级
				 NULL);//任务句柄
	xTaskCreate(task3,"hello_world_task",2048,(void *)param,8,NULL);
	xTaskCreate(task2, "PWM_LED_Task",3200,NULL,5,NULL);
}
/*TODO:
关于freertos的线程，优先级的大小，之前设置的堆栈太小，并且一些变量没有初始化，导致进程调用时失败
先是单灯的PWM波形测试，然后补充5通道的设置函数
结合联网API进行交互式设计,当然最好给出使用串口调节mbed平台的代码
*/


#include"freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "SingleLedConfig.h"
#include "esp_task_wdt.h"

static int level = 0;
static uint8_t signal[16]={1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1};
static TaskHandle_t xTask1=NULL;
static TaskHandle_t xTask2=NULL;
static void task1(void *arg)
{	

    while(1)
    {
 		SendPwmStream(0.02,signal);
    }
}
static void task2(void *arg)
{
	while(1)
	{
		level=!level;
        gpio_set_level(GPIO_NUM_2,level);
        vTaskDelay(1000/portTICK_PERIOD_MS);
	}

}

void app_main()
{
	printf("Initialize TWDT\n");
	TB6612_Config();
	ledc_example_timer_config();
	ledc_example_gpio_config();
	gpio_set_direction(GPIO_NUM_2,GPIO_MODE_OUTPUT);
	SetPwm(650);
	//设置signal发送方式
	//SetSignal(signal);
	//SetFreq(800,650,500);

	xTaskCreate(task1, //任务函数
                "SingleLed_Task",//任务名称
                 2048,//任务堆栈大小
                 NULL,//传递给任务函数的参数
                 12,//任务优先级
                 &xTask1);//任务句柄
	xTaskCreate(task2,"Blink",1024,NULL,10,&xTask2);
	/*while(1)
	{
		 vTaskDelay(1000/portTICK_PERIOD_MS);
	    // SendPwmStream(0.02,signal);
	}*/
}


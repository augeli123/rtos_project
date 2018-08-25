/*
	需要另外申请freertos的内存空间来处理中断，这一块需要看看freertos的使用
	使用GPIO的方法：先定义方向，如果是输入中断，暂时需要按部就“搬”
*/


#include"freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <stdio.h>
#define ESP_INTR_FLAG_DEFAULT 0

int level=0;
static xQueueHandle gpio_evt_queue = NULL;
extern "C" void app_main();
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}
class test{
private:
	int a,b;
public:
	test(int a,int b){
	this->a=a;
	this->b=b;	
	}
	void get()
	{
		printf("%d,%d",this->a,this->b);
	}
};
static void gpio_task_example(void* arg)
{
    gpio_num_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
          printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            level=!level;
            gpio_set_level(GPIO_NUM_2,level);
		 }
    }
}

void app_main(void)
{
	gpio_set_direction(GPIO_NUM_2,GPIO_MODE_OUTPUT);
	gpio_set_level(GPIO_NUM_2,level);
	printf("LED blink when button push\n");
    gpio_set_direction(GPIO_NUM_4,GPIO_MODE_INPUT);
	//change gpio intrrupt type for one pin
	gpio_set_intr_type(GPIO_NUM_4, GPIO_INTR_ANYEDGE);
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_NUM_4, gpio_isr_handler, (void*)GPIO_NUM_4);

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(GPIO_NUM_4);
	//hook isr handler for specific gpio pin again
	gpio_isr_handler_add(GPIO_NUM_4, gpio_isr_handler, (void*)GPIO_NUM_4);
	test t1(5,6);
	t1.get();
	while(true)
{
	
	vTaskDelay(2000/portTICK_PERIOD_MS);
}
}

#include"my_gpio.h"
static uint32_t level = 0;
void TB6612_Init(void)
{
	gpio_set_direction(TB6612_AIN1,GPIO_MODE_OUTPUT);
	gpio_set_level(TB6612_AIN1,1);
    gpio_set_direction(TB6612_AIN2,GPIO_MODE_OUTPUT);
    gpio_set_level(TB6612_AIN2,0);
    gpio_set_direction(TB6612_STBY,GPIO_MODE_OUTPUT);
    gpio_set_level(TB6612_STBY,1);
}
void LED_Init(void)
{   
    gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT);
}
void LED_Blink()
{	while(true)
	{
     level=!level;
     gpio_set_level(LED_PIN,level);
	 vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}



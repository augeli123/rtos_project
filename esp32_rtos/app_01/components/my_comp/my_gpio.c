#include"my_gpio.h"
static uint32_t level = 0;
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



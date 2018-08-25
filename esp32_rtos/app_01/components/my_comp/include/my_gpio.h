//状态指示灯
#ifndef MY_GPIO_H
#define MY_GPIO_H
#include"freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#define ESP_INTR_FLAG_DEFAULT 0
//可以修改管脚
#define LED_PIN GPIO_NUM_2
//#define KEY_PIN GPIO_NUM_4
#ifdef __cplusplus
extern "C" {
#endif
void LED_Init(void);
void LED_Blink(void);
#ifdef __cplusplus
}
#endif
#endif

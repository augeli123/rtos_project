//状态指示灯
#ifndef MY_GPIO_H
#define MY_GPIO_H
#include"freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#define ESP_INTR_FLAG_DEFAULT 0
//可以修改管脚,根据硬件
#define LED_PIN GPIO_NUM_2
#define TB6612_AIN1 GPIO_NUM_16
#define TB6612_AIN2 GPIO_NUM_4
#define TB6612_STBY GPIO_NUM_17
//#define KEY_PIN GPIO_NUM_4
#ifdef __cplusplus
extern "C" {
#endif
void TB6612_Init(void);
void LED_Init(void);
void LED_Blink(void);
#ifdef __cplusplus
}
#endif
#endif

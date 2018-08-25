#ifndef _MY_WIFI_H
#define _MY_WIFI_H
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_event_loop.h"
//#define EXAMPLE_ESP_WIFI_MODE_AP   CONFIG_ESP_WIFI_MODE_AP //TRUE:AP FALSE:STA
#define ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
//#define EXAMPLE_MAX_STA_CONN       CONFIG_MAX_STA_CONN

#ifdef __cplusplus
extern "C" {
#endif
esp_err_t event_handler(void *ctx,system_event_t *event);
//void WIFI_Init_SoftAP(void);
void WIFI_Init_STA(void); 

#ifdef __cplusplus
}
#endif
#endif



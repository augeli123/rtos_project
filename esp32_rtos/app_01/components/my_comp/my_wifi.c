#include"my_wifi.h"
static const char *TAG = "simple wifi";
esp_err_t event_handler(void *ctx,system_event_t *event){
switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
     //   xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        esp_wifi_connect();
    //    xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    default:
        break;
    }
	return ESP_OK;	
}
void WIFI_Init_STA(void)
{
	
	tcpip_adapter_init();
	esp_event_loop_init(event_handler,NULL);
	wifi_init_config_t cfg=WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);
	esp_wifi_set_storage(WIFI_STORAGE_RAM);
	esp_wifi_set_mode(WIFI_MODE_STA);
	wifi_config_t sta_config={
	.sta={
//	.ssid = ESP_WIFI_SSID,
	.ssid = "Inteagle_2.4G",
//	.password = ESP_WIFI_PASS,
	.password = "inteagle@123",
}
};
	esp_wifi_set_config(WIFI_IF_STA,&sta_config);
	esp_wifi_start();
	ESP_LOGI(TAG, "wifi_init_sta finished.");
//    ESP_LOGI(TAG, "connect to ap SSID:%s password:%s",EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
}

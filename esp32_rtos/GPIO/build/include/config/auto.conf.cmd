deps_config := \
	/home/logic/esp_ws/esp-idf/components/app_trace/Kconfig \
	/home/logic/esp_ws/esp-idf/components/aws_iot/Kconfig \
	/home/logic/esp_ws/esp-idf/components/bt/Kconfig \
	/home/logic/esp_ws/esp-idf/components/driver/Kconfig \
	/home/logic/esp_ws/esp-idf/components/esp32/Kconfig \
	/home/logic/esp_ws/esp-idf/components/esp_adc_cal/Kconfig \
	/home/logic/esp_ws/esp-idf/components/esp_http_client/Kconfig \
	/home/logic/esp_ws/esp-idf/components/ethernet/Kconfig \
	/home/logic/esp_ws/esp-idf/components/fatfs/Kconfig \
	/home/logic/esp_ws/esp-idf/components/freertos/Kconfig \
	/home/logic/esp_ws/esp-idf/components/heap/Kconfig \
	/home/logic/esp_ws/esp-idf/components/http_server/Kconfig \
	/home/logic/esp_ws/esp-idf/components/libsodium/Kconfig \
	/home/logic/esp_ws/esp-idf/components/log/Kconfig \
	/home/logic/esp_ws/esp-idf/components/lwip/Kconfig \
	/home/logic/esp_ws/esp-idf/components/mbedtls/Kconfig \
	/home/logic/esp_ws/esp-idf/components/mdns/Kconfig \
	/home/logic/esp_ws/esp-idf/components/openssl/Kconfig \
	/home/logic/esp_ws/esp-idf/components/pthread/Kconfig \
	/home/logic/esp_ws/esp-idf/components/spi_flash/Kconfig \
	/home/logic/esp_ws/esp-idf/components/spiffs/Kconfig \
	/home/logic/esp_ws/esp-idf/components/tcpip_adapter/Kconfig \
	/home/logic/esp_ws/esp-idf/components/vfs/Kconfig \
	/home/logic/esp_ws/esp-idf/components/wear_levelling/Kconfig \
	/home/logic/esp_ws/esp-idf/Kconfig.compiler \
	/home/logic/esp_ws/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/logic/esp_ws/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/logic/esp_ws/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/logic/esp_ws/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;

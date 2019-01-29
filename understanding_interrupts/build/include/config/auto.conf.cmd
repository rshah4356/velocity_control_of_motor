deps_config := \
	/home/atharv/esp/esp-idf/components/app_trace/Kconfig \
	/home/atharv/esp/esp-idf/components/aws_iot/Kconfig \
	/home/atharv/esp/esp-idf/components/bt/Kconfig \
	/home/atharv/esp/esp-idf/components/driver/Kconfig \
	/home/atharv/esp/esp-idf/components/esp32/Kconfig \
	/home/atharv/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/atharv/esp/esp-idf/components/esp_event/Kconfig \
	/home/atharv/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/atharv/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/atharv/esp/esp-idf/components/ethernet/Kconfig \
	/home/atharv/esp/esp-idf/components/fatfs/Kconfig \
	/home/atharv/esp/esp-idf/components/freemodbus/Kconfig \
	/home/atharv/esp/esp-idf/components/freertos/Kconfig \
	/home/atharv/esp/esp-idf/components/heap/Kconfig \
	/home/atharv/esp/esp-idf/components/libsodium/Kconfig \
	/home/atharv/esp/esp-idf/components/log/Kconfig \
	/home/atharv/esp/esp-idf/components/lwip/Kconfig \
	/home/atharv/esp/esp-idf/components/mbedtls/Kconfig \
	/home/atharv/esp/esp-idf/components/mdns/Kconfig \
	/home/atharv/esp/esp-idf/components/mqtt/Kconfig \
	/home/atharv/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/atharv/esp/esp-idf/components/openssl/Kconfig \
	/home/atharv/esp/esp-idf/components/pthread/Kconfig \
	/home/atharv/esp/esp-idf/components/spi_flash/Kconfig \
	/home/atharv/esp/esp-idf/components/spiffs/Kconfig \
	/home/atharv/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/atharv/esp/esp-idf/components/unity/Kconfig \
	/home/atharv/esp/esp-idf/components/vfs/Kconfig \
	/home/atharv/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/atharv/esp/esp-idf/components/app_update/Kconfig.projbuild \
	/home/atharv/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/atharv/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/atharv/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/atharv/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_TARGET)" "esp32"
include/config/auto.conf: FORCE
endif
ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;

deps_config := \
	/home/rshah/esp/esp-idf/components/app_trace/Kconfig \
	/home/rshah/esp/esp-idf/components/aws_iot/Kconfig \
	/home/rshah/esp/esp-idf/components/bt/Kconfig \
	/home/rshah/esp/esp-idf/components/driver/Kconfig \
	/home/rshah/esp/esp-idf/components/esp32/Kconfig \
	/home/rshah/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/rshah/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/rshah/esp/esp-idf/components/ethernet/Kconfig \
	/home/rshah/esp/esp-idf/components/fatfs/Kconfig \
	/home/rshah/esp/esp-idf/components/freertos/Kconfig \
	/home/rshah/esp/esp-idf/components/heap/Kconfig \
	/home/rshah/esp/esp-idf/components/libsodium/Kconfig \
	/home/rshah/esp/esp-idf/components/log/Kconfig \
	/home/rshah/esp/esp-idf/components/lwip/Kconfig \
	/home/rshah/esp/esp-idf/components/mbedtls/Kconfig \
	/home/rshah/esp/esp-idf/components/mdns/Kconfig \
	/home/rshah/esp/esp-idf/components/openssl/Kconfig \
	/home/rshah/esp/esp-idf/components/pthread/Kconfig \
	/home/rshah/esp/esp-idf/components/spi_flash/Kconfig \
	/home/rshah/esp/esp-idf/components/spiffs/Kconfig \
	/home/rshah/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/rshah/esp/esp-idf/components/vfs/Kconfig \
	/home/rshah/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/rshah/esp/esp-idf/Kconfig.compiler \
	/home/rshah/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/rshah/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/rshah/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/rshah/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;

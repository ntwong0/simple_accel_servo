deps_config := \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/app_trace/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/aws_iot/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/bt/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/driver/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/esp32/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/esp_adc_cal/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/esp_http_client/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/ethernet/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/fatfs/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/freertos/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/heap/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/http_server/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/libsodium/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/log/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/lwip/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/mbedtls/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/mdns/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/openssl/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/pthread/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/spi_flash/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/spiffs/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/tcpip_adapter/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/vfs/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/wear_levelling/Kconfig \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/Kconfig.compiler \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/Projects/XHR_test/components/arduino/Kconfig.projbuild \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/bootloader/Kconfig.projbuild \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/esptool_py/Kconfig.projbuild \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/components/partition_table/Kconfig.projbuild \
	/media/fishie2610/data/Users/fishie2610/Workspace/SJRobotics/controlsystems2019/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;

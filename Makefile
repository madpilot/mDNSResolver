# "/home/myles/Arduino/hardware/esp8266com/esp8266/tools/xtensa-lx106-elf/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-I/home/myles/Arduino/hardware/esp8266com/esp8266/tools/sdk/include" "-I/home/myles/Arduino/hardware/esp8266com/esp8266/tools/sdk/lwip/include" "-I/home/myles/Arduino/hardware/esp8266com/esp8266/tools/sdk/libc/xtensa-lx106-elf/include" "-I/tmp/arduino_build_119366/core" -c -w -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11  -ffunction-sections -fdata-sections -w -x c++ -E -CC -DF_CPU=160000000L -DLWIP_OPEN_SRC   -DARDUINO=10612 -DARDUINO_ESP8266_ESP01 -DARDUINO_ARCH_ESP8266 -DARDUINO_BOARD="ESP8266_ESP01"  -DESP8266 "-I/home/myles/Arduino/hardware/esp8266com/esp8266/cores/esp8266" "-I/home/myles/Arduino/hardware/esp8266com/esp8266/variants/generic" "/tmp/arduino_build_119366/sketch/mDNSTest.ino.cpp" -o "/dev/null"

build_tests:
	g++ -g -std=c++11 \
		test/Runner.cpp \
		test/QueryTest.cpp        src/Query.cpp \
		test/CacheTest.cpp        src/Cache.cpp \
		test/ResponseTest.cpp     src/Response.cpp \
		test/AnswerTest.cpp       src/Answer.cpp \
		test/mDNSResolverTest.cpp src/mDNSResolver.cpp \
		mocks/WiFiUdp.cpp mocks/IPAddress.cpp mocks/Arduino.cpp \
		-I src \
		-I mocks \
		-D TEST \
		-o test/runner

clean_tests:
	rm test/runner

debug_tests:
	make build_tests
	gdb test/runner

tests:
	make build_tests
	test/runner
	make clean_tests

ci:
	make build_tests
	test/runner

IDIR = ./
CC=g++
CFLAGS=-I$(IDIR) -I../inc/mavlink/common
LIBS=-lpthread -lz -lssl -lcrypto
COMPILE.cc = $(CC) -c -g $(CFLAGS)
output = -c -o $@ $<


UTILS_OBJS = utils/filter.o utils/fol_filter.o utils/util.o utils/mavlink_wtf.o utils/i_interval_thread.o utils/pid.o utils/avg_filter.o utils/sa_modem.o
LIBCONFIG_OBJS = ./utils/libconfig/grammar.o ./utils/libconfig/libconfig.o ./utils/libconfig/libconfigcpp.o ./utils/libconfig/scanctx.o ./utils/libconfig/scanner.o ./utils/libconfig/strbuf.o
LWS_OBJ=./utils/libwebsockets/alloc.o ./utils/libwebsockets/base64-decode.o ./utils/libwebsockets/handshake.o ./utils/libwebsockets/output.o ./utils/libwebsockets/client.o ./utils/libwebsockets/header.o ./utils/libwebsockets/parsers.o ./utils/libwebsockets/client-handshake.o ./utils/libwebsockets/pollfd.o ./utils/libwebsockets/client-parser.o ./utils/libwebsockets/server.o ./utils/libwebsockets/context.o ./utils/libwebsockets/server-handshake.o ./utils/libwebsockets/libwebsockets.o ./utils/libwebsockets/service.o ./utils/libwebsockets/extension.o ./utils/libwebsockets/lws-plat-unix.o ./utils/libwebsockets/sha-1.o ./utils/libwebsockets/extension-deflate-frame.o ./utils/libwebsockets/ssl.o ./utils/libwebsockets/extension-deflate-stream.o
IO_OBJS = device/driver/io/io.o device/driver/io/serial_handler.o device/driver/io/iic_handler.o device/driver/io/gpio_const.o device/driver/io/gpio_manager.o device/driver/io/gpio_handler.o device/driver/io/ws_handler.o
DRIVER_OBJS = device/driver/serial_radio_driver.o device/driver/bmx055_driver.o device/driver/serial_servo_driver.o device/driver/ads7828_driver.o device/driver/beeper_driver.o device/driver/hr_driver.o device/driver/websocket_driver.o
DEVICE_OBJS = device/radio.o device/i_device.o device/imu.o device/servo_controller.o device/aoa.o device/beeper.o device/agl.o device/virtual_imu.o device/hr.o
INSTRU_OBJS = instruments/adi.o
NAVIGATOR_OBJS = navigator/i_navigator.o navigator/attitude_navigator.o navigator/manully_navigator.o
OBJS = chicken.o device_manager.o status_controller.o attitude_controller.o $(UTILS_OBJS) $(LIBCONFIG_OBJS) $(LWS_OBJ) $(DRIVER_OBJS) $(DEVICE_OBJS) $(IO_OBJS) $(INSTRU_OBJS) $(NAVIGATOR_OBJS)


TARGET = chicken

SUB_DIR = ./utils ./utils/libconfig ./utils/libconfig ./device ./device/driver ./device/driver/io ./instruments ./navigator

all:	RETRIVE_SUB_DIR $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) 

RETRIVE_SUB_DIR:
	for dir in $(SUB_DIR); do\
		make -C $$dir;\
	done

chicken.o:	chicken.cpp
device_manager.o:	device_manager.cpp device_manager.h
status_controller.o:	status_controller.cpp status_controller.h



.PHONY: all clean RETRIVE_SUB_DIR
clean:
	rm -f *.cpp~
	rm -f *.h~
	rm -f *.sh~
	rm -f Makefile~
	rm -f *.o
	rm -f $(TARGET)

	for dir in $(SUB_DIR); do\
		make -C $$dir clean;\
	done

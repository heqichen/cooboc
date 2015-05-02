IDIR = ./
CC=g++
CFLAGS=-I$(IDIR) -I../inc/mavlink/common
LIBS=-lpthread
COMPILE.cc = $(CC) -c -g $(CFLAGS)
output = -c -o $@ $<


UTILS_OBJS = utils/filter.o utils/fol_filter.o utils/util.o utils/mavlink_wtf.o utils/i_interval_thread.o utils/pid.o utils/avg_filter.o
LIBCONFIG_OBJS = ./utils/libconfig/grammar.o ./utils/libconfig/libconfig.o ./utils/libconfig/libconfigcpp.o ./utils/libconfig/scanctx.o ./utils/libconfig/scanner.o ./utils/libconfig/strbuf.o
IO_OBJS = device/driver/io/io.o device/driver/io/serial_handler.o device/driver/io/iic_handler.o device/driver/io/gpio_const.o device/driver/io/gpio_manager.o device/driver/io/gpio_handler.o
DRIVER_OBJS = device/driver/serial_radio_driver.o device/driver/bmx055_driver.o device/driver/serial_servo_driver.o device/driver/ads7828_driver.o device/driver/beeper_driver.o device/driver/hr_driver.o
DEVICE_OBJS = device/radio.o device/i_device.o device/imu.o device/servo_controller.o device/aoa.o device/beeper.o device/agl.o device/virtual_imu.o device/hr.o
INSTRU_OBJS = instruments/adi.o
NAVIGATOR_OBJS = navigator/i_navigator.o navigator/attitude_navigator.o navigator/manully_navigator.o
OBJS = chicken.o device_manager.o status_controller.o attitude_controller.o $(UTILS_OBJS) $(LIBCONFIG_OBJS) $(DRIVER_OBJS) $(DEVICE_OBJS) $(IO_OBJS) $(INSTRU_OBJS) $(NAVIGATOR_OBJS)


TARGET = chicken

SUB_DIR = ./utils ./utils/libconfig ./device ./device/driver ./device/driver/io ./instruments ./navigator

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

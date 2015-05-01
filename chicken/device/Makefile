IDIR = ../
CC=g++
CFLAGS=-I$(IDIR) -I../inc/mavlink/common
COMPILE.cc = $(CC) -c -g $(CFLAGS)

OBJS = radio.o i_device.o imu.o servo_controller.o aoa.o beeper.o agl.o virtual_imu.o

LIBS=

all:	$(OBJS)

radio.o:	radio.cpp radio.h i_device.h
i_device.o:	i_device.cpp i_device.h i_device.h
imu.o:	imu.cpp imu.h
servo_controller.o:	servo_controller.cpp servo_controller.h
aoa.o:	aoa.cpp aoa.h
beeper.o:	beeper.cpp beeper.h
agl.o:	agl.cpp	agl.h
virtual_imu.o:	virtual_imu.cpp virtual_imu.h


.PHONY: clean
clean:
	rm -f *.cpp~
	rm -f *.h~
	rm -f *.sh~
	rm -f Makefile~
	rm -f *.o
	rm -f $(TARGET)

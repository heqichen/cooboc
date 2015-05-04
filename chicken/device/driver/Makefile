IDIR = ../../
CC=g++
CFLAGS=-I$(IDIR) -I../../../inc/mavlink/common/
COMPILE.cc = $(CC) -c -g $(CFLAGS)

OBJS = serial_radio_driver.o bmx055_driver.o serial_servo_driver.o ads7828_driver.o beeper_driver.o hr_driver.o websocket_driver.o

LIBS=

all:	$(OBJS)

serial_radio_driver.o:	serial_radio_driver.cpp serial_radio_driver.h
bmx055_driver.o:	bmx055_driver.cpp bmx055_driver.h
serial_servo_driver.o:	serial_servo_driver.cpp serial_servo_driver.h
ads7828_driver.o:	ads7828_driver.cpp ads7828_driver.h
beeper_driver.o:	beeper_driver.cpp beeper_driver.h
hr_driver.o:	hr_driver.cpp hr_driver.h
websocket_driver.o:	websocket_driver.cpp websocket_driver.h

.PHONY: clean
clean:
	rm -f *.cpp~
	rm -f *.h~
	rm -f *.sh~
	rm -f Makefile~
	rm -f *.o
	rm -f $(TARGET)

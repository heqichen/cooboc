IDIR = ../../
CC=g++
CFLAGS=-I$(IDIR)
COMPILE.cc = $(CC) -c -g $(CFLAGS)

OBJS = io.o serial_handler.o iic_handler.o gpio_const.o gpio_manager.o gpio_handler.o

LIBS=

all:	$(OBJS)

io.o:		io.cpp io.h
serial_handler.o:	serial_handler.cpp serial_handler.h
iic_handler.o:	iic_handler.cpp iic_handler.h
gpio_const.o:	gpio_const.cpp gpio_const.h
gpio_manager.o:	gpio_manager.cpp gpio_manager.h
gpio_handler.o:	gpio_handler.cpp gpio_handler.h

.PHONY: clean
clean:
	rm -f *.cpp~
	rm -f *.h~
	rm -f *.sh~
	rm -f Makefile~
	rm -f *.o
	rm -f $(TARGET)

IDIR = ../
CC=g++
CFLAGS=-I$(IDIR) -I../../mavlink/common
COMPILE.cc = $(CC) -c -g $(CFLAGS)

OBJS = i_navigator.o attitude_navigator.o manully_navigator.o

LIBS=

all:	$(OBJS)

i_navigator.o:	i_navigator.cpp i_navigator.h
attitude_navigator.o:	attitude_navigator.cpp attitude_navigator.h
manully_navigator.o:	manully_navigator.cpp manully_navigator.h


.PHONY: clean
clean:
	rm -f *.cpp~
	rm -f *.h~
	rm -f *.sh~
	rm -f Makefile~
	rm -f *.o
	rm -f $(TARGET)

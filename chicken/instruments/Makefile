IDIR = ../
CC=g++
CFLAGS=-I$(IDIR) -I../../mavlink/common
COMPILE.cc = $(CC) -c -g $(CFLAGS)

OBJS = adi.o

LIBS=

all:	$(OBJS)

adi.o:	adi.cpp adi.h


.PHONY: clean
clean:
	rm -f *.cpp~
	rm -f *.h~
	rm -f *.sh~
	rm -f Makefile~
	rm -f *.o
	rm -f $(TARGET)

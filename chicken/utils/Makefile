IDIR = ../
CC=g++
CFLAGS=-I$(IDIR) -I../../mavlink/common
COMPILE.cc = $(CC) -c -g $(CFLAGS)

OBJS = filter.o fol_filter.o util.o mavlink_wtf.o i_interval_thread.o pid.o avg_filter.o

LIBS=

all:	$(OBJS)

filter.o:	filter.cpp filter.h
fol_filter.o:	fol_filter.cpp fol_filter.h
util.o:	util.cpp util.h
mavlink_wtf.o:	mavlink_wtf.cpp mavlink_wtf.h
i_interval_thread.o:	i_interval_thread.cpp i_interval_thread.h
pid.o:	pid.cpp pid.h
avg_filter.o:	avg_filter.cpp avg_filter.h

.PHONY: clean
clean:
	rm -f *.cpp~
	rm -f *.h~
	rm -f *.sh~
	rm -f Makefile~
	rm -f *.o
	rm -f $(TARGET)

PWD=$(shell pwd)
CONFIG=$(PWD)/../mk.config
include $(CONFIG)

LIBS =  -lutilities -lcontrol-device


ex1: ex1.o
	$(CC) -o $(<:%.o=%.exe) $< $(LDFLAGS) $(LIBS)

all: ex1

new: clean all

clean: stdclean
	rm -f *.o *.exe
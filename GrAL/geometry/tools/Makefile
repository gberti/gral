PWD=$(shell pwd)
CONFIG=$(PWD)/../mk.config
include $(CONFIG)

LIBS =  -lexternal-control -lgeometry -lutilities 

EXTRA_DEPMODULES = sequence external-control

transform: transform.o
	$(CC) -o $(<:%.o=%.exe) $< $(LDFLAGS) $(LIBS)

all: rot

new: clean all

clean: stdclean
	rm -f *.o *.exe

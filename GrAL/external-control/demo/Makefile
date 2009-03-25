PWD=$(shell pwd)
CONFIG=$(PWD)/../local.config
include $(CONFIG)

LIBS =  -lexternal-control -lutilities


ex1: ex1.o
	$(CC) -o $(<:%.o=%.exe) $< $(LDFLAGS) $(LIBS)

ex2: ex2.o
	$(CC) -o $(<:%.o=%.exe) $< $(LDFLAGS) $(LIBS)

all: ex1 ex2

new: clean all

clean: stdclean
	rm -f *.o *.exe

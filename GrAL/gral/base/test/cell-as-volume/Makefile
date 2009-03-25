# gral/base/test/cell-as-volume

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC =  $(SOURCES:%.C=%.exe)

EXTRA_DEPMODULES += geometry gral/cartesiannd 

include $(DEPS)



LIBS = -lgeometry -lgral-base 


new-check-clean: new check clean

new : clean all


all : $(EXEC)


$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC)  $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	rm -f oct.complex2d


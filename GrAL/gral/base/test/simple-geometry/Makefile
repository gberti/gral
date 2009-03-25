# gral/base/test/simple-geometry

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC =  $(SOURCES:%.C=%.exe)

EXTRA_DEPMODULES += geometry gral/complex2d gral/complex3d gral/cartesiannd

include $(DEPS)


LIBS = -lgral-complex3d -lgral-complex2d -lgral-base  -lgeometry -lsequence  -lutilities


new-check-clean: new check clean

new : clean all


all : $(EXEC)


$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC)  $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: 
	$(MAKE) run-check FILTER=$(CONFIGURATION)/scripts/floating-point-filter.pl

clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



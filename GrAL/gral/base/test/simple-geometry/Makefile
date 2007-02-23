# gral/base/test/simple-geometry

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC =  $(SOURCES:%.C=%.exe)

EXTRA_DEPMODULES += geometry gral/complex2d gral/complex3d gral/cartesiannd

include $(DEPS)


LIBS = -lgral-complex3d -lgral-complex2d -lgral-base  -lgeometry  -lutilities


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



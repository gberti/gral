# gral/triang3d/test/geometry

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


EXTRA_DEPMODULES +=  external-control utilities 

SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS    =  $(SOURCES:%.C=%.d)
TARGETS =  $(SOURCES:%.C=%.exe)
EXEC =  $(TARGETS)

include $(DEPS)



LIBS = -lgral-triang3d -lgral-complex2d -lgral-base -lgeometry -lexternal-control -lutilities



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



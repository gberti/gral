# gral/base/test/restricted-grid-view

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC =  $(SOURCES:%.C=%.exe)

EXTRA_DEPMODULES += geometry gral/cartesian2d gral/testing

include $(DEPS)



LIBS = -lgral-cartesian2d -lgral-base  -lgeometry 


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



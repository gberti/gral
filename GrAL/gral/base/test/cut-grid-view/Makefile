# gral/base/test/cut-grid-view

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

SOURCES =  test-cut-grid-view.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC = $(SOURCES:%.C=%.exe)
TARGETS = $(EXEC)
EXTRA_DEPMODULES = geometry gral/cartesian2d gral/gmv-io gral/testing

include $(DEPS)

LIBS +=  -lgral-cartesian2d -lgral-gmv-io -lgral-base -lgeometry -lutilities


new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(TARGETS): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



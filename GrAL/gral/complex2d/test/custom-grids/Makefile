# gral/complex2d/test/custom-grids

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

SOURCES =  test-custom-grids.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC = $(SOURCES:%.C=%.exe)
TARGETS = $(EXEC)
EXTRA_DEPMODULES = geometry external-control gral/gmv-io

include $(DEPS)

LIBS += -lgral-complex2d  -lgral-gmv-io -lgral-base -lgeometry -lexternal-control


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



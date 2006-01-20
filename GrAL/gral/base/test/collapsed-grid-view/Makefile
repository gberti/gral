# gral/base/test/collapsed-grid-view

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

SOURCES =  test-collapsed-grid-view.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC = $(SOURCES:%.C=%.exe)
TARGETS = $(EXEC)
EXTRA_DEPMODULES = geometry external-control gral/triang2d gral/gmv-io gral/testing

include $(DEPS)

LIBS +=  -lgral-triang2d -lgral-gmv-io -lgral-base -lgeometry -lexternal-control


new-check-clean: new check clean

new : clean all


all : $(TARGETS)

simplify-mesh.exe: simplify-mesh.o
	$(CC) -o $@ $< $(LIBS) $(LDFLAGS)

$(TARGETS): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



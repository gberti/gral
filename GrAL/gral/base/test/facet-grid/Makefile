# gral/base/test/facet-grid

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

SOURCES =  test-facet-grid.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC = $(SOURCES:%.C=%.exe)
TARGETS = $(EXEC)
EXTRA_DEPMODULES =  gral/cartesian3d 

include $(DEPS)

LIBS +=  -lgral-cartesian3d  -lgral-base -lsequence -lutilities


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



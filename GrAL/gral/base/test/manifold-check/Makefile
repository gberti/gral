# gral/base/test/orientation

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

SOURCES =  test-manifold-check.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC = $(SOURCES:%.C=%.exe)
TARGETS = $(EXEC)
EXTRA_DEPMODULES = gral/triang2d gral/complex2d 

include $(DEPS)

LIBS +=  -lgral-complex2d -lgral-triang2d -lgral-base -lgeometry -lsequence -lutilities


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



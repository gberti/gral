# gral/base/test/disjoint-union

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-disjoint-union
EXEC =  $(TARGETS)

include $(DEPS)

EXTRA_DEPMODULES+= gral/cartesian2d gral/complex2d gral/testing geometry

LIBS = -lgral-cartesian2d -lgral-complex2d  -lgral-base -lgeometry



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-disjoint-union: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



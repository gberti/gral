# gral/base/test/disjoint-union

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-shrunk-grid
EXEC =  $(TARGETS)

include $(DEPS)

DEPMODULES+= gral/cartesian3d gral/complex2d gral/complex3d \
             gral/testing gral/gmv-io geometry

LIBS = -lgral-cartesian3d -lgral-complex3d -lgral-complex2d \
       -lgral-gmv-io -lgral-base -lgeometry



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-shrunk-grid: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



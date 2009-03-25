# gral/hierarchical/test/non-conforming-leaf-grid

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

EXTRA_DEPMODULES += gral/complex2d  gral/cartesian3d gral/cartesian2d gral/cartesiannd gral/geomview-io gral/testing gral/gmv-io
LOCALSOURCES =  test-non-conforming-leaf-grid.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC = $(LOCALSOURCES:%.C=%.exe)

TARGETS =$(EXEC)


include $(DEPS)



LIBS = -lgral-geomview-io -lgral-cartesian3d -lgral-cartesian2d -lgral-complex2d -lgral-base -lgral-gmv-io -lutilities





new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(TARGETS): $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



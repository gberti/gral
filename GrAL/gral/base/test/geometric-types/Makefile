# gral/base/test/geometric-types

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

LOCALSOURCES =  test-geometric-types.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC =  $(LOCALSOURCES:%.C=%.exe)

TARGETS = $(EXEC)
EXTRA_DEPMODULES += geometry gral/cartesian2d gral/cartesian3d gral/complex2d

include $(DEPS)



LIBS = -lgral-cartesian2d -lgral-cartesian3d -lgral-complex2d -lgral-base


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
	rm -f oct.complex2d


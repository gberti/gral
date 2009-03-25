# gral/cartesiannd/test/construct

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

EXTRA_DEPMODULES += geometry gral/testing gral/complexnd gral/complex2d gral/complex3d gral/gmv-io
LOCALSOURCES =  test-cartesiannd-copy.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC = $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)

LIBS += -lgral-complex3d -lgral-complex2d  -lgral-gmv-io
include $(DEPS)

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



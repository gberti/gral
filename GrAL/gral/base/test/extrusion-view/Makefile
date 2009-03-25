# gral/base/test/extrusion-view

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

LOCALSOURCES =  test-extrusion-view.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC =  $(LOCALSOURCES:%.C=%.exe)

TARGETS = $(EXEC)
EXTRA_DEPMODULES += geometry gral/complex2d  gral/testing gral/gmv-io 

include $(DEPS)



LIBS = -lgral-gmv-io -lgral-complex2d -lgral-base -lgeometry -lsequence -lutilities


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


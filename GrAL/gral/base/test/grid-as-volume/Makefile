# gral/base/test/grid-as-volume

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

LOCALSOURCES =  test-grid-as-volume.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC =  $(LOCALSOURCES:%.C=%.exe)

TARGETS = $(EXEC)
EXTRA_DEPMODULES += geometry gral/cartesian2d gral/complex2d

include $(DEPS)



LIBS = -lgral-base -lgral-cartesian2d -lgral-complex2d -lgeometry


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


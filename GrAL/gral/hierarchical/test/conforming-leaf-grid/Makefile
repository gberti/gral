# gral/hierarchical/test/conforming-leaf-grid

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

EXTRA_DEPMODULES += gral/complex2d  gral/cartesiannd
LOCALSOURCES =  test-conforming-leaf-grid.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC = $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)

include $(DEPS)



LIBS =  -lgral-complex2d -lgral-base -lutilities





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


# gral/base/test/flag

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-flag
EXEC =  $(TARGETS)

include $(DEPS)

EXTRA_DEPMODULES+= gral/cartesian2d geometry

LIBS = -lgral-cartesian2d -lgral-base



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-flag: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



# gral/base/test/gmv-output

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-gmv-output
EXEC =  $(TARGETS)

include $(DEPS)

DEPMODULES+= gral/cartesian2d gral/complex2d gral/cartesian3d geometry

LIBS = -lgral-gmv-io -lgral-cartesian3d  -lgral-cartesian2d -lgral-base 



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-gmv-output: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



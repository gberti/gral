# gral/base/test/gmv-output


PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
TARGETS = test-geomview-output
EXEC =  $(TARGETS)

include $(DEPS)

EXTRA_DEPMODULES+= gral/cartesian2d 

LIBS = -lgral-geomview-io -lgral-cartesian2d -lgral-base 



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



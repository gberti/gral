# gral/cartesian2d/test/iterators

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  test-cartesian-iterators.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-cartesian-iterators
EXEC =  $(TARGETS)

include $(DEPS)



LIBS = -lgral-cartesian2d -lgral-base -lutilities

# -lexternal-control -lutilities 



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-cartesian-iterators: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



# gral/distributed/test/composite

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  test-composite.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-composite
EXEC =  $(TARGETS)

include $(DEPS)



LIBS += -lgral-distributed -lgral-complex2d -lgral-base  -lexternal-control -lutilities 



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-composite: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check:
	./runtest

clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	rm -f out?x?p?


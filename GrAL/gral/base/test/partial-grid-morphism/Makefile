# gral/base/test/partial-grid-morphism

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-partial-grid-morphisms
EXEC =  $(TARGETS)

include $(DEPS)

DEPMODULES+= gral/cartesian2d geometry

LIBS = -lgral-cartesian2d -lgral-base



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-partial-grid-morphisms: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check:
	./runtest

clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



# gral/base/test/construct-isomorphism

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-construct-isomorphism
EXEC =  $(TARGETS)

include $(DEPS)

EXTRA_DEPMODULES+= gral/cartesian2d gral/cartesian3d gral/complex2d geometry

LIBS = -lgral-cartesian3d -lgral-cartesian2d -lgral-base -lgral-complex2d 



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-construct-isomorphism: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



# gral/complex3d/test/iterators

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)


LOCALSOURCES =  test-complex3d-iterators.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-complex3d-iterators
EXEC =  $(TARGETS)

EXTRA_DEPMODULES += gral/cartesian3d
include $(DEPS)



LIBS = -lgral-complex3d -lgral-complex2d -lgral-cartesian3d -lgral-base\
       -lexternal-control -lutilities 



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-complex3d-iterators: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



# gral/complex2d/test/construct

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

EXTRA_DEPMODULES += gral/testing
LOCALSOURCES =  test-construct-main.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-construct
EXEC =  $(TARGETS)

include $(DEPS)



LIBS = -lgral-complex2d -lgral-base -lgeometry -lexternal-control -lutilities 



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-construct: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



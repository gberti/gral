# gral/triang2d/test/construct

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


DEPMODULES +=  external-control utilities

SOURCES =  test-triang2d-construct.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS    =  $(SOURCES:%.C=%.d)
TARGETS = test-triang2d-construct
EXEC =  $(TARGETS)

include $(DEPS)



LIBS = -lgral-triang2d -lgral-base -lgeometry -lexternal-control -lutilities



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check:
	./runtest

clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



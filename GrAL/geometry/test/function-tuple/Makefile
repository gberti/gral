# geometry/test/function-tuple

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS    =  $(SOURCES:%.C=%.d)
EXEC    =  $(SOURCES:%.C=%.exe)

EXTRA_DEPMODULES +=  sequence
LIBS = -lgeometry 

include $(DEPS)




all : $(EXEC)

new-check-clean: new check clean

new : clean all

$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS)  $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



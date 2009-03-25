# sequence/test/bijective-mapping

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)


LOCALDIR = bijective-mapping
LOCALSOURCES =  test-bijective.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-bijective
EXEC =  $(TARGETS)

include $(DEPS)



LIBS =  -lutilities 



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-bijective: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



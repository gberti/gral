# sequence/test/tuple

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

EXTRA_DEPMODULES += geometry
LOCALSOURCES =  test-tuple.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC = $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)


include $(DEPS)

DEPMODULES+= 


LIBS += 



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(EXEC): $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:	
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



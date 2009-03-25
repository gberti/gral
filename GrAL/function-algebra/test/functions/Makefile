# function-algebra/test/functions

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

LOCALSOURCES =  test-functions.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)

TARGETS = test-functions
EXEC =  $(TARGETS)

include $(DEPS)



LIBS = -lfunction-algebra -lgeometry -lutilities 
#LDFLAGS += $(LIBS)


new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-functions: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS)  $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



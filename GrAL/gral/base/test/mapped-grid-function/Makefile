# gral/base/test/mapped-grid-function

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS    =  $(SOURCES:%.C=%.d)
EXEC    =  $(SOURCES:%.C=%.exe)

#EXTRA_CCFLAGS  
EXTRA_DEPMODULES +=  gral/cartesiannd
LIBS =  -lgral-base 


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



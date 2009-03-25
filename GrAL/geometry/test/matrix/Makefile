# geometry/test/matrix

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS    =  $(SOURCES:%.C=%.d)
EXEC    =  $(SOURCES:%.C=%.exe)

#EXTRA_CCFLAGS += -DGRAL_NO_ABORT -DGRAL_NO_SHOW_DATE -DNMWR_DEBUG
EXTRA_CCFLAGS +=  -DGRAL_NO_SHOW_DATE -DNMWR_DEBUG
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

check: 
	$(MAKE) run-check FILTER=$(CONFIGURATION)/scripts/floating-point-filter.pl


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



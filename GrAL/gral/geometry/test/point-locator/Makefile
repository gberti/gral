# gral/geometry/point-locator/

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC =  $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)


include $(DEPS)

EXTRA_DEPMODULES+= geometry  gral/cartesiannd

LIBS = -lgeometry -lgral-base



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(EXEC): $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: 
	$(MAKE) run-check FILTER=$(CONFIGURATION)/scripts/floating-point-filter.pl


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



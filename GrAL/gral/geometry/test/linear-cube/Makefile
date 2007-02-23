# gral/geometry/linear-cube

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC =  $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)


include $(DEPS)

EXTRA_DEPMODULES+= geometry  gral/complex2d gral/cartesiannd

LIBS = -lgral-base -lgral-complex2d -lgeometry -lutilities



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



# gral/geometry/test/intersection-ray-grid

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  test-intersection-ray-grid.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC =  $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)


include $(DEPS)

EXTRA_DEPMODULES+= geometry  gral/complex2d gral/cartesiannd

LIBS = -lgral-base -lgral-complex2d -lgeometry 



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



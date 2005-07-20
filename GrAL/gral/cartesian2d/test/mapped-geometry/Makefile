# gral/cartesian2d/test/mapped-geometry

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

EXTRA_DEPMODULES +=  geometry

SOURCES = $(shell ls *.C)
OBJECTS = $(SOURCES:%.C=%.o)
DEPS    = $(SOURCES:%.C=%.d)
EXEC    = $(SOURCES:%.C=%.exe)
TARGETS = $(EXEC)


include $(DEPS)



LIBS = -lgral-base -lgral-cartesian2d -lgeometry


new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(TARGETS): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: 
	$(MAKE) run-check FILTER=$(CONFIGURATION)/scripts/floating-point-filter.pl



clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	rm -f oct.complex2d


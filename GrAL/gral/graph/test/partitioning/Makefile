# gral/graph/partitioning

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC =  $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)


include $(DEPS)

EXTRA_DEPMODULES+= geometry  gral/complex2d gral/cartesian2d gral/gmv-io

LIBS =   -lgral-gmv-io -lgral-complex2d -lgral-cartesian2d -lgral-base -lgeometry -lexternal-control


.SUFFIXES: .exe .C 

new-check-clean: new check clean

new : clean all


all : $(TARGETS)

.o.exe :
	$(CC) -o $@ $<  $(LDFLAGS) $(LIBS)

#$(EXEC): $(LOCALOBJECTS)
#	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)


run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



# gral/base/test/cell-on-cell-iterator

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)

EXEC =  $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)


include $(DEPS)

EXTRA_DEPMODULES+= gral/cartesian2d gral/complex2d \
                   gral/testing geometry

LIBS = -lgral-cartesian2d  -lgral-complex2d \
       -lgral-base -lgeometry


echo:
	@echo DEPMODULES_0_PATHS: $(DEPMODULES_0_PATHS)
	@echo DEPMODULES_1:       $(DEPMODULES_1)

new-check-clean: new check clean

new :  clean all


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



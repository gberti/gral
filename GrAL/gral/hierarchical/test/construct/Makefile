# gral/hierarchical/test/construct

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

EXTRA_DEPMODULES += gral/complex2d  gral/cartesian3d gral/cartesian2d gral/cartesiannd
ADDCCFLAGS += -DGRAL_NO_ABORT -DGRAL_NO_SHOW_DATE -DNMWR_DEBUG
LOCALSOURCES =  test-hier-construct.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC = $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)

include $(DEPS)




LIBS = -lgral-cartesian3d -lgral-cartesian2d -lgral-complex2d -lgral-base -lutilities





new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(TARGETS): $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



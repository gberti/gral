# gral/cartesiannd/test/gf

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

EXTRA_DEPMODULES += gral/testing
#ADDCCFLAGS += -DGRAL_NO_ABORT -DGRAL_NO_SHOW_DATE -DNMWR_DEBUG
LOCALSOURCES =  test-cartesiannd-gf.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC = $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)

include $(DEPS)

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



# gral/generaton/test/Grummp

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

EXTRA_CCFLAGS+=-DLINUX -DANSI_DECLARATORS
EXTRA_IFLAGS+=-I$(GRUMMP_INC)
LOCALSOURCES =  test-grummp-main.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-grummp
EXEC =  $(TARGETS)

include $(DEPS)

DEPMODULES+= gral/dx-io gral/complexnd gral/comple2d gral/complex3d

LDFLAGS += -L$(GRUMMP_LIB)
LIBS = -lgral-complex2d -lgral-dx-io -lgral-generation -lgral-base -lgeometry -lGR_base -lGR_2D -lOptMS -lSUMAAlog_lite -lm

new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-grummp: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	rm -f dane.mesh dane.qual dane.msg



# gral/generaton/test/Triangle
PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config

include $(CONFIG)

EXTRA_CCFLAGS+=-DLINUX -DANSI_DECLARATORS --bounds-checking
EXTRA_IFLAGS+=-I$(TRIANGLE_INC)

LOCALSOURCES =  test-triangle-main.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-triangle test-trianglePro
EXEC = $(TARGETS)

DEPMODULES+= gral/dx-io gral/complexnd

include $(DEPS)


LDFLAGS += -L$(TRIANGLE_LIB)
LIBS = -lgral-complex2d -lgral-dx-io \
       -lgral-generation -lgral-base -lgeometry -ltriangle

new-check-clean: new check clean

new : clean all


all : $(TARGETS)

test-triangle: test-triangle-main.o 
	$(CC) -o $@ $(OBJECTS) $^ $(LDFLAGS) $(LIBS)

test-trianglePro: test-trianglePro-main.o 
	$(CC) -o $@ $(OBJECTS) $^ $(LDFLAGS) $(LIBS)



run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



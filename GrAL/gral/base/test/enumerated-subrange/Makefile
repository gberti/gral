# gral/base/test/enumerated-subrange

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  $(shell ls *.C)
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-enumerated-subrange
EXEC =  $(TARGETS)

include $(DEPS)

DEPMODULES+= gral/cartesian2d gral/base  geometry 


LIBS = -lgral-cartesian2d  \
       -lgral-base -lgeometry



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-enumerated-subrange: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:	
	$(EXEC) 

check:
	./runtest ./$(EXEC) 

clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



# gral/complex3d/test/construct

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

ALLDEPMODULES+= gral/cartesian3d
SOURCES =  test-construct-complex3d.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS     = $(SOURCES:%.C=%.d)
TARGETS = test-construct-complex3d
EXEC =  $(TARGETS)

include $(DEPS)



LIBS = -lgral-cartesian3d -lgral-complex3d -lgral-complex2d -lgral-base\
       -lexternal-control -lutilities 



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-construct-complex3d: $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



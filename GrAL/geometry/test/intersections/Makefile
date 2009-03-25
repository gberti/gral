# geometry/test/intersection

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

LOCALSOURCES =  test-intersections.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC =  $(LOCALSOURCES:%.C=%.exe)

#BOOST=$(HOME)/sources/boost/boost/
#IFLAGS += -I$(BOOST)

TARGETS = $(EXEC)
EXTRA_DEPMODULES +=  sequence

include $(DEPS)


LIBS = -lgeometry 


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



# gral/partitioning/test/metis

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  test-metis-main.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-metis
EXEC =  $(TARGETS)

include $(DEPS)


LDFLAGS += -L$(METIS_LIB)
LIBS = -lgral-complex2d -lgral-partitioning -lmetis



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-metis: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check:
	./runtest

clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



# utilities/test/reference-count

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

LOCALDIR = refcount
LOCALSOURCES =  test-refcount.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)

TARGETS = test-refcount
EXEC =  $(TARGETS)

include $(DEPS)



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-refcount: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check:
	./runtest

clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



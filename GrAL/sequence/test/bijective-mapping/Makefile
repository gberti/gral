# $(NMWRROOT)/test/bijective-mapping

include ../../nmwr.config
include $(NMWRROOT)/defs/Makefile.test.leaf


LOCALDIR = bijective-mapping
LOCALSOURCES =  test-bijective.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
LOCALTARGET = test-bijective

include $(DEPS)

EXEC =  $(LOCALTARGET)

TARGETS =  local

LIBS =  -lnmwr-io -lutility 



new : clean  all


all : $(TARGETS)


local: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check:
	./runtest

clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out /tmp/br-schenk*.tree


depend:



# gral/complex3d/test/boundary

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

LOCALSOURCES =  test-boundary.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC =  $(LOCALSOURCES:%.C=%.exe)

TARGETS = $(EXEC)
EXTRA_DEPMODULES += gral/cartesian3d sequence 


include $(DEPS)



LIBS = -lgral-complex3d -lgral-cartesian3d -lgral-complex2d -lgral-base -lgeometry  


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
	rm -f oct.complex2d


# gral/hierarchical/test/geometry

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

EXTRA_DEPMODULES += gral/complex2d  gral/cartesian3d gral/cartesian2d
LOCALSOURCES =  test-hier-geometry.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGET = exec
EXEC =  $(LOCALSOURCES:%.C=%.exe)

include $(DEPS)


LIBS = -lgral-cartesian3d -lgral-cartesian2d -lgral-complex2d -lgral-base





new-check-clean: new check clean

new : clean all


all : $(TARGET)


$(TARGET): $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	rm -f oct.complex2d


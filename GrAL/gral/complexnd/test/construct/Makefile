# gral/complexnd/test/construct

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

EXTRA_DEPMODULES += gral/testing gral/cartesian2d gral/cartesian3d gral/complex2d
LIBS = -lgral-cartesian2d -lgral-cartesian3d -lgral-complex2d -lgral-base 
LOCALSOURCES =  test-complexnd-construct.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC = $(LOCALSOURCES:%.C=%.exe)
TARGETS = $(EXEC)

include $(DEPS)

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
	rm -f oct.complex2d


# gral/tools/converter

DEPMODULES+= gral/base gral/gmv-io gral/geomview-io \
             external-control configuration utilities geometry sequence

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)



SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)

EXECOFF = complex2d2off
EXECGMV = complex2d2gmv
EXEC =  $(EXECOFF) $(EXECGMV)
TARGETS = $(EXEC)

include $(DEPS)


LIBS = -lgral-base \
       -lexternal-control  -lutilities -lgeometry

LIBSGMV = -lgral-gmv-io
LIBSOFF = -lgral-geomview-io


new-check-clean: # new check clean1
	$(MAKE) new
	$(MAKE) check
	$(MAKE) clean

new : clean all


all : $(TARGETS)


complex2d2gmv: complex2d2gmv.o
	$(CC) -o complex2d2gmv complex2d2gmv.o $(LDFLAGS) $(LIBSGMV) $(LIBS)

complex2d2off: complex2d2off.o
	$(CC) -o complex2d2off complex2d2off.o $(LDFLAGS) $(LIBSOFF) $(LIBS)

rungmv:
	$(EXECGMV) -g grid.complex2d -o grid.gmv  -off 1

runoff:
	$(EXECOFF) -g grid.complex2d -o grid.off -off 1


clean: stdclean
	@rm -f $(EXEC)
	@rm -f *.o *~ *.flc core
	@rm -f .depend *.oogl *out 



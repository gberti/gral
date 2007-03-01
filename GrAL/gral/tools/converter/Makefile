# gral/tools/converter


PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

EXTRA_DEPMODULES+= gral/base gral/gmv-io gral/geomview-io gral/complex2d gral/cartesian2d \
             external-control configuration utilities geometry sequence


SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)

EXECOFF = complex2d2off
EXECGMV = complex2d2gmv
EXECCOMPLEX2D = complex2d2complex2d 
EXECCARTCOMPLEX2D = cartesian2complex2d
EXEC =  $(EXECOFF) $(EXECGMV) $(EXECCOMPLEX2D)  $(EXECCARTCOMPLEX2D)
TARGETS = $(EXEC)

include $(DEPS)


LIBS = -lgral-complex2d -lgral-base \
       -lexternal-control  -lutilities -lgeometry

LIBSGMV = -lgral-gmv-io
LIBSOFF = -lgral-geomview-io
LIBSDX  = -lgral-complex2d 
LIBSCART = -lgral-cartesian2d

new-check-clean: # new check clean1
	$(MAKE) new
	$(MAKE) check
	$(MAKE) clean

new : clean all


all : $(TARGETS)


cartesian2complex2d: cartesian2complex2d.o
	$(CC) -o cartesian2complex2d  cartesian2complex2d.o $(LDFLAGS) $(LIBSCART) $(LIBS)

complex2d2complex2d: complex2d2complex2d.o
	$(CC) -o complex2d2complex2d complex2d2complex2d.o $(LDFLAGS) $(LIBS)

complex2d2gmv: complex2d2gmv.o
	$(CC) -o complex2d2gmv complex2d2gmv.o $(LDFLAGS) $(LIBSGMV) $(LIBS)

complex2d2off: complex2d2off.o
	$(CC) -o complex2d2off complex2d2off.o $(LDFLAGS) $(LIBSOFF) $(LIBS)

complex2d2dx: complex2d2dx.o
	$(CC) -o complex2d2dx complex2d2dx.o $(LDFLAGS) $(LIBSDX) $(LIBS)

complex2d2stl: complex2d2stl.o
	$(CC) -o $@ $< $(LDFLAGS)  $(LIBS)

complex2d2smp: complex2d2smp.o
	$(CC) -o $@ $< $(LDFLAGS)  $(LIBS)

rungmv:
	$(EXECGMV) -g grid.complex2d -o grid.gmv  -off 1

runoff:
	$(EXECOFF) -g grid.complex2d -o grid.off -off 1


clean: stdclean
	@rm -f $(EXEC)
	@rm -f *.o *~ *.flc core
	@rm -f .depend *.oogl *out 



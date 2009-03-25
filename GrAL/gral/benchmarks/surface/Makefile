# $(GRAL)/benchmarks/surface

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

EXTRA_DEPMODULES+= external-control geometry sequence \
             gral/base  gral/complex2d gral/triang2d gral/cartesian2d

CCSOURCES = bench-surface.C 
F77SOURCES = loop.f 
CSOURCES = loopC.c

CCOBJECTS =  $(CCSOURCES:%.C=%.o)
COBJECTS =   $(CSOURCES:%.c=%.o)
F77OBJECTS = $(F77SOURCES:%.f=%.o)
LOCALOBJECTS = $(CCOBJECTS) $(F77OBJECTS) $(COBJECTS)
DEPS=$(CCSOURCES:%.C=%.d)
include $(DEPS)

LOCALTARGET =  bench-surface

F77NAME=ifc70#
CCNAME=gcc3#
CNAME=gcc3#

include f77/$(F77NAME).defs
include c/$(CNAME).defs
include c++/$(CCNAME).defs

# F77=/usr/net/pgi/linux86/bin/pgf77
# CCOMP=/usr/net/pgi/linux86/bin/pgcc

#F77=/usr/net/lnx/INTEL/FORTRAN/compiler70/ia32/bin/ifc
#CCOMP=$(HOME)/bin/gcc3# 

.f.o:
	$(F77) -c $(F77OPTFLAGS) $<

.c.o:
	$(CCOMP) -c $(COPTFLAGS) $<

#COPTFLAGS= -funroll-all-loops -O3 -ffast-math -fexpensive-optimizations \
#           -finline-limit=1000000
#COPTFLAGS= -fast
#ifeq (linux, $(ARCH))
#F77OPTFLAGS= -O3 -fforce-addr -funroll-loops 
#F77OPTFLAGS = -O2 #-fast
#else
#F77OPTFLAGS= -fast  -unroll=3
#endif


LDFLAGS += 
LIBS += -lgral-cartesian2d -lgral-complex2d -lgral-triang2d \
        -lgral-base \
        -lexternal-control -lgeometry \
        -lutilities 



WORKDIR = $(APPDIR)/test/$(LOCALDIR)

#ifeq (KCC,$(CCC))
#CCOPTFLAGS= +K3 --abstract_pointer --abstract_float --restrict --inline_implicit_space_time=100
#CCFLAGS =  -DNMWR_INCLUDE_TEMPLATE_DEFS $(CCOPTFLAGS) 
#else
#CCOPTFLAGS=-O3 -finline-functions -funroll-loops #-Winline
#CCFLAGS = -ftemplate-depth-50 \
#          -DNMWR_INCLUDE_TEMPLATE_DEFS -DNMWR_PARTIAL_SPEC $(CCOPTFLAGS) 
#endif

EXEC =  $(LOCALTARGET)

TARGETS = local

new: clean  all

all : $(TARGETS)

local: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS) 

clean: stdclean
	rm -f $(EXEC) 
	rm -f *.o *~ *.flc core make.out.*






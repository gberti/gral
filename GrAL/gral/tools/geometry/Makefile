# gral/tools/gridinfo

EXTRA_DEPMODULES+= gral/base gral/complex2d gral/geometry gral/cartesiannd\
             external-control configuration utilities geometry sequence

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)



SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)


EXEC =  $(SOURCES:%.C=%.exe)
TARGETS = $(EXEC)

include $(DEPS)


LIBS = -lgral-complex2d -lgral-base \
       -lexternal-control -lsequence -lutilities -lgeometry



new-check-clean: # new check clean1
	$(MAKE) new
	$(MAKE) check
	$(MAKE) clean

new : clean all


all : $(TARGETS)


project-to-surface.exe: project-to-surface.o
	$(CC) -o $@ $< $(LDFLAGS) $(LIBS)

clean: stdclean
	@rm -f $(EXEC)
	@rm -f *.o *~ *.flc core
	@rm -f .depend *.oogl *out 



# gral/tools/converter

EXTRA_DEPMODULES+= gral/base gral/complex2d \
             external-control configuration utilities geometry sequence

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)



SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)


EXEC =  gridinfo
TARGETS = $(EXEC)

include $(DEPS)


LIBS = -lgral-complex2d -lgral-base \
       -lexternal-control  -lutilities -lgeometry




new-check-clean: # new check clean1
	$(MAKE) new
	$(MAKE) check
	$(MAKE) clean

new : clean all


all : $(TARGETS)


$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBSGMV) $(LIBS)

clean: stdclean
	@rm -f $(EXEC)
	@rm -f *.o *~ *.flc core
	@rm -f .depend *.oogl *out 



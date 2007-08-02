# gral/measurement/test/compare-grids

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


SRC  =  $(shell ls *.C)
OBJ  =  $(SRC:%.C=%.o)
DEPS =  $(SRC:%.C=%.d)
EXEC =  test-compare-grids

include $(DEPS)

EXTRA_DEPMODULES+= gral/measurement gral/cartesiannd geometry

LIBS = -lgral-base\
       -lgeometry -lutilities



new-check-clean: # new check clean1
	$(MAKE) new
	$(MAKE) check
	$(MAKE) clean

new : clean all


all : $(EXEC)


$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	@rm -f $(EXEC)
	@rm -f *.o *~ *.flc core
	@rm -f .depend *.oogl *out 



# gral/base/test/vertex-stars

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


SRC  =  $(shell ls *.C)
OBJ  =  $(SRC:%.C=%.o)
DEPS =  $(SRC:%.C=%.d)
EXEC =  test-vertex-stars

include $(DEPS)

DEPMODULES+= gral/cartesian2d geometry

LIBS = -lgral-cartesian2d -lgral-base



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

check:
	./runtest

clean: stdclean
	@rm -f $(EXEC)
	@rm -f *.o *~ *.flc core
	@rm -f .depend *.oogl *out 



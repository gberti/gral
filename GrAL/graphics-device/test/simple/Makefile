# graphics-device/test/simple

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


SRC  =  $(shell ls *.C)
OBJ  =  $(SRC:%.C=%.o)
DEPS =  $(SRC:%.C=%.d)
EXEC =  test-simple-graphics

include $(DEPS)

DEPMODULES+=  geometry

LIBS = -lgraphics-device -lfunction-algebra -lgeometry  -lutilities 
#LIBS = -lgraphics-device -lfunction-algebra -lutilities \
#      $(HOME)/CVS-work/geometry/lib/solaris/gcc/debug/libgeometry.a

#$(HOME)/CVS-work/geometry/src/Geometry/obj.tmp/point.o



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
	$(MAKE) run-check FILTER=$(CONFIGURATION)/scripts/floating-point-filter.pl


clean: stdclean
	@rm -f $(EXEC)
	@rm -f *.o *~ *.flc core
	@rm -f .depend *.oogl *out 



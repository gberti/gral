# generic Makefile for Obj directories

include $(CONFIG)


VPATH=..
OBJ  = $(SRC:%.C=%.o)
DEPS = $(SRC:%.C=%.d)

include $(DEPS)

CCFLAGS += $(LIBCCFLAGS)

new: clean all

all: compile lib

compile: $(OBJ)

lib: $(OBJ)
	$(AR) $(ARFLGS) $(LIB) $(?);

clean: stdclean
	rm -f *.o *~ *.flc core .depend *.d
	rm -rf tempinc


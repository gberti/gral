# gral/vtk/test/construct

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

SOURCES =  test-construct-vtkugrid.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS    =  $(SOURCES:%.C=%.d)
EXEC    =  $(SOURCES:%.C=%.exe)
EXTRA_IFLAGS += -I$(VTK_INC)

EXTRA_DEPMODULES = gral/triang3d gral/cartesian3d

##CCFLAGS += -Wno-deprecated
EXTRA_LDFLAGS += -L$(VTK_LIB)
VTKLIBS = -lvtkFiltering -lvtkGraphics -lvtkIO  -lvtkCommon 

LIBS = -lgral-base -lgeometry -lgral-triang3d -lgral-cartesian3d -lgral-complex2d \
       -lsequence -lutilities $(VTKLIBS) -ldl -lpthread

include $(DEPS)

new-check-clean: new check clean

new : clean all

all : $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@  $^ $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check

clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out  *.vtk

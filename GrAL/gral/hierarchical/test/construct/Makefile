# gral/hierarchical/test/construct

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

ALLDEPMODULES += gral/complex2d  gral/cartesian3d gral/cartesian2d
ADDCCFLAGS += -DGRAL_NO_ABORT -DGRAL_NO_SHOW_DATE -DNMWR_DEBUG
LOCALSOURCES =  test-hier-construct.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
EXEC = $(LOCALSOURCES:%.C=%.exe)
TARGET = $(EXEC)

include $(DEPS)




LIBS = -lgral-cartesian3d -lgral-cartesian2d -lgral-complex2d -lgral-base





new-check-clean: new check clean

new : clean all


all : $(TARGET)


$(TARGET): $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



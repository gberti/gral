# gral/base/base/stream-grid-adapter

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

SOURCES =  test-stream-grid-adapter.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC = $(SOURCES:%.C=%.exe)
TARGETS = $(EXEC)
EXTRA_DEPMODULES = geometry 

include $(DEPS)

LIBS += -lgral-base -lgeometry -lutilities


new-check-clean: new check clean

new : clean all


all : $(TARGETS)

$(TARGETS): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



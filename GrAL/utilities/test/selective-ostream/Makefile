# utilities/test/selective-ostream

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

SOURCES =  $(shell ls *.C)
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC =  $(SOURCES:%.C=%.exe)

EXTRA_CCFLAGS += -DGRAL_NO_ABORT -DGRAL_NO_SHOW_DATE -DGRAL_NO_SHOW_LINE -DNMWR_DEBUG

include $(DEPS)


new-check-clean: new check clean

new: clean all


all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check:
	 $(MAKE) run-check FILTER=$(CONFIGURATION)/scripts/program-error-mgs-filter.pl


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



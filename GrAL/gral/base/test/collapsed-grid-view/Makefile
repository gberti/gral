# gral/base/test/collapsed-grid-view

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

SOURCES =  test-collapsed-grid-view.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC = $(SOURCES:%.C=%.exe)
TARGETS = $(EXEC)
EXTRA_DEPMODULES = geometry external-control gral/triang2d gral/gmv-io gral/testing

include $(DEPS)

LIBS +=  -lgral-triang2d -lgral-gmv-io -lgral-base -lgeometry -lexternal-control -lsequence -lutilities


new-check-clean: new check clean

new : clean all


all : $(TARGETS)

simplify-mesh.exe: simplify-mesh.o
	$(CC) -o $@ $< $(LIBS) $(LDFLAGS)

$(TARGETS): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check check-grids

check-grids: run-check
	@(echo='echo -e'; \
          for i in *.gmv; \
          do \
            if diff $$i $$i.exp; \
            then  $$echo "\t\t\t" $$i OK; \
            else  $$echo "\t\t\t" ERROR in $$i; \
            fi ;\
           done ;\
           $$echo "\n"; \
        )


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



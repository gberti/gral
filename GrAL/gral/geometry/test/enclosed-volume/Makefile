# gral/geometry/test/enclosed-volume

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)

SOURCES =  test-enclosed-volume.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS = $(SOURCES:%.C=%.d)
EXEC = $(SOURCES:%.C=%.exe)
TARGETS = $(EXEC)
EXTRA_DEPMODULES = gral/complex2d 

include $(DEPS)

LIBS +=  -lgral-complex2d  -lgral-base -lgeometry -lsequence -lutilities


new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(TARGETS): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

mkoutput:
	mkdir -p output

run-check: mkoutput

run: mkoutput
	$(EXEC) 

check: run-check
	@(echo='echo -e'; \
        /bin/rm -rf output/*.filtered.out; \
        for i in output/*; \
        do \
         $(FILTER) $$i > $$i.filtered.out; \
         b=`basename $$i`; \
         if diff $$i.filtered.out exp/$$b; \
             then  $$echo "\t\t\t" $$i OK; \
             else  $$echo "\t\t\t" ERROR in $$i; \
         fi ;\
        done;)


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	rm -rf output



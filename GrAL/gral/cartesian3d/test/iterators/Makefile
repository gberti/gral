# gral/cartesian3d/test/iterators

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)


LOCALSOURCES =  test-cartesian-iterators.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-cartesian-iterators
EXEC =  $(TARGETS)

include $(DEPS)



LIBS = -lgral-cartesian3d -lgral-complex2d -lgral-base -lgeometry -lutilities





new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-cartesian-iterators: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check



standard-check-new:
	@(out=$(EXEC).out; \
          echo='echo -e'; \
         if ! ./$(EXEC)  > $$out '2>&1' ; \
         then \
         $$echo "ERROR running $(EXEC) "; \
         echo "Output was:"; \
         cat $$out; \
         $$echo " End output "; \
         else \
         $$echo "\n"; \
         for i in  *.out; \
         do \
         if diff $$i $$i.exp; \
         then  $$echo "\t\t\t" $$i OK; \
         else  $$echo "\t\t\t" ERROR in $$i; \
         fi ;\
         done ;\
         $$echo "\n"; \
         fi)

testit:
	@(out=out; \
         if ! ./$(EXEC)  > $$out '2>&1' ; \
	 then \
         echo " failed "; \
         else \
         echo " succeeded "; \
         fi)

clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 



# adapt-mouse/test/distributed

CONFIG=../../mk.config
include $(CONFIG)

IFLAGS  += $(MPI_INCLUDES)
LDFLAGS += $(MPI_LIBRARIES)

LOCALSOURCES =  test-distributed.C test-distri-io.C 
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS =  $(LOCALSOURCES:%.C=%.d)
LOCALTARGET = test-distributed
#MPIPATH=/usr/lib/mpich
#MPIRUN=$(MPIPATH)/bin/mpirun
#IFLAGS += -I$(MPIPATH)/include
LIBS = -lgral-distributed -lgral-complex2d -lgral-base -lexternal-control -lutilities



include $(DEPS)

EXEC =  $(LOCALTARGET)






new: clean all

all : test-distributed test-distri-io

test-distributed: test-distributed.o
	$(CC) -o $@ $< $(LDFLAGS) $(LIBS)

test-distri-io: test-distri-io.o
	$(CC) -o $@ $< $(LDFLAGS) $(LIBS)


new-check-clean: new check clean

clean: stdclean
	rm -f $(EXEC) 
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out dgf.? grid.complex2d grid.part out
	rm -f PI* grid.master-local.complex2d 

NP=2
run:
	$(MPIRUN) -v -np $(NP) -machinefile ./mpihosts $(GDB)  test-distributed

check:
	./runtest $(MPIRUN)






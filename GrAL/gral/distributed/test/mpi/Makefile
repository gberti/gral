# gral/distributed/test/mpi

CONFIG=../../mk.config
include $(CONFIG)

IFLAGS  += $(MPI_INCLUDES)
LDFLAGS += $(MPI_LIBRARIES)

LOCALSOURCES =  test-distributed.C test-distri-io.C 
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS =  $(LOCALSOURCES:%.C=%.d)
LOCALTARGET = test-distributed test-distri-io
#MPIPATH=/usr/lib/mpich
#MPIRUN=$(MPIPATH)/bin/mpirun
#IFLAGS += -I$(MPIPATH)/include
LIBS = -lgral-distributed -lgral-complex2d -lgral-base \
       -lgeometry -lexternal-control -lutilities



include $(DEPS)

EXEC =  $(LOCALTARGET)




env:
	@echo "ENVIRONMENT CHECK"
	@printenv | grep LD  || echo ""
	@$(MPICC) -v         || echo ""
	@echo "-----------------"

new: env clean all

all : $(LOCALTARGET)

CC:=$(MPICC)
#DEPEND=$(MPICC) -v
DEPFLAGS += -I/usr/local/mpi/include -I/usr/local/mpi/include/mpi2c++

test-distributed: test-distributed.o
	$(MPICC) -o $@ $< $(LDFLAGS) $(LIBS)

test-distri-io: test-distri-io.o
	$(MPICC) -o $@ $< $(LDFLAGS) $(LIBS)


new-check-clean: new check clean

clean: stdclean
	rm -f $(EXEC) 
	rm -f *.o *~ *.flc core *.d
	rm -f .depend *.oogl *.out dgf.? grid.complex2d grid.part out
	rm -f PI* grid.master-local.complex2d 

NP=2
run:
	$(MPIRUN) -np $(NP)  $(GDB)  test-distributed
#	$(MPIRUN) -v -np $(NP) -machinefile ./mpihosts $(GDB)  test-distributed

check:
	./runtest    $(MPIRUN)
	sleep 10
	./runtest-io $(MPIRUN)






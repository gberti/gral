# adapt-mouse/test/distributed

CONFIG=../../mk.config
include $(CONFIG)

IFLAGS  += $(MPI_INCLUDES)
LDFLAGS += $(MPI_LIBRARIES)

LOCALSOURCES =  test-distributed.C 
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS =  $(LOCALSOURCES:%.C=%.d)
LOCALTARGET = test-distributed
#MPIPATH=/usr/lib/mpich
#MPIRUN=$(MPIPATH)/bin/mpirun
#IFLAGS += -I$(MPIPATH)/include
LDFLAGS +=  -R$(MOUSE)/lib 
LIBS = -lgral-distributed -lgral-complex2d  -lexternal-control -lutilities



include $(DEPS)

EXEC =  $(LOCALTARGET)

mpitest.o:
	$(CC) -c  $(MPI_INCLUDE) mpitest.C

mpitest: mpitest.o
	$(CC) -o mpitest mpitest.o $(MPI_LIBRARIES)



new: clean all

all : $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(LOCALOBJECTS)  $(LDFLAGS) $(LIBS) 

#	$(CC) -o $(EXEC) $(LOCALOBJECTS) ./libmouse.a  $(LDFLAGS) $(LIBS) 

new-check-clean: new check clean

clean: stdclean
	rm -f $(EXEC) 
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out dgf.? grid.complex2d grid.part out
	rm -f PI* grid.master-local.complex2d 

NP=2
run:
	$(MPIRUN) -v -np $(NP) -machinefile ./mpihosts $(GDB)  $(LOCALTARGET)   

check:
	./runtest $(MPIRUN)






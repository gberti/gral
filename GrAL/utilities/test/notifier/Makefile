# utilities/test/ref-ptr

PWD=$(shell pwd)
CONFIG=$(PWD)/../../mk.config
include $(CONFIG)

LOCALSOURCES =  test-notifier.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGET = exec
EXEC =  $(LOCALSOURCES:%.C=%.exe)

include $(DEPS)

LIBS = -lutilities

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



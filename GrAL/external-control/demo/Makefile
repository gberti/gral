include $(CONFIG)

CTRLLIB=control-device
LIBS = -l$(CTRLLIB) -lnmwr-io


ex1: ex1.o
	$(CC) -o $(<:%.o=%.exe) $< $(LDFLAGS) $(LIBS)

all: ex1

new: clean all

clean: stdclean
	rm -f *.o *.exe
export CONFIG:=$(shell pwd)/mk.config
unexport PWD
include $(CONFIG)



install-headers:
	mkdir -p $(INSTALLHEADERS)/Gral/Test
	cp include/Gral/Test/*.h $(INSTALLHEADERS)/Gral/Test
	cp include/Gral/Test/*.C $(INSTALLHEADERS)/Gral/Test


new: libclean

all:

test:

check:

demo:

test-check-clean:

clean: stdclean

realclean: clean
	rm -rf doxygen

ADDRELPATH=..

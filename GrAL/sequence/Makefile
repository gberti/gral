export CONFIG:=$(shell pwd)/mk.config
include $(CONFIG)


TARGET=all

HEADERS= $(shell ls include/Container/*.h)
all:

new: libclean


compile:

lib:


test: standard-test

check: standard-check 

test-check-clean: standard-test-check-clean


install-headers:
	mkdir -p $(INSTALLHEADERS)/Container
	cp include/Container/*.h $(INSTALLHEADERS)/Container
	cp include/Container/*.C $(INSTALLHEADERS)/Container

clean: stdclean
	(cd test; $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")
	rm -f *~ include/*~ include/*/*~

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen/






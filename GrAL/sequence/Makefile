export CONFIG:=$(shell pwd)/local.config
include $(CONFIG)


TARGET=all

HEADERS= $(shell ls include/Container/*.h)

all:
	(cd src; $(MAKE) all)

new: libclean
	(cd src; $(MAKE) new)

compile:
	(cd src; $(MAKE) compile)

lib:
	(cd src; $(MAKE) lib)


test: standard-test

check: standard-check 

test-check-clean: standard-test-check-clean


install-headers: std-install-headers

clean: stdclean
	(cd test; $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")
	rm -f *~ include/*~ include/*/*~

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen/






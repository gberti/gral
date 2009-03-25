export CONFIG:=$(shell pwd)/local.config
unexport PWD
include $(CONFIG)


TARGET=all

all:
	(cd src; $(MAKE) all)

new: libclean
	(cd src; $(MAKE) new)


compile:

lib:

test: standard-test

demo:

check: standard-check 

test-check-clean: standard-test-check-clean


DIRS = IO

install-headers: std-install-headers

install: install-headers install-libs

clean: stdclean
	(cd src;  $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))
	(cd test; $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))

#	(cd doc;  $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))

tarclean: clean
	rm -rf lib/

realclean: tarclean
	rm -rf doxygen/
	(cd src;  $(MAKE) realclean DEPS="" GRALROOT=$(GRALROOT))
	(cd test; $(MAKE) realclean DEPS="" GRALROOT=$(GRALROOT))

#	(cd doc; $(MAKE) realclean)


doc: 

# depth + 1
ADDRELPATH=..#




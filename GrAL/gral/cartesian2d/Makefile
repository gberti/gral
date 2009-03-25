export CONFIG:=$(shell pwd)/local.config
include $(CONFIG)


TARGET=all

all:
	(cd src; $(MAKE) all)

new: libclean
	(cd src; $(MAKE) new)

compile:
	(cd src; $(MAKE) compile)

lib:
	(cd src; $(MAKE) lib)



test: standard-test

demo:

check: standard-check 

test-check-clean: standard-test-check-clean


install-headers: std-install-headers

install: install-headers  install-libs

clean: stdclean
	(cd src;  $(MAKE) clean DEPS="")
	(cd test;  $(MAKE) clean DEPS="")
	find include -name "*~" -exec rm {} \; -print

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen/


# depth + 1
ADDRELPATH=..#



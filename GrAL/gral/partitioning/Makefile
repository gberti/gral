export CONFIG:=$(shell pwd)/local.config
include $(CONFIG)


TARGET=all

all:
	(cd src; $(MAKE) new)

new: libclean
	(cd src; $(MAKE) new)


compile:

lib:

test: standard-test

demo:

check: standard-check 

test-check-clean: standard-test-check-clean


install-headers: std-install-headers

install: install-headers  install-libs

clean: stdclean
	find . -name "*~" -exec rm {} \; -print
	(cd src;  $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))
	(cd test; $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen/
	(cd src;  $(MAKE) realclean DEPS="" GRALROOT=$(GRALROOT))
	(cd test; $(MAKE) realclean DEPS="" GRALROOT=$(GRALROOT))


# depth + 1
ADDRELPATH :=..#


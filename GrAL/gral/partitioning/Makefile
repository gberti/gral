export CONFIG:=$(shell pwd)/mk.config
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


install-headers:
	mkdir -p $(INSTALLHEADERS)/Gral/Partitioning/internal
	cp include/Gral/Partitioning/*.h  $(INSTALLHEADERS)/Gral/Partitioning
	cp include/Gral/Partitioning/internal/*.C \
           $(INSTALLHEADERS)/Gral/Partitioning/internal

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


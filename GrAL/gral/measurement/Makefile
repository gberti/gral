export CONFIG:=$(shell pwd)/local.config
unexport PWD
include $(CONFIG)


TARGET=all

all:


new: libclean


compile:

lib:


test: standard-test

demo:

check: standard-check 

test-check-clean: standard-test-check-clean



install-headers: std-install-headers


clean: stdclean
	(cd test; $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))

#	(cd src;  $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))
#	(cd doc;  $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))

tarclean: clean
	rm -rf lib/

realclean: tarclean
	rm -rf doxygen/
	(cd test; $(MAKE) realclean DEPS="" GRALROOT=$(GRALROOT))

#	(cd src;  $(MAKE) realclean DEPS="" GRALROOT=$(GRALROOT))
#	(cd doc;  $(MAKE) realclean DEPS="" GRALROOT=$(GRALROOT))



doc: 



# depth + 1
ADDRELPATH=..#




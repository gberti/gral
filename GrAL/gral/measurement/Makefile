export CONFIG:=$(shell pwd)/mk.config
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


DIRS = Measurement
install-headers:
	for i in $(DIRS);\
        do mkdir -p $(INSTALLHEADERS)/Gral/$$i; \
	cp  include/Gral/$$i/*.h $(INSTALLHEADERS)/Gral/$$i || echo "no .h files in dir"; \
	cp  include/Gral/$$i/*.C $(INSTALLHEADERS)/Gral/$$i || echo "no .C files in dir"; \
        done;

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




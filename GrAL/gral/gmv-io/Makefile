export CONFIG:=$(shell pwd)/mk.config
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
#SHELL=/bin/bash

install-headers:
	for i in $(DIRS);\
        do mkdir -p $(INSTALLHEADERS)/Gral/$$i; \
	cp  include/Gral/$$i/*.h $(INSTALLHEADERS)/Gral/$$i || echo "no .h files in dir"; \
	cp  include/Gral/$$i/*.C $(INSTALLHEADERS)/Gral/$$i || echo "no .C files in dir"; \
        done;

install: install-headers install-libs

clean: stdclean
	find . -name "*~" -o -name "*.dvi" -o -name "*.aux" -o -name "*.log" -exec rm {} \;
	rm -f doxy.tags doxy.log

# 	(cd doc;  $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))

tarclean: clean
	rm -rf lib/

realclean: tarclean
	rm -rf doxygen/

#	(cd doc; $(MAKE) realclean)


doc: 



# depth + 1
ADDRELPATH=..#




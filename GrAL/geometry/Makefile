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

install-headers: std-install-headers

install: install-headers install-libs

clean: stdclean
	find . -name "*~" -o -name "*.dvi" -o -name "*.aux" -o -name "*.log" -exec rm {} \;
	rm -f doxy.tags doxy.log
	(cd test; $(MAKE) clean)
	(cd src; $(MAKE) clean)


realclean: clean libclean
	rm -rf doxygen



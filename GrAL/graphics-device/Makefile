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

install: install-headers install-libs


clean: stdclean
	(cd src;  $(MAKE) clean DEPS="")
	rm -f *~ include/*~ include/*/*~

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen



#doc: doxy tex

additional-predoc:

additional-postdoc:

additional-copydoc:



export CONFIG:=$(shell pwd)/mk.config
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


install-headers:
	mkdir -p $(INSTALLHEADERS)/FunctionAlgebra
	mkdir -p $(INSTALLLIBRARIES)
	cp include/FunctionAlgebra/*.h $(INSTALLHEADERS)/FunctionAlgebra

install: install-headers install-libs


clean: stdclean
	(cd src;  $(MAKE) clean DEPS="")
	rm -f *~ include/*~ include/*/*~

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen

test:

demo:


#doc: doxy tex

additional-predoc:

additional-postdoc:

additional-copydoc:



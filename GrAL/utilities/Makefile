export CONFIG:=$(shell pwd)/mk.config
include $(CONFIG)


TARGET=all

all:
	(cd src; $(MAKE) all)

new:
	(cd src; $(MAKE) new)


compile:
	(cd src; $(MAKE) compile)

lib:
	(cd src; $(MAKE) lib)

test:
	(cd test; $(MAKE) $(TARGET))

module-check: test-new-check-clean

install-headers:
	mkdir -p $(INSTALLHEADERS)/Utility
	mkdir -p $(INSTALLHEADERS)/Forward
	cp include/Utility/*.h $(INSTALLHEADERS)/Utility
	cp include/Forward/*.h $(INSTALLHEADERS)/Forward

install: install-headers install-libs

clean: stdclean
	(cd src;  $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")
	rm -f *~ include/*~ include/*/*~


realclean: clean
	rm -rf doxygen
	rm -rf lib/


doc: doxy



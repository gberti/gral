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

module-check: test-new-check-clean

install-headers:
	mkdir -p $(INSTALLHEADERS)/Gral/Grids/Complex2D
	cp include/Gral/Grids/Complex2D/*.h  $(INSTALLHEADERS)/Gral/Grids/Complex2D
	mkdir -p $(INSTALLHEADERS)/Gral/Grids/Complex2D/internal
	cp include/Gral/Grids/Complex2D/internal/*.h  $(INSTALLHEADERS)/Gral/Grids/Complex2D/internal
	cp include/Gral/Grids/Complex2D/internal/*.C  $(INSTALLHEADERS)/Gral/Grids/Complex2D/internal

install: install-headers  install-libs

clean: stdclean
	(cd src;  $(MAKE) clean DEPS="")
	(cd test;  $(MAKE) clean DEPS="")
	find include -name "*~" -exec rm {} \; -print

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen/

test:
	(cd test; $(MAKE) $(TARGET) CONFIG=$(CONFIG))

# depth + 1
ADDRELPATH=..#



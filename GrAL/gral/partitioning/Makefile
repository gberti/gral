export CONFIG:=$(shell pwd)/mk.config
include $(CONFIG)


TARGET=all

all:
	(cd src; $(MAKE) new)

new: libclean
	(cd src; $(MAKE) new)


compile:

lib:

install-headers:
	mkdir -p $(INSTALLHEADERS)/Gral/Partitioning/internal
	cp include/Gral/Partitioning/*.h  $(INSTALLHEADERS)/Gral/Partitioning
	cp include/Gral/Partitioning/internal/*.C \
           $(INSTALLHEADERS)/Gral/Partitioning/internal

install: install-headers  install-libs

clean: stdclean
	find . -name "*~" -exec rm {} \; -print

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen/

test:
	(cd test; $(MAKE) $(TARGET) CONFIG=$(CONFIG))



# depth + 1
ADDRELPATH :=..#


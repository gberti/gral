export CONFIG:=$(PWD)/mk.config
include $(CONFIG)


TARGET=all

all:

new:

compile:

lib:

install-headers:
	mkdir -p $(INSTALLHEADERS)/Gral/Partitioning/internal
	cp include/Gral/Partitioning/*.h  $(INSTALLHEADERS)/Gral/Partitioning
	cp include/Gral/Partitioning/internal/*.C \
           $(INSTALLHEADERS)/Gral/Partitioning/internal

install: install-headers 

clean: stdclean
	find . -name "*~" -exec rm {} \; -print

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen/

test:

# depth + 1
ADDRELPATH :=..

additional-postdoc: 
	(../bin/install-doc.pl -root $(RELROOT) \
                               -path doxygen/html \
                               -stl $(STLURL) \
                               -gral $(RELROOT)/gral/base/doc/hyperlatex/html\
         doxygen/html/*.html)

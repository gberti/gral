export CONFIG:=$(PWD)/mk.config
unexport PWD
include $(CONFIG)


TARGET=all

HEADERS= $(shell ls include/Container/*.h)
all:

new:


compile:

lib:


install:
	mkdir -p $(INSTALLHEADERS)/Container
	cp include/Container/*.h $(INSTALLHEADERS)/Container

clean:
	(cd doc;  $(MAKE) clean DEPS="")
	find . -name "*~" -o -name "*.dvi" -o -name "*.aux" -o -name "*.log" -exec rm {} \;
	rm -f doxy.tags doxy.log

tarclean: clean
	rm -rf lib/

realclean: tarclean
	rm -rf doxygen/
	(cd doc; gmake realclean)

test:
	(cd test; $(MAKE) $(TARGET) CONFIG=$(CONFIG))

doxy:
	mkdir -p doxygen
	(TEXINPUTS=$(TEXINPUTS):$(ROOT); export TEXINPUTS; doxygen doxy.cfg;)

concepts:
	(cd doc; gmake concepts)

doc: doxy concepts

TMPDIR=$(HOME)/tmp
TARDIR=$(TMPDIR)/gral/base

tar: 
	(cd $(HOME)/tmp;\
         rm -rf gral/base;\
         cvs -d /home/nmwr/CVS export -r HEAD gral/base; \
          (cd gral/base;\
            $(MAKE) doc 'CONFIG=$(TARDIR)/mk.config' 'ROOT=$(TARDIR)';\
            $(MAKE) tarclean 'CONFIG=$(TARDIR)/mk.config' 'ROOT=$(TARDIR)';\
           ); \
         tar cf gral-base.tar gral/base; \
         cvs -d /home/nmwr/CVS release -d gral/base;\
        )


testit:
	(cd $(HOME)/tmp/gral/base; \
         $(MAKE) doxy 'CONFIG=/home/berti/tmp/gral/base/mk.config');

#                      'ROOT=/home/berti/tmp/gral/base/mk.config';)

dist: doc  tarclean tar

.PHONY: demo test doc
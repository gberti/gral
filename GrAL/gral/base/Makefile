export CONFIG:=$(PWD)/mk.config
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
	(TEXINPUTS=$(TEXINPUTS):$(PWD); export TEXINPUTS; doxygen doxy.cfg;)

concepts:
	(cd doc; gmake concepts)

doc: doxy concepts


tar: 
	(cd ..; tar cf gral-base.tar base; gzip gral-base.tar)

dist: doc  tarclean tar

.PHONY: demo test doc
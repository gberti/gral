export CONFIG:=$(PWD)/mk.config
unexport PWD
include $(CONFIG)


TARGET=all

all:

new:


compile:

lib:


install:
	mkdir -p $(INSTALLHEADERS)/Geometry
	cp include/Geometry/*.h $(INSTALLHEADERS)/Geometry

clean:
	find . -name "*~" -o -name "*.dvi" -o -name "*.aux" -o -name "*.log" -exec rm {} \;
	rm -f doxy.tags doxy.log

tarclean: clean
	rm -rf lib/

realclean: tarclean
	rm -rf doxygen/
	(cd doc; gmake realclean)

doxy:
	mkdir -p doxygen
	(TEXINPUTS=$(TEXINPUTS):$(ROOT); export TEXINPUTS; doxygen doxy.cfg;)



dist: doc  tarclean tar

.PHONY: demo test doc
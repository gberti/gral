export CONFIG:=$(PWD)/mk.config
unexport PWD
include $(CONFIG)


TARGET=all

all:

new:


compile:

lib:

test:

demo:


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


.PHONY: demo test doc
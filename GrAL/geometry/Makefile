export CONFIG:=$(shell pwd)/mk.config
unexport PWD
include $(CONFIG)


TARGET=all

all:
	(cd src; $(MAKE) all)

new: libclean
	(cd src; $(MAKE) new)

compile:

lib:

test:
	 (cd test; $(MAKE) $(TARGET) CONFIG=$(CONFIG))

demo:

module-check: 
	(cd test; $(MAKE) check)

install-headers:
	mkdir -p $(INSTALLHEADERS)/Geometry
	cp include/Geometry/*.h $(INSTALLHEADERS)/Geometry
	cp include/Geometry/*.C $(INSTALLHEADERS)/Geometry

install: install-headers install-libs

clean: stdclean
	find . -name "*~" -o -name "*.dvi" -o -name "*.aux" -o -name "*.log" -exec rm {} \;
	rm -f doxy.tags doxy.log

realclean: clean
	rm -rf doxygen



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


install-headers:
	mkdir -p $(INSTALLHEADERS)/Geometry
	cp include/Geometry/*.h $(INSTALLHEADERS)/Geometry
	cp include/Geometry/*.C $(INSTALLHEADERS)/Geometry

clean: stdclean
	find . -name "*~" -o -name "*.dvi" -o -name "*.aux" -o -name "*.log" -exec rm {} \;
	rm -f doxy.tags doxy.log

realclean: clean
	rm -rf doxygen



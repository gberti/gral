export CONFIG:=$(PWD)/mk.config
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

install:
	mkdir -p $(INSTALLHEADERS)/Gral/Grids/Complex2D
	cp include/Gral/Grids/Complex2D/*.h  $(INSTALLHEADERS)/Gral/Grids/Complex2D
	mkdir -p $(INSTALLHEADERS)/Gral/Grids/Complex2D/internal
	cp include/Gral/Grids/Complex2D/internal/*.h  $(INSTALLHEADERS)/Gral/Grids/Complex2D/internal
	cp include/Gral/Grids/Complex2D/internal/*.C  $(INSTALLHEADERS)/Gral/Grids/Complex2D/internal
	cp $(LIBDIR)/*.a $(INSTALLLIBRARIES)

clean:
	(cd src;  $(MAKE) clean DEPS="")
	find include -name "*~" -exec rm {} \; -print

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen/

test:
	(cd test; $(MAKE) $(TARGET) CONFIG=$(CONFIG))

doxy:
	mkdir -p doxygen
	(TEXINPUTS=$(TEXINPUTS):$(PWD); export TEXINPUTS; doxygen doxy.cfg;)

docs:
	(cd doc; $(MAKE) $(TARGET))
	rm -f $(ROOT)/doxygen/html/external-control.html
	ln -s $(ROOT)/doc/html_dir/external-control.html $(ROOT)/doxygen/html

tar:
	(cd $(HOME)/tmp;\
	rm -rf external-control/;\
         cvs -d /home/nmwr/CVS export -r HEAD external-control; \
         tar -cf external-control.tar external-control; \
         rm -rf external-control/;)

.PHONY: demo test doc

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
	mkdir -p $(INSTALLHEADERS)/IO
	mkdir -p $(INSTALLLIBRARIES)
	cp include/IO/*.h $(INSTALLHEADERS)/IO
	cp $(LIBDIR)/*.a $(INSTALLLIBRARIES)

clean:
	(cd src;  $(MAKE) clean DEPS="")
	(cd demo; $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")
	rm -f *~ include/*~ include/*/*~

realclean: clean
	rm -rf lib/$(ARCH)/

test:
	(cd test; $(MAKE) $(TARGET) CONFIG=$(CONFIG))

doxy:
	mkdir -p doxygen
	doxygen doxy.cfg;

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

.PHONY: demo test
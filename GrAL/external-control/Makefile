export CONFIG:=$(shell pwd)/mk.config
include $(CONFIG)

TARGET=all

all:
	(cd src; $(MAKE) all)

new: libclean
	(cd src; $(MAKE) new)


compile:
	(cd src; $(MAKE) compile)

lib:
	(cd src; $(MAKE) lib)


install-headers:
	mkdir -p $(INSTALLHEADERS)/IO
	mkdir -p $(INSTALLLIBRARIES)
	cp include/IO/*.h $(INSTALLHEADERS)/IO

install: install-headers install-libs


clean: stdclean
	(cd src;  $(MAKE) clean DEPS="")
	(cd demo; $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")
	rm -f *~ include/*~ include/*/*~

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen

test:

demo:
	(cd demo; $(MAKE) $(TARGET) CONFIG=$(CONFIG))


#doc: doxy tex

additional-predoc: tex

additional-postdoc:
	(cd doc;  $(MAKE) postdoc-local)
	rm -f $(ROOT)/doxygen/html/external-control.html
	ln -s $(ROOT)/doc/html_dir/external-control.html $(ROOT)/doxygen/html

CONCEPTSPATH=doc/html_dir
additional-copydoc:

#	mkdir -p $(DOCDEST)/$(CONCEPTSPATH)
#	cp -r $(CONCEPTSPATH)/* $(DOCDEST)/$(CONCEPTSPATH)

tex:
	(cd doc; $(MAKE) html)


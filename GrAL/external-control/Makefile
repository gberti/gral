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


install-headers: std-install-headers


install: install-headers install-libs


clean: stdclean
	(cd src;  $(MAKE) clean DEPS="")
	(cd demo; $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")
	rm -f *~ include/*~ include/*/*~

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen

# no test files yet ...

test: demo

check:

test-check-clean:

demo:
	(cd demo; $(MAKE) new CONFIG=$(CONFIG))


#doc: doxy tex

additional-predoc: tex



#SHELL=/bin/bash
additional-postdoc:
	(cd doc;  $(MAKE) postdoc-local)
	rm -f $(ROOT)/doxygen/html/external-control.html
	(ln -s $(ROOT)/doc/html_dir/external-control.html \
               $(ROOT)/doxygen/html/external-control.html)

CONCEPTSPATH=doc/html_dir
additional-copydoc:

#	mkdir -p $(DOCDEST)/$(CONCEPTSPATH)
#	cp -r $(CONCEPTSPATH)/* $(DOCDEST)/$(CONCEPTSPATH)

tex:
	(cd doc; $(MAKE) html)


export CONFIG:=$(PWD)/mk.config
include $(CONFIG)


TARGET=all

HEADERS= $(shell ls include/Container/*.h)
all:

new:


compile:

lib:


install-headers:
	mkdir -p $(INSTALLHEADERS)/Container
	cp include/Container/*.h $(INSTALLHEADERS)/Container

clean:
	(cd test; $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")
	rm -f *~ include/*~ include/*/*~

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen/

test:
	(cd test; $(MAKE) $(TARGET) CONFIG=$(CONFIG))

doc: doxy tex

doxy:
	mkdir -p doxygen
	(TEXINPUTS=$(TEXINPUTS):$(PWD); export TEXINPUTS; doxygen doxy.cfg;)

tex:
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
export CONFIG:=$(PWD)/mk.config
include $(CONFIG)


TARGET=all

all:
	(cd src; $(MAKE) all)

new: realclean
	(cd src; $(MAKE) new)

compile:
	(cd src; $(MAKE) compile)

lib:
	(cd src; $(MAKE) lib)

install-headers:
	mkdir -p $(INSTALLHEADERS)/Gral/Distributed/internal
	cp include/Gral/Distributed/*.h  $(INSTALLHEADERS)/Gral/Distributed
	cp include/Gral/Distributed/internal/*.h  $(INSTALLHEADERS)/Gral/Distributed/internal
	cp include/Gral/Distributed/internal/*.C  $(INSTALLHEADERS)/Gral/Distributed/internal

install: install-headers  install-libs

clean:
	(cd src;  $(MAKE) clean DEPS="")
	(cd test;  $(MAKE) clean DEPS="")
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

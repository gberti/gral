export CONFIG:=$(PWD)/mk.config
include $(CONFIG)


TARGET=all

all:

new:

compile:

lib:

install-headers:
	mkdir -p $(INSTALLHEADERS)/Gral/Partitioning/internal
	cp include/Gral/Partitioning/*.h  $(INSTALLHEADERS)/Gral/Partitioning
	cp include/Gral/Partitioning/internal/*.C \
           $(INSTALLHEADERS)/Gral/Partitioning/internal

install: install-headers 

clean:
	find . -name "*~" -exec rm {} \; -print

realclean: clean
	rm -rf lib/$(ARCH)/
	rm -rf doxygen/

test:


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

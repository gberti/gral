export CONFIG:=$(shell pwd)/mk.config
unexport PWD
include $(CONFIG)


TARGET=all

all:

new: libclean


compile:

lib:

DIRS = Base Geometries IO Iterators Subranges Algorithms Algorithms/internal
#SHELL=/bin/bash

install-headers:
	for i in $(DIRS);\
        do mkdir -p $(INSTALLHEADERS)/Gral/$$i; \
	cp  include/Gral/$$i/*.h $(INSTALLHEADERS)/Gral/$$i || echo "no .h files in dir"; \
	cp  include/Gral/$$i/*.C $(INSTALLHEADERS)/Gral/$$i || echo "no .C files in dir"; \
        done;

clean: stdclean
	(cd doc;  $(MAKE) clean DEPS="" ALLROOT=$(ALLROOT))
	find . -name "*~" -o -name "*.dvi" -o -name "*.aux" -o -name "*.log" -exec rm {} \;
	rm -f doxy.tags doxy.log

tarclean: clean
	rm -rf lib/

realclean: tarclean
	rm -rf doxygen/
	(cd doc; $(MAKE) realclean)

test:

#	(cd test; $(MAKE) $(TARGET) CONFIG=$(CONFIG))


doc: 



# depth + 1
ADDRELPATH=..#

CONCEPTSPATH=doc/hyperlatex/html
additional-predoc:
	(cd doc; $(MAKE) concepts)

additional-postdoc: 
	(cd doc; $(MAKE) install-doc)

additional-copydoc:
	mkdir -p $(DOCDEST)/$(CONCEPTSPATH)
	cp -r $(CONCEPTSPATH)/* $(DOCDEST)/$(CONCEPTSPATH)



CONFIG=$(shell pwd)/mk.config
#unexport PWD
include $(CONFIG)


TARGET=all

all:
	(cd src; $(MAKE) all)

new: libclean
	(cd src; $(MAKE) new)

compile:

lib:



test: standard-test

demo:

check: standard-check 

test-check-clean: standard-test-check-clean


install-headers: std-install-headers

install: install-headers install-libs



clean: stdclean
	(cd doc;  $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))
	(cd src;  $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))
	(cd test; $(MAKE) clean DEPS="" GRALROOT=$(GRALROOT))
	find . -name "*~" -o -name "*.dvi" -o -name "*.aux" -o -name "*.log" -exec rm {} \;
	rm -f doxy.tags doxy.log

tarclean: clean
	rm -rf lib/

realclean: tarclean
	rm -rf doxygen/
	(cd doc;  $(MAKE) realclean DEPS="" GRALROOT=$(GRALROOT))
	(cd src;  $(MAKE) realclean DEPS="" GRALROOT=$(GRALROOT))
	(cd test; $(MAKE) realclean DEPS="" GRALROOT=$(GRALROOT))




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



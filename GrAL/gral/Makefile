export CONFIG:=$(PWD)/mk.config
unexport PWD
include $(CONFIG)


TARGET=all

all:

new:


compile:

lib:

#DIRS =  base complex2d distributed partitioning

SHELL=/bin/bash

install-headers:

clean: stdclean
	rm -f doxy.tags doxy.log

realclean: clean
	rm -rf doxygen/

test:


additional-postdoc: install-doc

install-doc:
	(./bin/install-doc.pl -root $(RELROOT) \
                             -path  $(DOXYPATH) \
                             -stl $(STLURL) \
                             -gral $(RELROOT)/gral/base/doc/hyperlatex/html \
        doxygen/html/*.html)


# assumes that RELPATH=../../..
additional-copydoc:
	cp $(DOXYPATH)/index.html $(DOCDEST)
	perl -p -i -e "s|\.\./\.\./\.\.|\.|g" $(DOCDEST)/index.html


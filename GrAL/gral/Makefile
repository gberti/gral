export CONFIG:=$(PWD)/mk.config
unexport PWD
include $(CONFIG)


TARGET=all

all:

new: clean


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



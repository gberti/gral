export CONFIG:=$(shell pwd)/mk.config
#unexport PWD
include $(CONFIG)

SUBDIRS =  base complex2d distributed partitioning

#include $(ALLROOT)/configuration/defs/recursive-targets

TARGET=all

all:

new:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $$i;echo 'ENTERING directory' $$i;\
        $(MAKE) new CONFIG=$(ALLROOT)/gral/$$i/mk.config; \
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;

generic:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $$i;echo 'ENTERING directory' $$i;\
        $(MAKE) $(TARGET) CONFIG=$(ALLROOT)/gral/$$i/mk.config; \
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;



#SHELL=/bin/bash

install-headers: 
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $$i;echo 'ENTERING directory' $$i;\
        $(MAKE) install CONFIG=$(ALLROOT)/gral/$$i/mk.config; \
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;

module-clean:
	rm -f doxy.tags doxy.log

clean: stdclean

realclean: clean
	rm -rf doxygen/

test:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $$i;echo 'ENTERING directory' $$i;\
        $(MAKE) test CONFIG=$(ALLROOT)/gral/$$i/mk.config; \
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;

doc: doc-recursive

doc-recursive:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $$i;echo 'ENTERING directory' $$i;\
        $(MAKE) doc CONFIG=$(ALLROOT)/gral/$$i/mk.config; \
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;


additional-postdoc: install-doc

install-doc:
	(./bin/install-doc.pl -root $(RELROOT) \
                             -path  $(DOXYPATH) \
                             -stl $(STLURL) \
                             -gral $(RELROOT)/gral/base/doc/hyperlatex/html \
        doxygen/html/*.html)


# assumes that RELPATH=../../..

additional-copydoc:

#	cp $(DOXYPATH)/index.html $(DOCDEST)
#	perl -p -i -e "s|\.\./\.\./\.\.|\.|g" $(DOCDEST)/index.html


export CONFIG:=$(PWD)/mk.config
unexport PWD
include $(CONFIG)


TARGET=all

all:

new: clean


compile:

lib:

DIRS = Base Geometries IO Iterators Subranges Algorithms Algorithms/internal
SHELL=/bin/bash
install-headers:
	for i in $(DIRS);\
        do mkdir -p $(INSTALLHEADERS)/Gral/$$i; \
	cp  include/Gral/$$i/*.{h,C} $(INSTALLHEADERS)/Gral/$$i;\
        done;

clean: stdclean
	(cd doc;  $(MAKE) clean DEPS="")
	find . -name "*~" -o -name "*.dvi" -o -name "*.aux" -o -name "*.log" -exec rm {} \;
	rm -f doxy.tags doxy.log

tarclean: clean
	rm -rf lib/

realclean: tarclean
	rm -rf doxygen/
	(cd doc; gmake realclean)

test:
	(cd test; $(MAKE) $(TARGET) CONFIG=$(CONFIG))



concepts:
	(cd doc; gmake concepts)

doc: doxy concepts



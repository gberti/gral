export CONFIG:=$(PWD)/mk.config
include $(CONFIG)


TARGET=all

all:
	(cd src; $(MAKE) all)

new:
	(cd src; $(MAKE) new)


compile:
	(cd src; $(MAKE) compile)

lib:
	(cd src; $(MAKE) lib)

#install-libs generic

install-headers:
	mkdir -p $(INSTALLHEADERS)/Utility
	mkdir -p $(INSTALLHEADERS)/Forward
	cp include/Utility/*.h $(INSTALLHEADERS)/Utility
	cp include/Forward/*.h $(INSTALLHEADERS)/Forward

install: install-headers install-libs

clean:
	(cd src;  $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")
	rm -rf doxygen/*;
	rm -f *~ include/*~ include/*/*~


realclean: clean
	rm -rf lib/$(ARCH)/


docs:
	mkdir -p doxygen
	doxygen doxy.cfg;


tar:
	(cd $(HOME)/tmp;\
	rm -rf $(PACKAGE)/;\
         cvs -d /home/nmwr/CVS export -r HEAD $(PACKAGE); \
         tar -cf $(PACKAGE).tar $(PACKAGE); \
         rm -rf $(PACKAGE)/;)

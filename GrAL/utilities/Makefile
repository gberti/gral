export CONFIG:=$(PWD)/mk.config

export PACKAGE:=utilities#
export DEBUG
export CCC
#export CCFLAGS
export LIBNAME:=$(PACKAGE)#
export LIBDIR
#export IFLAGS

TARGET=all

all:
	(cd src; $(MAKE) all)

new:
	(cd src; $(MAKE) new)


compile:
	(cd src; $(MAKE) compile)

lib:
	(cd src; $(MAKE) lib)

clean:
	(cd src;  $(MAKE) clean DEPS="")
#	(cd demo; $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")
	rm -rf doxygen/*;
	rm -f *~ include/*~ include/*/*~


realclean: clean
	rm -f lib/*.a

#demos:
#	(cd demo; $(MAKE) $(TARGET))

docs:
	doxygen doxy.cfg;


tar:
	(cd $(HOME)/tmp;\
	rm -rf $(PACKAGE)/;\
         cvs -d /home/nmwr/CVS export -r HEAD $(PACKAGE); \
         tar -cf $(PACKAGE).tar $(PACKAGE); \
         rm -rf $(PACKAGE)/;)

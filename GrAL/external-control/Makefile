CONFIG:=$(PWD)/mk.config

export DEBUG
export CCC
#export CCFLAGS
export IOLIB:=control-device#
export LIBDIR
export CONFIG
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
	(cd demo; $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")

realclean: clean
	rm -f lib/*.a

demos:
	(cd demo; $(MAKE) $(TARGET))

docs:
	(cd doc; $(MAKE) $(TARGET))


tar:
	(cd $(HOME)/tmp;\
	rm -rf external-control/;\
         cvs -d /home/nmwr/CVS export -r HEAD external-control; \
         tar -cf external-control.tar external-control; \
         rm -rf external-control/;)

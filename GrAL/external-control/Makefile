export CONFIG:=$(PWD)/mk.config
include $(CONFIG)

# exporting PDW could give trouble in subdir Makefiles
# which try to give default values to other  variables such as CONFIG
# in order to enable direct make invocation in the subdirectory
unexport PWD 

export LIBDIR

# The name of the library to be generated
export IOLIB:=control-device#

TARGET=all

all:
	(cd src; $(MAKE) all)

new:
	(cd src; $(MAKE) new)


compile:
	(cd src; $(MAKE) compile)

lib:
	(cd src; $(MAKE) lib)

install:
	mkdir -p $(INSTALLHEADERS)/IO
	mkdir -p $(INSTALLLIBRARIES)
	cp include/IO/*.h $(INSTALLHEADERS)/IO
	cp $(LIBDIR)/*.a $(INSTALLLIBRARIES)

clean:
	(cd src;  $(MAKE) clean DEPS="")
	(cd demo; $(MAKE) clean DEPS="")
	(cd doc;  $(MAKE) clean DEPS="")
	rm -f *~ include/*~ include/*/*~

realclean: clean
	rm -f lib/*/

demo:
	(cd demo; $(MAKE) $(TARGET) CONFIG=$(CONFIG))

docs:
	(cd doc; $(MAKE) $(TARGET))


tar:
	(cd $(HOME)/tmp;\
	rm -rf external-control/;\
         cvs -d /home/nmwr/CVS export -r HEAD external-control; \
         tar -cf external-control.tar external-control; \
         rm -rf external-control/;)

.PHONY: demo
include  $(CONFIG)


LOCALSOURCES= control-device.C istream-control-device.C\
              read-parameters.C command-line.C 	skip-comments.C

LOCALOBJECTS= $(LOCALSOURCES:%.C=%.o)


OBJDIR=obj

new: partial-clean all

# use generic makefile for building objects
makefile:
	cp $(CONFIGURATION)/defs/Makefile.obj $(OBJDIR)/Makefile 

all: compile lib

compile: makefile
	(cd $(OBJDIR);\
         gmake compile SRC="$(LOCALSOURCES)")

lib: makefile
	mkdir -p $(LIBDIR)
	(cd $(OBJDIR); \
         gmake lib SRC="$(LOCALSOURCES)" LIBDIR="$(LIBDIR)" LIB="lib$(IOLIB).a")

partial-clean: makefile
	(cd $(OBJDIR); gmake clean DEPS="")


clean: stdclean makefile
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	(cd $(OBJDIR); gmake clean DEPS="")
	rm -rf tempinc/*.o


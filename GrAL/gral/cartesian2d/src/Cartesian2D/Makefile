CONFIG=$(shell pwd)/../../mk.config
include  $(CONFIG)


LOCALSOURCES= cartesian-grid2d.C

LOCALOBJECTS= $(LOCALSOURCES:%.C=%.o)


OBJDIR=obj.tmp

new: partial-clean all

# use generic makefile for building objects
makefile:
	mkdir -p $(OBJDIR)
	cp $(CONFIGURATION)/defs/Makefile.objects $(OBJDIR)/Makefile 

all: compile lib

compile: makefile
	(cd $(OBJDIR);\
         $(MAKE) compile SRC="$(LOCALSOURCES)")

lib: makefile
	mkdir -p $(LIBDIR)
	(cd $(OBJDIR); \
         $(MAKE) lib SRC="$(LOCALSOURCES)" LIBDIR="$(LIBDIR)" LIB="lib$(LIBNAME).a")

partial-clean: makefile
	(cd $(OBJDIR); $(MAKE) clean DEPS="")


clean: stdclean 
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	rm -rf $(OBJDIR)
	rm -rf tempinc/*.o


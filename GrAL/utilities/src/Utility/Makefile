CONFIG=$(shell pwd)/../../mk.config

include  $(CONFIG)


LOCALSOURCES= context.C file-utils.C safe-file.C notifier.C
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


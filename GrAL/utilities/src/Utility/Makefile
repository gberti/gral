include  $(CONFIG)


LOCALSOURCES= safe-file.C
LOCALOBJECTS= $(LOCALSOURCES:%.C=%.o)


OBJDIR=obj

new: partial-clean all

# use generic makefile for building objects
makefile:
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


clean: stdclean makefile
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	(cd $(OBJDIR); $(MAKE) clean DEPS="")
	rm -f $(OBJDIR)/Makefile
	rm -rf tempinc/*.o


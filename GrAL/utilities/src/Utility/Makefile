include  $(CONFIG)


LOCALSOURCES= safe-file.C

LOCALOBJECTS= $(LOCALSOURCES:%.C=%.o)


OBJDIR=obj
TARGETS = local

new: partial-clean all

# use generic makefile for building objects
makefile:
	cp $(NMWRROOT)/defs/Makefile.obj $(OBJDIR)/Makefile 

all: compile lib

compile: makefile
	(cd $(OBJDIR);\
         gmake compile SRC="$(LOCALSOURCES)")

lib: makefile
	(cd $(OBJDIR); \
         gmake lib SRC="$(LOCALSOURCES)" LIB="$(LIBDIR)/lib$(LIBNAME).a")

partial-clean: makefile
	(cd $(OBJDIR); gmake clean DEPS="")


clean: stdclean makefile
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 
	(cd $(OBJDIR); gmake clean DEPS="")
	rm -f $(OBJDIR)/Makefile
	rm -rf tempinc/*.o


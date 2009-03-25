CONFIG=$(shell pwd)/../../local.config
include  $(CONFIG)


TriangleFlexLexer.C : TriangleFlexLexer.l
	flex++ -o$@ $<

LOCALSOURCES= triangle-input.C triangle-generator.C triangle-adapter.C TriangleFlexLexer.C TriangleOptionsParser.C

LOCALOBJECTS= $(LOCALSOURCES:%.C=%.o)

OBJDIR=obj.tmp

new: partial-clean all

# use generic makefile for building objects
makefile:
	mkdir -p $(OBJDIR)
	cp $(CONFIGURATION)/defs/Makefile.objects $(OBJDIR)/Makefile 

all: compile lib



compile: TriangleFlexLexer.C makefile
	(cd $(OBJDIR);\
         $(MAKE) compile SRC="$(LOCALSOURCES)" EXTRA_IFLAGS+=-I$(TRIANGLE_INC) EXTRA_CCFLAGS+="-DLINUX -DANSI_DECLARATORS -DUSER_DATA")

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


CONFIG=$(shell pwd)/../../mk.config
include  $(CONFIG)


LOCALSOURCES= attribute.C \
              renderable-geom.C \
              rendering-language.C \
              graphics-device.C \
              transformation.C \
              calc-transformations.C \
              rgb-color.C \
              oogl.C \
              simplex.C\
              ball.C \
              sphere.C \
              quadrilateral.C \
              torus.C \
              circle.C \
              cone.C \
              triangle.C \
              cube.C \
              cylinder.C \
              segment.C \
              word.C \
              letter.C 

#              xfig.C \


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
	rm -rf  $(OBJDIR)
	rm -rf tempinc/*.o


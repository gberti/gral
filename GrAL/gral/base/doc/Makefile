CONFIG=../mk.config

include $(CONFIG)

all: concepts install-doc

gen-paths:
	./gen-tex-paths.pl  > hyperlatex/paths.tex



install-doc1:
	./install-doc.pl -stl $(STLURL) \
         hyperlatex/html/*.html


install-doc: install-doc1  make-links
concepts: gen-paths latex

latex:
	(cd hyperlatex;\
           rm -f html/*; \
          (cd bilder; $(MAKE) all; $(MAKE) clean);\
          hyperlatex-2.3.1 top;\
          cp not-yet-done.html html/)

make-links:
	(cd hyperlatex;\
          cp *.xbm html/;\
          (cd html; \
           ln -s ../gralconcepts.css .; \
           rm -f bilder; ln -s ../bilder ./; \
          ln -s top.html index.html;\
          ln -s GridVertex.html  Vertex.html;\
          ln -s GridEdge.html    Edge.html;\
          ln -s GridFace.html    Face.html;\
          ln -s GridFacet.html   Facet.html;\
          ln -s GridCell.html    Cell.html;\
          ln -s GridVertexIterator.html VertexIterator.html; \
          ln -s GridVertexIterator.html EdgeIterator.html; \
          ln -s GridVertexIterator.html FacetIterator.html; \
          ln -s GridVertexIterator.html CellIterator.html; \
          ln -s VertexGridRange.html VertexRange.html;\
          ln -s VertexGridRange.html EdgeRange.html;\
          ln -s VertexGridRange.html FacetRange.html;\
          ln -s VertexGridRange.html CellRange.html;\
          ln -s VertexGridRange.html CellGridRange.html;\
          ln -s VertexGridRange.html FacetGridRange.html;\
          ln -s VertexGridRange.html EdgeGridRange.html;\
          ln -s Vertex-On-CellIterator.html VertexOnCellIterator.html; \
          ln -s Vertex-On-CellIterator.html EdgeOnCellIterator.html; \
          ln -s Vertex-On-CellIterator.html VertexOnFacetIterator.html; \
          ln -s Vertex-On-CellIterator.html FacetOnCellIterator.html; \
          ln -s Cell-On-CellIterator.html   CellOnCellIterator.html; \
          ln -s GridIncidenceIterator.html  IncidenceIterator.html; \
          ln -s GridSequenceIterator.html   SequenceIterator.html; \
          )) || echo "(ignored)"


clean: stdclean
	rm -f *.dvi *.log *.aux *~ 
	(cd hyperlatex; $(MAKE) clean)

realclean: clean
	rm -rf hyperlatex/html
	(cd hyperlatex; $(MAKE) realclean)



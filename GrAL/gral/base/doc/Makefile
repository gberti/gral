CONFIG=../mk.config

include $(CONFIG)

gen-paths:
	./gen-tex-paths.pl  > hyperlatex/paths.tex



install-doc:
	./install-doc.pl -stl $(STLURL) -gralroot "../../.."  hyperlatex/html/*.html

concepts: gen-paths latex make-links

latex:
	(cd hyperlatex;\
           rm -f html/*; \
          (cd bilder; gmake all; gmake clean);\
          hyperlatex-2.3.1 top;)

make-links:
	(cd hyperlatex;\
          cp $(HOME)/WWW/icons/*.xbm html/;\
          (cd html; rm -f bilder; ln -s ../bilder ./; \
          ln -s top.html index.html;\
          ln -s GridVertex.html  Vertex.html;\
          ln -s GridEdge.html   Edge.html;\
          ln -s GridFacet.html  Facet.html;\
          ln -s GridCell.html   Cell.html;\
          ln -s VertexGridRange.html VertexRange.html;\
          ln -s VertexGridRange.html EdgeRange.html;\
          ln -s VertexGridRange.html FacetRange.html;\
          ln -s VertexGridRange.html CellRange.html;\
          ln -s Vertex-On-CellIterator.html VertexOnCellIterator.html; \
          ln -s Vertex-On-CellIterator.html EdgeOnCellIterator.html; \
          ln -s Vertex-On-CellIterator.html FacetOnCellIterator.html; \
          ))


clean: stdclean
	rm -f *.dvi *.log *.aux *~ 
	rm -f hyperlatex/paths.tex

realclean: clean
	rm -rf hyperlatex/html




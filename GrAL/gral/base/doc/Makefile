CONFIG=../mk.config

include $(CONFIG)


concepts:
	(cd hyperlatex;\
          (cd bilder; gmake all; gmake clean);\
          hyperlatex-2.3.1 top; \
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
          ))

clean:
	rm -f *.dvi *.log *.aux *~ 

realclean: clean
	rm -rf hyperlatex/html




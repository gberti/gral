LATEX = latex

all : pdf 

%.eps : %.fig
	fig2dev -L eps $< > $@

%.pdf : %.ps
	ps2pdf $<

FIGS = gps.eps toolspyramid.eps

gps.eps : gps.fig
toolspyramid.eps : toolspyramid.fig

pdf : gral_roadmap.pdf

gral_roadmap.ps : gral_roadmap.dvi
	dvips $<

gral_roadmap.dvi :  gral_roadmap.tex $(FIGS)
	$(LATEX) $<
	$(LATEX) $<
	$(LATEX) $<


.PHONY : clean distclean

clean : 
	rm -f *.dvi *.aux *.toc *.log gral_roadmap.ps

distclean : clean
	rm -f gral_roadmap.pdf $(FIGS)

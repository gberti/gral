

PSTRICKS=$(shell ls *.tex)
EPSFIGS:=$(PSTRICKS:%.tex=%.ps)
GIFFILES=$(PSTRICKS:%.tex=%.gif)
JPGFILES=$(PSTRICKS:%.tex=%.jpeg)


CONVERT = convert
#CONVERT = pstogif

.SUFFIXES: .gif .eps .ps .tex .jpeg

.tex.eps:
	latex $<  && dvips -Pamz -Pcmz -E ${<:%.tex=%.dvi} -o $@

.tex.ps:
	latex $<  && dvips -Pamz -Pcmz -E ${<:%.tex=%.dvi} -o $@

#.eps.gif:
#	$(CONVERT) $< $@

.eps.gif:
	$(CONVERT) $< $@
	mv ${@}.0 $@
	rm -r ${@}.1

.eps.jpeg:
	$(CONVERT) $< $@
	mv ${@}.0 $@
	rm -r ${@}.1

# ?????
cell-on-cell.jpeg: cell-on-cell.ps
	pstogif  cell-on-cell.ps cell-on-cell.gif
	convert  cell-on-cell.gif cell-on-cell.jpeg

cell-on-cell.eps: stop-sign.eps
cell-on-cell.ps: stop-sign.eps

all: stop-sign.eps $(JPGFILES)

clean:
	rm -f *.eps *.ps *.aux *.log *.dvi *~ 

realclean: clean
	rm -f $(JPGFILES) $(GIFFILES)

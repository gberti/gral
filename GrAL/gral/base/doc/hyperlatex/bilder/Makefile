

PSTRICKS=$(shell ls *.tex)
EPSFIGS:=$(PSTRICKS:%.tex=%.ps)
GIFFILES=$(PSTRICKS:%.tex=%.gif)


#CONVERT = convert
CONVERT = pstogif

.SUFFIXES: .gif .eps .ps .tex

.tex.eps:
	latex $<  && dvips -Pamz -Pcmz -E ${<:%.tex=%.dvi} -o $@

.tex.ps:
	latex $<  && dvips -Pamz -Pcmz -E ${<:%.tex=%.dvi} -o $@

#.eps.gif:
#	$(CONVERT) $< $@

.ps.gif:
	$(CONVERT) $< $@


all: stop-sign.eps $(GIFFILES)

clean:
	rm -f *.eps *.ps *.aux *.log *.dvi

realclean: clean
	rm -f *.gif

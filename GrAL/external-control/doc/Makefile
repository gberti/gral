CONFIG=../mk.config

include $(CONFIG)

all: html dvi

html:
	hyperlatex-2.3.1 external-control.tex

dvi:
	latex  external-control.tex

clean:
	rm -f *.dvi *.log *.aux 
	rm -f html_dir/*.html
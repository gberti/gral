CONFIG=../mk.config

include $(CONFIG)

all: hmtl dvi

html:
	hyperlatex external-control.tex

dvi:
	latex  external-control.tex

clean:
	rm -f *.dvi *.log *.aux 
	rm -f html_dir/*.html
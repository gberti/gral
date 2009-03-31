CONFIG=../local.config

include $(CONFIG)

all: html dvi

html:
	hyperlatex external-control.tex

postdoc-local:
	./install-doc.pl -gralroot $(RELROOT) -stl $(STLURL) html_dir/external-control.html

dvi:
	latex  external-control.tex

clean:
	rm -f *.dvi *.log *.aux 
	rm -f html_dir/*.html
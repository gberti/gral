CONFIG=$(shell pwd)/../../mk.config

include $(CONFIG)


clean: 
	rm -f paths.tex
	rm -f *.aux *.log *.dvi
	(cd bilder; $(MAKE) clean)

realclean: clean
	(cd bilder; $(MAKE) realclean)
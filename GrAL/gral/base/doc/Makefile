CONFIG=../mk.config

include $(CONFIG)


concepts:
	(cd hyperlatex; hyperlatex-2.3.1 top; \
         cp *.xbm html/;\
         (cd html; rm -f bilder; ln -s ../bilder .))

clean:
	rm -f *.dvi *.log *.aux *~ 

realclean: clean
	rm -rf hyperlatex/html




export CONFIG:=$(shell pwd)/mk.config
unexport PWD
include $(CONFIG)



install-headers:
	mkdir -p $(INSTALLHEADERS)/Config
	cp include/Config/*.h $(INSTALLHEADERS)/Config


new: libclean

all:

test:

check:

doc:
	mkdir -p doxygen/html
	echo "# dummy file" > doxygen/configuration.tags

demo:

clean: stdclean

realclean: clean
	rm -rf doxygen

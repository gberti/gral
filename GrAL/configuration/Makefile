export CONFIG:=$(shell pwd)/mk.config
unexport PWD
include $(CONFIG)



install-headers: std-install-headers

install-config:
	mkdir -p $(PREFIX)/configuration
	cp -r defs $(PREFIX)/configuration/

install: install-headers install-config

new: libclean

all:

test:

check:

test-check-clean:

doc:
	mkdir -p doxygen/html
	echo "# dummy file" > doxygen/configuration.tags

demo:

clean: stdclean

realclean: clean
	rm -rf doxygen

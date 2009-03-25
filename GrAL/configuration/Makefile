export CONFIG:=$(shell pwd)/local.config
unexport PWD
include $(CONFIG)


all:

install-headers: std-install-headers

install-config:
	mkdir -p $(PREFIX)/configuration
	cp -r defs $(PREFIX)/configuration/

install: install-headers install-config

new: libclean


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

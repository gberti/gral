
GRALROOT=$(shell pwd)
SUBDIRS= configuration utilities external-control sequence geometry \
         graphics-device function-algebra gral


new:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) new GRALROOT=$(GRALROOT) DEVELOP=develop CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)


clean:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) clean GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;

realclean:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) realclean GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;

all:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) all GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)


install:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) install GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)


test:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) test GRALROOT=$(GRALROOT) TARGET=new DEVELOP=user CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

check:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) check GRALROOT=$(GRALROOT) DEVELOP=user DEPS='' CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

test-check-clean:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) test-check-clean GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)



# predoc  = generate docs
# postdoc = install docs (replace dummy links)

doxy:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) doxy GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

installdox:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) installdox GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

doc: predoc postdoc

predoc:	
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) predoc GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

postdoc:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) postdoc GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

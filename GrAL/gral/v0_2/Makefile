
ALLROOT=$(shell pwd)
SUBDIRS= configuration utilities external-control sequence geometry \
         graphics-device function-algebra gral


new:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) new ALLROOT=$(ALLROOT) DEVELOP=develop CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)


clean:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) clean ALLROOT=$(ALLROOT) CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;

realclean:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) realclean ALLROOT=$(ALLROOT) CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;

all:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) all ALLROOT=$(ALLROOT) CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)


install:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) install ALLROOT=$(ALLROOT) CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)


test:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) test ALLROOT=$(ALLROOT) TARGET=new DEVELOP=user CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

check:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) check ALLROOT=$(ALLROOT) DEVELOP=user DEPS='' CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

test-check-clean:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) test-check-clean ALLROOT=$(ALLROOT) CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)



# predoc  = generate docs
# postdoc = install docs (replace dummy links)

doxy:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) doxy ALLROOT=$(ALLROOT) CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

installdox:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) installdox ALLROOT=$(ALLROOT) CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

doc: predoc postdoc

predoc:	
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) predoc ALLROOT=$(ALLROOT) CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

postdoc:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(ALLROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) postdoc ALLROOT=$(ALLROOT) CONFIG=$(ALLROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

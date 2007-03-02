
GRALROOT=$(shell pwd)
SUBDIRS= configuration utilities external-control sequence geometry \
         graphics-device function-algebra gral

SUBDIRS := $(filter-out $(EXCLUDED),$(SUBDIRS))

TARGET=new

all:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) all GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

clean:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) clean GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;

libclean:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) libclean GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;


realclean:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) realclean GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)
	rm -f *.o *~ *.flc core;


new:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) new GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)
new-force:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) new GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config \
        || echo ">>>>>>>>>>> TARGET new-force FAILED for " $$i " (continuing)";\
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
        $(MAKE) test TARGET=$(TARGET) GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)
test-force:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) test TARGET=$(TARGET) GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config \
        || echo ">>>>>>>>>>> TARGET test-force FAILED for " $$i " (continuing)";\
        echo "LEAVING  directory" $$i;cd ..; done)

check:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) check GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config;\
        echo "LEAVING  directory" $$i;cd ..; done)

check-force:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) check GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config \
        || echo ">>>>>>>>>>> TARGET check-force FAILED for " $$i " (continuing)";\
        echo "LEAVING  directory" $$i;cd ..; done)

new-check-clean: new test-check-clean

test-check-clean:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) test-check-clean GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config; \
        echo "LEAVING  directory" $$i;cd ..; done)

test-check-clean-force:
	@set -e; \
        (for i in $(SUBDIRS); \
        do cd $(GRALROOT)/$$i;echo 'ENTERING directory' $$i;\
        $(MAKE) test-check-clean GRALROOT=$(GRALROOT) CONFIG=$(GRALROOT)/$$i/mk.config \
        || echo ">>>>>>>>>>> TARGET test-check-clean-force FAILED for " $$i " (continuing)";\
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

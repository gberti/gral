CONFIG=$(shell pwd)/../mk.config
include $(CONFIG)

ifeq ($(WITHMETIS),true)
METISDIR = Metis
endif

SUBDIRS += $(METISDIR)


include  $(RECURSIVETARGETS)





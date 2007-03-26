PWD=$(shell pwd)
CONFIG=$(PWD)/../mk.config

ifeq ($(WITHMETIS),true)
METISDIR  = metis
SUBDIRS  += $(METISDIR)
endif



include  $(CONFIG)
include  $(RECURSIVETARGETS)




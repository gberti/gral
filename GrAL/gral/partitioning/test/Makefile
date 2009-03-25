PWD=$(shell pwd)
CONFIG=$(PWD)/../local.config

ifeq ($(WITHMETIS),true)
METISDIR  = metis
SUBDIRS  += $(METISDIR)
endif



include  $(CONFIG)
include  $(RECURSIVETARGETS)




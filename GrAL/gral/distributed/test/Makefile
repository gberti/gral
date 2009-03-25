PWD=$(shell pwd)
CONFIG=$(PWD)/../local.config

include  $(CONFIG)

SUBDIRS = composite 

ifeq ($(WITHMPI),true)
SUBDIRS += mpi
endif




include  $(RECURSIVETARGETS)








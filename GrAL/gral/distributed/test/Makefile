PWD=$(shell pwd)
CONFIG=$(PWD)/../mk.config

include  $(CONFIG)

SUBDIRS = composite 

ifeq ($(WITHMPI),true)
SUBDIRS += mpi
endif




include  $(RECURSIVETARGETS)








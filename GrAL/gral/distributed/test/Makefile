SUBDIRS = composite 

ifdef WITHMPI
SUBDIRS += mpi
endif


PWD=$(shell pwd)
CONFIG=$(PWD)/../mk.config

include  $(CONFIG)
include  $(RECURSIVETARGETS)




PWD=$(shell pwd)
CONFIG=$(PWD)/../mk.config

include  $(CONFIG)

SUBDIRS = composite 

ifdef WITHMPI
SUBDIRS += mpi
endif




include  $(RECURSIVETARGETS)








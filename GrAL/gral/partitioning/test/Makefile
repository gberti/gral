PWD=$(shell pwd)
CONFIG=$(PWD)/../mk.config

ifdef WITHMETIS
METISDIR  = metis
SUBDIRS  += $(METISDIR)
endif



include  $(CONFIG)
include  $(RECURSIVETARGETS)




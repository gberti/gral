
PWD=$(shell pwd)
CONFIG=$(PWD)/../local.config

ifeq ($(WITHVTK),true)
SUBDIRS = construct
endif

include  $(CONFIG)
include  $(RECURSIVETARGETS)





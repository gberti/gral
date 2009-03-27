
PWD=$(shell pwd)
CONFIG=$(PWD)/../local.config

ifeq ($(WITHCGNS),true)
SUBDIRS = write
endif


include  $(CONFIG)
include  $(RECURSIVETARGETS)





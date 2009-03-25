CONFIG=$(shell pwd)/../local.config

include  $(CONFIG)


ifeq ($(WITHVTK),true)
VTKDIR = VTK
SUBDIRS += $(VTKDIR)
endif


include  $(RECURSIVETARGETS)





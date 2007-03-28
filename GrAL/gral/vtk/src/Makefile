CONFIG=$(shell pwd)/../mk.config

include  $(CONFIG)

ifdef WITHVTK
VTKDIR = VTK
SUBDIRS += $(VTKDIR)
endif


include  $(RECURSIVETARGETS)





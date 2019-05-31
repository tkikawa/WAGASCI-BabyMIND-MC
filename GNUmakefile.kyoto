###
CPPFLAGS += -Df2cFortran

###
CERN_ROOT = /cern/pro
CERNINC = $(CERN_ROOT)/include
CERNLIB = -L$(CERN_ROOT)/lib 
CERNLIBS = -lmathlib -lpacklib -lgraflib -lgrafX11 -lpacklib -lkernlib -lmathlib
CPPFLAGS += -I$(CERNINC)
EXTRALIBS += $(CERNLIBS)

###
MYTEMP = -L/usr/X11R6/lib -L/usr/lib/gcc/x86_64-redhat-linux/3.4.6/ -lg2c
EXTRALIBS += $(MYTEMP)

DATALIBDIR = $(G4WORKDIR)/lib
CPPFLAGS += -I$(DATALIBDIR)

MYLIB = -lm $(DATALIBDIR)/B2EVENTSUMMARY.so $(DATALIBDIR)/B2HitSummary.so $(DATALIBDIR)/B2SimHitSummary.so $(DATALIBDIR)/B2SimVertexSummary.so $(DATALIBDIR)/B2SimParticleSummary.so $(DATALIBDIR)/BeamInfoSummary.so $(DATALIBDIR)/B2BasicReconSummary.so $(DATALIBDIR)/B2TrackSummary.so $(DATALIBDIR)/NeutInfoSummary.so $(DATALIBDIR)/B2ReconSummary.so $(DATALIBDIR)/B2AnaSummary.so
EXTRALIBS += -L$(MYLIBDIR) $(MYLIB)

###
ROOTCINT=YES
ROOTLIBS =  $(shell $(ROOTSYS)/bin/root-config --glibs)
CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
EXTRALIBS += $(ROOTLIBS)

###
name := B2MC
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: bin

include $(G4INSTALL)/config/binmake.gmk

LDFLAGS += $(CERNLIB) 

CXXFLAGS_WITHOUT_O := $(filter-out -O% , $(CXXFLAGS)) 
CXXFLAGS_WITHOUT_O := $(filter-out +O% , $(CXXFLAGS_WITHOUT_O))

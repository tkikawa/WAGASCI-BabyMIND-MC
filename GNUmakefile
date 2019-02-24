### CPPFLAGS
CPPFLAGS += -Df2cFortran

### CERNLIB
CERN_ROOT = /cern/pro
CERNINC = $(CERN_ROOT)/include
CERNLIB = -L$(USER)/cernlib-2006
CERNLIBS = -lmathlib -lpacklib -lgraflib -lgrafX11 -lpacklib -lkernlib -lmathlib
CPPFLAGS += -I$(CERNINC)
EXTRALIBS += $(CERNLIBS) $(shell geant4-config --libs) -L./lib

### GEANT4
CPPFLAGS += $(shell geant4-config --cflags)
G4WORKDIR = .

### OTHER LIBS
# EXTRALIBS += -L$(USER)/gcc-7.3.0/lib64 -lg2c -lm
EXTRALIBS += -L/opt/rh/devtoolset-6/root/usr/lib/gcc/x86_64-redhat-linux/6.3.1 -lg2c -lm

### MY INCS
DATALIBDIR = $(G4WORKDIR)/lib
CPPFLAGS += -I$(DATALIBDIR)

### MY LIBS
MYLIB = $(DATALIBDIR)/B2EVENTSUMMARY.so $(DATALIBDIR)/B2HitSummary.so $(DATALIBDIR)/B2SimHitSummary.so $(DATALIBDIR)/B2SimVertexSummary.so $(DATALIBDIR)/B2SimParticleSummary.so $(DATALIBDIR)/BeamInfoSummary.so $(DATALIBDIR)/B2BasicReconSummary.so $(DATALIBDIR)/B2TrackSummary.so $(DATALIBDIR)/NeutInfoSummary.so $(DATALIBDIR)/B2ReconSummary.so $(DATALIBDIR)/B2AnaSummary.so
EXTRALIBS += $(MYLIB)

### ROOT
ROOTCINT=YES
ROOTLIBS =  $(shell $(ROOTSYS)/bin/root-config --glibs)
CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
EXTRALIBS += $(ROOTLIBS)

############ MAKE ALL ############
name := B2MC
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: bin

include $(G4INSTALL)/config/binmake.gmk

LDFLAGS += $(CERNLIB) 

CXXFLAGS_WITHOUT_O := $(filter-out -O% , $(CXXFLAGS)) 
CXXFLAGS_WITHOUT_O := $(filter-out +O% , $(CXXFLAGS_WITHOUT_O))

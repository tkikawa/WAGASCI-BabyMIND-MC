### CPPFLAGS
CPPFLAGS += -Df2cFortran

### CERNLIB
ifeq ($(CERN_ROOT),)
CERN_ROOT = $(USER)/cernlib-2006
endif
CERNINC = $(CERN_ROOT)/include
CERNLIB = -L$(CERN_ROOT)/lib
CPPFLAGS += -I$(CERNINC)
LDFLAGS += $(CERNLIB)
CERNLIBS = -lmathlib -lpacklib -lgraflib -lgrafX11 -lkernlib -lgfortran
EXTRALIBS += $(CERNLIBS)

### GEANT4
CPPFLAGS += $(shell geant4-config --cflags)
EXTRALIBS += $(shell geant4-config --libs)
G4WORKDIR = .

### OTHER LIBS
EXTRALIBS += -L./lib -lg2c -lm -Wl,-rpath=/opt/python-2.7/lib

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
all: lib bin B2MC

include $(G4INSTALL)/config/binmake.gmk

clean::
	rm -f B2MC
	$(MAKE) clean -C lib

lib:
	$(MAKE) -C lib

CXXFLAGS_WITHOUT_O := $(filter-out -O% , $(CXXFLAGS)) 
CXXFLAGS_WITHOUT_O := $(filter-out +O% , $(CXXFLAGS_WITHOUT_O))

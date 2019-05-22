### CPPFLAGS
CPPFLAGS += -Df2cFortran

### GEANT4
CPPFLAGS += $(shell geant4-config --cflags)
EXTRALIBS += $(shell geant4-config --libs)
G4WORKDIR = .

### MY INCS
DATALIBDIR = $(G4WORKDIR)/lib
CPPFLAGS += -I$(DATALIBDIR)

### CERNLIB
ifeq ($(CERN_ROOT),)
CERN_ROOT = $(USER)/cernlib
endif
CERNINC_PATH = $(CERN_ROOT)/include
CERNLIBS_PATH = $(CERN_ROOT)/lib
CPPFLAGS += -I$(CERNINC_PATH)
LDFLAGS += -L$(CERNLIBS_PATH)
CERNLIBS = -Wl,-rpath=$(CERNLIBS_PATH) -lmathlib -lpacklib -lgraflib -lgrafX11 -lkernlib -lgfortran
EXTRALIBS += $(CERNLIBS)

### OTHER LIBS
EXTRALIBS += -L./lib -lg2c -lm -Wl,-rpath=/opt/python-2.7/lib

### MY LIBS
MYLIBS  = $(DATALIBDIR)/B2EVENTSUMMARY.so $(DATALIBDIR)/B2HitSummary.so $(DATALIBDIR)/B2SimHitSummary.so
MYLIBS += $(DATALIBDIR)/B2SimVertexSummary.so $(DATALIBDIR)/B2SimParticleSummary.so $(DATALIBDIR)/BeamInfoSummary.so
MYLIBS += $(DATALIBDIR)/B2BasicReconSummary.so $(DATALIBDIR)/B2TrackSummary.so $(DATALIBDIR)/NeutInfoSummary.so
MYLIBS += $(DATALIBDIR)/B2ReconSummary.so $(DATALIBDIR)/B2AnaSummary.so
EXTRALIBS += $(MYLIBS)

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

### CPPFLAGS
CPPFLAGS += -Df2cFortran

### CERNLIB
ifeq ($(CERN_ROOT),)
CERN_ROOT := CERN_ROOT=$(USER)/cernlib-2006
CERNINC = $(CERN_ROOT)/include
CERNLIB = -L$(CERN_ROOT)/lib
CPPFLAGS += -I$(CERNINC)
LDFLAGS += $(CERNLIB)
endif
CERNLIBS = -lmathlib -lpacklib -lgraflib -lgrafX11 -lkernlib -lgfortran
EXTRALIBS += $(CERNLIBS)

### GEANT4
CPPFLAGS += $(shell geant4-config --cflags)
EXTRALIBS += $(shell geant4-config --libs)
G4WORKDIR = .

### OTHER LIBS
GCCVERSION := $(shell gcc -dumpversion)
# this is SL6 gcc 7
ifeq "$(GCCVERSION)" "7.3.0"
EXTRALIBS += -L./lib -L$(USER)/gcc-7.3.0/lib64 -lg2c -lm
# this is Ubuntu gcc 7
else ifeq "$(GCCVERSION)" "7"
EXTRALIBS += -L./lib -lg2c -lm
# this is SL6 gcc 6
else ifeq "$(GCCVERSION)" "6.3.1"
EXTRALIBS +=  -L./lib -L/opt/rh/devtoolset-6/root/usr/lib/gcc/x86_64-redhat-linux/6.3.1 -lg2c -lm
else
$(warning GCC version not recognized )
endif

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

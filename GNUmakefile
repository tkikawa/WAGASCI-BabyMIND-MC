### Compiler version
CPP_VERSION := $(shell expr `g++ -dumpversion | cut -f1 -d.` \>= 5)
ifeq "$(CPP_VERSION)" "1"
CPPFLAGS += -no-pie
LDFLAGS += -no-pie
endif

### GEANT4
LDFLAGS += -L$(shell geant4-config --prefix)/lib
G4WORKDIR = .
DATALIBDIR = $(G4WORKDIR)/lib
CPPFLAGS += -I$(DATALIBDIR)
# Comment next line if you are not debugging the compilation flags
CPPVERBOSE = 1

### CERNLIB
ifneq ($(CERN_ROOT),)
CERNINC_PATH = $(CERN_ROOT)/include
CERNLIBS_PATH = $(CERN_ROOT)/lib
CPPFLAGS += -I$(CERNINC_PATH)
LDFLAGS += -L$(CERNLIBS_PATH) -Wl,-rpath=$(CERNLIBS_PATH)
endif
EXTRALIBS += -l:libgraflib.a -l:libmathlib.a -l:libgrafX11.a -l:libpacklib.a -l:libkernlib.a -l:liblapack.a -l:libgfortran.a -l:libquadmath.a

### OTHER LIBS
LDFLAGS += -Wl,-rpath=/opt/python-2.7/lib
EXTRALIBS += -lg2c -lX11 -lnsl -lcrypt -ldl -lm 

### ROOT
ROOTCINT=YES
ROOTLIBS =  $(shell $(ROOTSYS)/bin/root-config --glibs)
CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
EXTRALIBS += $(ROOTLIBS)

### MY LIBS
LDFLAGS += -L./lib
MYLIBS  = $(DATALIBDIR)/B2EVENTSUMMARY.so $(DATALIBDIR)/B2HitSummary.so $(DATALIBDIR)/B2SimHitSummary.so
MYLIBS += $(DATALIBDIR)/B2SimVertexSummary.so $(DATALIBDIR)/B2SimParticleSummary.so $(DATALIBDIR)/BeamInfoSummary.so
MYLIBS += $(DATALIBDIR)/B2BasicReconSummary.so $(DATALIBDIR)/B2TrackSummary.so $(DATALIBDIR)/NeutInfoSummary.so
MYLIBS += $(DATALIBDIR)/B2ReconSummary.so $(DATALIBDIR)/B2AnaSummary.so
EXTRALIBS += $(MYLIBS)

### MY CPPFLAGS
CPPFLAGS += -g -O0 -Df2cFortran -Wno-write-strings -Wno-overloaded-virtual -Wno-unused-but-set-variable -Wno-unused-variable -Wno-unused-parameter

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

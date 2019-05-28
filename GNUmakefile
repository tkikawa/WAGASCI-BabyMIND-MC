name = B2MC

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
G4VIS_USE_OPENGLX = YES
G4EXLIB = YES
G4TARGET = $(name)
# Comment next line if you are not debugging the compilation flags
CPPVERBOSE = YES

### CERNLIB
ifneq ($(CERN_ROOT),)
CERNINC_PATH = $(CERN_ROOT)/include
CERNLIBS_PATH = $(CERN_ROOT)/lib
CPPFLAGS += -I$(CERNINC_PATH)
LDFLAGS += -L$(CERNLIBS_PATH) -Wl,-rpath=$(CERNLIBS_PATH)
endif
CERNLIBS := $(shell cernlib packlib kernlib)
EXTRALIBS += $(CERNLIBS)

### OTHER LIBS
LDFLAGS += -Wl,-rpath=/opt/python-2.7/lib
EXTRALIBS += -lg2c

### ROOT
ROOTCINT = YES
ROOTLIBS =  $(shell $(ROOTSYS)/bin/root-config --glibs)
CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
EXTRALIBS += $(ROOTLIBS)

### MY LIBS
LDFLAGS += -L./lib
EXTRALIBS += $(DATALIBDIR)/B2EVENTSUMMARY.so $(DATALIBDIR)/B2HitSummary.so $(DATALIBDIR)/B2SimHitSummary.so
EXTRALIBS += $(DATALIBDIR)/B2SimVertexSummary.so $(DATALIBDIR)/B2SimParticleSummary.so $(DATALIBDIR)/BeamInfoSummary.so
EXTRALIBS += $(DATALIBDIR)/B2BasicReconSummary.so $(DATALIBDIR)/B2TrackSummary.so $(DATALIBDIR)/NeutInfoSummary.so
EXTRALIBS += $(DATALIBDIR)/B2ReconSummary.so $(DATALIBDIR)/B2AnaSummary.so

### MY CPPFLAGS
CPPFLAGS += -g -O0 -Df2cFortran -Wno-write-strings -Wno-overloaded-virtual -Wno-unused-but-set-variable -Wno-unused-variable -Wno-unused-parameter

############ MAKE ALL ############
.PHONY: all
all: lib link

include $(G4INSTALL)/config/binmake.gmk

link: $(G4TMPDIR)/exe/$(G4TARGET).o $(G4TMPDIR)/Neut.o
	@echo Linking $(G4TARGET)
	$(CXX) $(CPPFLAGS) -o $(G4TARGET) $^ -Wl,-rpath=$(G4TMPDIR)/$(G4TARGET) $(LDFLAGS) $(LDLIBS)

clean::
	rm -f B2MC
	$(MAKE) clean -C lib

lib:
	$(MAKE) -C lib

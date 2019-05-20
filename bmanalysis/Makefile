TARGET = mest

ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)
ROOTGLIBS = $(shell root-config --glibs)

CXXFLAGS = $(ROOTFLAGS) -Wall -fPIC
CXXLIBS	= $(ROOTLIBS)
CC = g++

#WORKDIR = /Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis
WORKDIR = ./

#LIB = /Users/jinaren/Documents/T2K/BabyMIND/commissioning_run/event_estimation/macro/Myanalysis/lib

LIBS            = $(ROOTGLIBS) \
          -lm $(LIB)/B2EVENTSUMMARY.so $(LIB)/B2HitSummary.so \
          $(LIB)/B2SimHitSummary.so $(LIB)/B2SimVertexSummary.so \
          $(LIB)/B2SimParticleSummary.so $(LIB)/BeamInfoSummary.so \
          $(LIB)/B2BasicReconSummary.so $(LIB)/B2TrackSummary.so \
          $(LIB)/B2ReconSummary.so $(LIB)/B2AnaSummary.so \
          $(LIB)/NeutInfoSummary.so \

OBJDIR = $(WORKDIR)/obj
SRCDIR = $(WORKDIR)/src
OUTDIR = $(WORKDIR)/Analysis
INCDIR = -I$(WORKDIR)/include 

PROG := $(OUTDIR)/$(TARGET)

all: $(PROG)

SRCS := $(TARGET).cc
OBJS  = $(addprefix $(SRCDIR)/, $(SRCS:.cc=.o))

$(PROG): $(OBJS)
	$(CC) $(CXXLIBS) $(OBJS) -o $@

$(SRCDIR)/%.o : $(SRCDIR)/%.cc
	@[ -d $(OBJDIR) ]
	$(CC) $(CXXFLAGS) $(INCDIR) -o $@ -c $<






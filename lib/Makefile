ROOTINC = -I`root-config --incdir`
CLASSDIR = .
CLASSINC= -I${CLASSDIR}
INCDIRS = ${ROOTINC} ${CLASSINC}

CXXFLAGS  = `root-config --cflags`
COPTFLAGS = -g

SOBJS = NeutInfoSummary.so B2EVENTSUMMARY.so B2HitSummary.so B2SimHitSummary.so B2SimVertexSummary.so B2SimParticleSummary.so BeamInfoSummary.so B2BasicReconSummary.so B2TrackSummary.so B2ReconSummary.so B2AnaSummary.so

OBJS  =  ${SOBJS} 

.SUFFIXES:      .so

GENROOTSO = env COPTFLAGS="${COPTFLAGS}" INCDIRS="${INCDIRS}" \
                        ./gen_root_so.sh


.cc.o:
	$(CXX) -c $(COPTFLAGS) $(CXXFLAGS) $(INCDIRS) -o $@ $<

.cc.so:
	$(GENROOTSO) $(basename $<)

.F.o:
	$(FC) -c $(FOPTFLAGS) $(FINCDIRS) -o $@ $<

all: $(OBJS)

clean:
	$(RM) -f *.o *~ *LinkDef.h *Dict.* ${OBJS} chkfort

#ifndef B2RECONSUMMARY_H
#define B2RECONSUMMARY_H
#include <iostream>
#include "TObject.h"
#include "TClonesArray.h"

#include "TRefArray.h"
#include "TRef.h"
#include "vector"
using namespace std;

#include "B2HitSummary.h"
#include "B2TrackSummary.h"
#define B2HIT_MAXHITS 1000  //## Temporary
#define B2RECON_MAXTRACKS 10 //## Temporary

//......................................................................
class B2ReconSummary : public TObject{
public:

    int       view;      // view (tate or yoko)
    float     clstime;      // time of cluster defined by most high p.e. channel's time
    //float     CTtimecorr;   // i'th beam bunch timing at CT - 581*i 
    float     clstimecorr;  // time after correction by measurement time of CT
                            // ; clstime - ( CTtime - expectecdCTtime )
    //float     exptime;      // difference from expected beam time
                            // ; ( clstimecorr - TDCoffset )%GapTimebwBunch - expected(200nsec)
    //int       nhitclster;   // # of hits in clster
    int       nactpln;      // number of active planes
    //int       actinarow;    // active plane's bit
    float     layerpe;      // number of p.e. / active layer
    //bool      upstreamVETO; // 
    //bool      upstreamedge; //

    //bool      newfid;       // 2010/5/12 new fiducial volume
                            // x, x, (4 ~ 19), (2 ~ 21), (1 ~ 22)...
    //bool      newfidcosmic; // x, x, (4 ~ 19), (2 ~ 21), (1 ~ 22)...(1 ~ 22), (2 ~ 21), (4 ~ 19), x, x

    //bool      vinternal;    //
    int       hitmod;       //
    int       hitcyc;       //
    bool      spill_flag;   //
    bool      bunch_flag;   //
    bool      ontime;       //
    //int       trgbit;
    //int       inarowbit;
    float     vetodist;
    bool      vetowtracking; // Upstream VETO
    bool      edgewtracking; // Fiducial CUT
    bool      hastrk;        // Tracking succsess
    bool      matchtrk;      // Track matching selection
    bool      modfc;         // module FC(Fully Contained) event
    //int       penIron;       // # of penetrated iron
    //float     muE;           // energy of muon
    //float     nuErec;        // reconstructed energy of neutrino

    //int       nhitx;         // Number of Hit at X view (view == FromY)
    //int       nhity;         // Number of Hit at Y view (view == FromX)
    //int       ntrackhitx;    // Number of Hit at X view which are associated with reconstrack track
    //int       ntrackhity;    // Number of Hit at Y view which are associated with reconstrack track


    //int       retracktest;   // Bit for re-tracking test
    //int       trg_sec;
    //int       upstreamtpl;   //
    //int       vertexz;       // most upstream TPL #
    float       startz;      
    float       endz;       
    //int       vertexxz;      // most upstream TPL #(from X view)
    //int       vertexyz;      // most upstream TPL #(from Y view)
    int       vertexxy;      // most upstream TPL #(from Y view)
    float     angle;

    float slope;
    float intcpt;
    
    //float     thetax;
    //float     thetay;
    //vector<int>       vertexx;      // X Ch# at upstream TPL
    //vector<int>       vertexy;      // Y Ch# at upstream TPL
    //bool      horn250;      // all horns 250kA
    //float     vpe;

    //int       startxpln;
    //int       startypln;
    int         startpln;
    //int       startxch;
    //int       startych;
    float       startxy;
    //int       endxpln;
    //int       endypln;
    int         endpln;
    //int       endxch;
    //int       endych;
    float       endxy;
    //float       totpe;
    //int         isohit;
    //int         pdg[4];

    //float     vertexx_true;  // true local x-position[cm] within module
    //float     vertexy_true;  // true local y-position[cm] within module
    //float     vertexz_true;  // true local z-position[cm] within module

    //###########################################
    //###########################################
    B2ReconSummary();
    B2ReconSummary(const B2ReconSummary& basicsum);
    virtual ~B2ReconSummary();
    void Clear   (Option_t* option="");
    void Print();
    void AddB2Hit(B2HitSummary* hit);
    B2HitSummary* GetB2Hit(int i) const;
    void AddTrack(B2TrackSummary* trk);
    B2TrackSummary* GetTrack(int i) const;
    int nhits;

private:

    TRef fB2Hit[B2HIT_MAXHITS];
    TRef fTrack[B2RECON_MAXTRACKS];
public:
    int ntracks;
    int Nhits(){return nhits;}
    int Ntracks(){return ntracks;}
    ClassDef(B2ReconSummary, 1) // B2 Hit Summary
};

#endif // B2RECONSUMMARY_H
////////////////////////////////////////////////////////////////////////

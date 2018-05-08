#ifndef B2EVENTSUMMARY_H
#define B2EVENTSUMMARY_H
#include <iostream>
#include "TObject.h"
#include "TClonesArray.h"
#include "TRef.h"
#include "TTimeStamp.h"

#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
#include "B2SimHitSummary.h"
#include "B2HitSummary.h"
#include "B2SimVertexSummary.h"
#include "B2SimParticleSummary.h"
#include "BeamInfoSummary.h"
#include "B2BasicReconSummary.h"
#include "B2TrackSummary.h"
#include "NeutInfoSummary.h"
#include "B2ReconSummary.h"
#include "B2AnaSummary.h"

//......................................................................

class B2EventSummary : public TObject
{
public:
    B2EventSummary();
    B2EventSummary(const B2EventSummary& evt);
    virtual ~B2EventSummary();

    void NewTClonesArrays();
    void Clear     (Option_t* option="");

    void Print();

    B2SimHitSummary* AddB2SimHit (B2SimHitSummary* hit);
    B2SimHitSummary* GetB2SimHit (int i) const;
    B2HitSummary*    AddB2Hit    (B2HitSummary* hit);
    B2HitSummary*    GetB2Hit    (int i) const;
    B2HitSummary*    AddB2ModHit (B2HitSummary* hit, int nmod, int ncyc);
    B2HitSummary*    GetB2ModHit (int i, int nmod, int ncyc) const;

    B2SimVertexSummary*      AddSimVertex     (B2SimVertexSummary* hit);
    B2SimVertexSummary*      GetSimVertex     (int i) const;
    B2SimParticleSummary*    AddSimParticle   (B2SimParticleSummary* hit);
    B2SimParticleSummary*    GetSimParticle    (int i) const;

    BeamInfoSummary*     AddBeamSummary     (BeamInfoSummary*  beamsummary);
    BeamInfoSummary*     GetBeamSummary     (int i) const;

    B2BasicReconSummary*     AddBasicRecon     (B2BasicReconSummary* basicrecon);
    B2BasicReconSummary*     GetBasicRecon     (int i) const;
    B2TrackSummary*          AddTrack     (B2TrackSummary* trk);
    B2TrackSummary*          GetTrack     (int i) const;


    NeutInfoSummary*       AddNeut     (NeutInfoSummary* neut); 
    NeutInfoSummary*       GetNeut     (int i) const;

    B2ReconSummary*     AddB2Recon     (B2ReconSummary* b2recon);
    B2ReconSummary*     GetB2Recon     (int i) const;
    B2ReconSummary*     AddB2ModRecon  (B2ReconSummary* hit, int nmod, int ncyc, int view);
    B2ReconSummary*     GetB2ModRecon  (int i, int nmod, int ncyc, int view) const;
    B2AnaSummary*       AddB2Ana       (B2AnaSummary* b2ana);
    B2AnaSummary*       GetB2Ana       (int i) const;


public:
  
    unsigned int      run;        // run number
    unsigned int    event;        // event number
    int           runmode;        // run mode
    int             trgid;        // trgger id(1:Beam, 2:Calib. , 128:Cosmic)
    unsigned char version;        // data structure version
    int              date;
    int              time;
    int           trgtime;
    int       nd280nspill;        // spill # at nd280 
                                  // = ( spill # at beam line ) 0xffff  + 1
    bool     bunch_flag[23];

    int NB2SimHits         ()    const {return nb2simhits;}
    int NB2Hits            ()    const {return nb2hits;}  
    int NB2SimVertexes     ()    const {return nsimvertexes;}
    int NB2SimParticles    ()    const {return nsimparticles;}
    int NNeutInfo              ()    const {return nneutinfos;}    
    int NB2BeamSummarys    ()    const {return nbeamsummarys;}
    int NB2BasicRecons     ()    const {return nbasicrecons;}
    int NB2Recons              ()    const {return nb2recons;}
    int NB2Anas                ()    const {return nb2anas;}
    int NB2Tracks          ()    const {return nb2tracks;}  
    int NB2ModHits (int nmod, int ncyc ) const {return nb2modhits[nmod][ncyc];}  
    int NB2ModRecons   (int nmod, int ncyc , int view) const {return nb2modrecons[nmod][ncyc][view];}


private:
    int nb2simhits;   // number of B2SimHitSummaries  in this event
    int nb2hits;      // number of B2HitSummaries     in this event
    int nsimparticles;    // number of B2SimParticles     in this event
    int nsimvertexes;     // number of B2SimVertiexes     in this event
    int nbeamsummarys;    // number of BeamInfoSummarys       in this event  
    int nbasicrecons;     // number of B2BasicReconSummarys  in this event    
    int nb2recons;        // number of B2ReconSummarys  in this event
    int nb2anas;          // number of B2AnaSummarys  in this event
    int nb2tracks;    // number of B2TrackSummarys  in this event    
    int nneutinfos;       // number of NeutInfoSummarys       in this event

    int nb2modhits[17][23];
    vector<int> nidmodhits[17*23];

    int nb2modrecons[17][23][2];
    vector<int> nidb2modrecons[17*23*2];


    TClonesArray* fB2SimHit; 
    TClonesArray* fB2Hit;
    TClonesArray* fSimParticle;
    TClonesArray* fSimVertex;
    TClonesArray* fBeamSummary;
    TClonesArray* fBasicRecon;
    TClonesArray* fB2Recon;
    TClonesArray* fB2Ana;
    TClonesArray* fB2Track;
    TClonesArray* fNeutInfo;
  
    ClassDef(B2EventSummary, 5) // DST Summary of event
        
        };

#endif // B2EVENTSUMMARY_H
////////////////////////////////////////////////////////////////////////


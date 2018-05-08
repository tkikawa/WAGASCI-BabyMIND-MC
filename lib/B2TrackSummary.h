#ifndef B2TRACKSUMMARY_H
#define B2TRACKSUMMARY_H
#include <iostream>
#include <vector>
#include "TObject.h"

#include "TRefArray.h"
#include "TRef.h"

#include "B2HitSummary.h"
#include "B2SimParticleSummary.h"

#define TRACK_MAXHITS 1000
#define TRACK_MAXSIMPARTICLES 10


//......................................................................

class B2TrackSummary : public TObject
{
public:
    B2TrackSummary();
    B2TrackSummary(const B2TrackSummary& trk);
    virtual ~B2TrackSummary();
    
    void Clear   (Option_t* option="");
    void Print();

public:
  
    float vtxi[3];
    float vtxf[3];
    float length;
    float ekin;
    float tx;
    float ty;
    float etx;
    float ety;
    float ex0;
    float ey0;
    float covx;
    float covy;
    float chi2x;
    float chi2y;
    float btheta;
    float bphi;
    int mrdhitid[2];
    float mucl;
    float vpe;
    int view;
    
    int NHits() const {return nhits;}
    int NSimParticles() const {return nsimparticles;}
    int NSimEMShowers() const {return nsimemshowers;}

    void AddB2Hit(B2HitSummary* hit);
    B2HitSummary* GetB2Hit(int i) const;

    void AddSimParticle(B2SimParticleSummary* part);
    B2SimParticleSummary* GetSimParticle(int i) const;



private:

    int nhits;
    int nsimparticles;
    int nsimemshowers;

    TRef fB2Hit[TRACK_MAXHITS];
    TRef fSimParticle[TRACK_MAXSIMPARTICLES];



    ClassDef(B2TrackSummary, 4) // SciBar Track Summary
        };

#endif // SBSCIBARTRACKSUMMARY_H
////////////////////////////////////////////////////////////////////////


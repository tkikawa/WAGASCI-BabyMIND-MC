#ifndef B2ANASUMMARY_H
#define B2ANASUMMARY_H
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
class B2AnaSummary : public TObject{
 public:

  int       Ntrack;
  int       Ningtrack;
  float     clstime;      // time of cluster defined by most high p.e.
  float     clstimecorr;  // time after correction by measurement time of CT
  float     exptime;      // diff. from expected time
  int       hitcyc;       //
  bool      ontime;       //
  bool      vetowtracking; // Upstream VETO
  bool      edgewtracking; // Fiducial CUT
  float     vact[100];

  vector<Float_t>  x;
  vector<Float_t>  y;
  vector<Float_t>  z;
  vector<Float_t>  zx;
  vector<Float_t>  zy;

  vector<Int_t>    startxpln;
  vector<Int_t>    startypln;
  vector<Float_t>    startxch;
  vector<Float_t>    startych;
  vector<Int_t>    endxpln;
  vector<Int_t>    endypln;
  vector<Float_t>    endxch;
  vector<Float_t>    endych;
  vector<Float_t>  thetax;
  vector<Float_t>  thetay;
  vector<Float_t>  angle;
  vector<Int_t>    ing_startmod;
  vector<Int_t>    ing_endmod;
  vector<Int_t>    ing_startpln;
  vector<Int_t>    ing_endpln;
  vector<Bool_t>   ing_trk;
  vector<Bool_t>   pm_stop;
  vector<Bool_t>   ing_stop;
  vector<Float_t>  sci_range;
  vector<Float_t>  iron_range;
  vector<Int_t>    iron_pene;
  vector<Bool_t>   veto; // Upstream VETO
  vector<Bool_t>   edge; // Fiducial CUT
  vector<Int_t>    pdg;
  vector<Float_t>  mucl;
  vector<Float_t>  trkpe;

 
  
  //###########################################
  //###########################################
  B2AnaSummary();
  B2AnaSummary(const B2AnaSummary& basicsum);
  virtual ~B2AnaSummary();
  void Clear   (Option_t* option="");
  void Print();
  void AddB2Hit(B2HitSummary* hit);
  B2HitSummary* GetB2Hit(int i) const;
  void AddB2HitTrk(B2HitSummary* hit, int track);
  B2HitSummary* GetB2HitTrk(int i, int track) const;
  void AddTrack(B2TrackSummary* trk);
  B2TrackSummary* GetTrack(int i) const;
  int nhits;
  int nhitTs[B2RECON_MAXTRACKS];
  
 private:
  
  TRef fB2Hit[B2HIT_MAXHITS];
  TRef fTrack[B2RECON_MAXTRACKS];
  TRef fB2HitTrk[B2HIT_MAXHITS][B2RECON_MAXTRACKS];

 public:
  int ntracks;
  int Nhits(){return nhits;}
  int NhitTs(int trk) {return nhitTs[trk];}
  int Ntracks(){return ntracks;}
  ClassDef(B2AnaSummary, 2) // B2 Hit Summary
    };

#endif // B2ANASUMMARY_H
////////////////////////////////////////////////////////////////////////

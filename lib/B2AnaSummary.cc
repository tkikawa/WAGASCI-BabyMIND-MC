#include "B2AnaSummary.h"

//......................................................................

B2AnaSummary::B2AnaSummary():
  nhits(0), ntracks(0)
{ 
  Ntrack      = -1; 
  Ningtrack   = -1; 
  clstime     = -1.e5;  
  clstimecorr = -1.e5;
  exptime     = -1.e5;
  hitcyc      = -1;
  ontime      =  false;
  vetowtracking=  false;
  edgewtracking=  false;
  for(int i=0;i<100;i++){
  	vact[i]=-1;
  }

  x.clear();
  y.clear();
  z.clear();
  zx.clear();
  zy.clear();
  startxpln.clear();
  startypln.clear();
  startxch.clear();
  startych.clear();
  endxpln.clear();
  endypln.clear();
  endxch.clear();
  endych.clear();
  thetax.clear();
  thetay.clear();
  angle.clear();
  ing_startmod.clear();
  ing_endmod.clear();
  ing_startpln.clear();
  ing_endpln.clear();
  ing_trk.clear();
  pm_stop.clear();
  ing_stop.clear();
  sci_range.clear();
  iron_range.clear();
  iron_pene.clear();
  veto.clear();
  edge.clear();
  pdg.clear();
  mucl.clear();
  trkpe.clear();

  for(int itrk=0;itrk<B2RECON_MAXTRACKS;itrk++){
    nhitTs[itrk]=0;
  }

}

//......................................................................
B2AnaSummary::B2AnaSummary(const B2AnaSummary& basicsum) :
  nhits(0), ntracks(0)
{
  Ntrack      = basicsum. Ntrack ;
  Ningtrack   = basicsum. Ningtrack ;
  clstime     = basicsum. clstime ;
  clstimecorr = basicsum. clstimecorr;
  exptime     = basicsum. exptime;
  hitcyc      = basicsum. hitcyc;
  ontime      = basicsum. ontime;
  vetowtracking  = basicsum. vetowtracking ;
  edgewtracking  = basicsum. edgewtracking ;
  for(int i=0;i<100;i++){
  	vact[i]=basicsum.vact[i];
  }

  x = basicsum. x;
  y = basicsum. y;
  z = basicsum. z;
  zx= basicsum. zx;
  zy= basicsum. zy;
  startxpln = basicsum. startxpln;
  startypln = basicsum. startypln;
  startxch = basicsum. startxch;
  startych = basicsum. startych;
  endxpln = basicsum. endxpln;
  endypln = basicsum. endypln;
  endxch = basicsum. endxch;
  endych = basicsum. endych;
  thetax = basicsum. thetax;
  thetay = basicsum. thetay;
  angle = basicsum. angle;
  ing_startmod = basicsum. ing_startmod;
  ing_endmod = basicsum. ing_endmod;
  ing_startpln = basicsum. ing_startpln;
  ing_endpln = basicsum. ing_endpln;
  ing_trk = basicsum. ing_trk;
  pm_stop = basicsum. pm_stop;
  ing_stop = basicsum. ing_stop;
  sci_range = basicsum. sci_range;
  iron_range = basicsum. iron_range;
  iron_pene = basicsum. iron_pene;
  veto = basicsum. veto;
  edge = basicsum. edge;
  pdg = basicsum. pdg;
  mucl = basicsum. mucl;
  trkpe = basicsum. trkpe;

  for(int itrk=0;itrk<B2RECON_MAXTRACKS;itrk++){
    nhitTs[itrk]=0;
  }

  for (int i=0; i<B2HIT_MAXHITS; ++i) {
    for (int j=0; j<B2RECON_MAXTRACKS; ++j) {
      fB2HitTrk[i][j] = TRef(NULL);
    }
  }
  for (int j=0; j<basicsum.Ntrack; ++j) {
    for (int i=0; i < basicsum.nhitTs[j]; ++i) {
      AddB2HitTrk(basicsum.GetB2HitTrk(i,j),j);
    }
  }
  

  for (int i=0; i<B2HIT_MAXHITS; ++i) {
    fB2Hit[i] = TRef(NULL);
  }
  for (int i=0; i < basicsum.nhits; ++i) 
    AddB2Hit(basicsum.GetB2Hit(i));

  for (int i=0; i<B2RECON_MAXTRACKS; ++i) {
    fTrack[i] = TRef(NULL);
  }
  for (int i=0; i < basicsum.ntracks; ++i) 
    AddTrack(basicsum.GetTrack(i));

}

//......................................................................



B2AnaSummary::~B2AnaSummary() 
{

}

//......................................................................


void B2AnaSummary::Clear(Option_t* option)
{
  for (int i=0; i<B2HIT_MAXHITS; ++i)
    fB2Hit[i] = TRef(NULL);
  nhits = 0;
  
  for (int i=0; i<B2RECON_MAXTRACKS; ++i)
    fTrack[i] = TRef(NULL);
  ntracks = 0;
  
  for (int i=0; i<B2RECON_MAXTRACKS; ++i) {
    for (int j=0; j<B2HIT_MAXHITS; ++j) {
      fB2HitTrk[j][i] = TRef(NULL);
    }
    nhitTs[i]=0;  
  }

}

//......................................................................

void B2AnaSummary::Print()
{

}
//......................................................................
void B2AnaSummary::AddB2Hit(B2HitSummary* hit) 
{
  if (nhits < B2HIT_MAXHITS) {
    fB2Hit[nhits] = TRef((B2HitSummary*) hit);
    ++nhits;
  }
}
//......................................................................


B2HitSummary* B2AnaSummary::GetB2Hit(int i) const
{ 
  return (B2HitSummary*)fB2Hit[i].GetObject();
}
//......................................................................

void B2AnaSummary::AddB2HitTrk(B2HitSummary* hit, int trk)
{

  if (trk < B2RECON_MAXTRACKS) {
    if (nhitTs[trk] < B2HIT_MAXHITS) {
      fB2HitTrk[nhitTs[trk]][trk] = TRef((B2HitSummary*) hit);
      fB2Hit[nhits] = TRef((B2HitSummary*) hit);
      ++nhitTs[trk];
      ++nhits;
    }
    //++ntracks;                                                                                                                                                          
  }  

}
//....................................................................

B2HitSummary* B2AnaSummary::GetB2HitTrk(int i, int j) const
{

  return (B2HitSummary*)fB2HitTrk[i][j].GetObject();
}

//......................................................................

void B2AnaSummary::AddTrack(B2TrackSummary* trk) 
{
  if (ntracks < B2RECON_MAXTRACKS) {
    fTrack[ntracks] = TRef((B2TrackSummary*) trk);
    ++ntracks;
  }
}
//......................................................................


B2TrackSummary* B2AnaSummary::GetTrack(int i) const
{ 

  return (B2TrackSummary*)fTrack[i].GetObject();
}

//......................................................................


ClassImp(B2AnaSummary)

////////////////////////////////////////////////////////////////////////

#include "B2BasicReconSummary.h"


//......................................................................

B2BasicReconSummary::B2BasicReconSummary():
  nhits(0), ntracks(0)
{ 
  Ntrack      = -1;
  clstime     = -1.e5;
  CTtimecorr  = -1.e5;
  clstimecorr = -1.e5;
  exptime     = -1.e5;
  nhitclster  = -1;
  nactpln     = -1;
  actinarow   = -1;
  layerpe     = -1.e5;
  hitmod      = -1;
  hitcyc      = -1;
  spill_flag  =  false;
  bunch_flag  =  false;
  upstreamtpl = -1;
  upstreamVETO=  false;
  upstreamedge=  false;
  newfid      =  false;
  newfidcosmic=  false;
  vinternal   =  false;
  ontime      =  false;
  vetodist    = -1e-5;
  vetowtracking   =  false;
  edgewtracking   =  false;
  matchtrk    =  false;
  hastrk      =  false;
  trg_sec     = -1e-5;
  vpe         = -1e-5;
  angle       = -1e-5;
  thetax      = -1e-5;
  thetay      = -1e-5;
  vertexz     = -1;
  vertexxz    = -1;
  vertexyz    = -1;
  trgbit      = -1;
  inarowbit   = -1;
  horn250     =  false;

  vertexx.clear();
  vertexy.clear();

  startxpln = -1;
  startypln = -1;
  startxch  = -1;
  startych  = -1;
  endxpln   = -1;
  endypln   = -1;
  endxch    = -1;
  endych    = -1;
  modfc     = false;
  penIron   = -1;
  muE       = -1e-5;
  nuErec    = -1e-5;

  nhitx     = -1;
  nhity     = -1;
  ntrackhitx= -1;
  ntrackhity= -1;

    retracktest= false;
    //retrackx   = false;
    //retracky   = false;

  x      = -1.e5;
  y      = -1.e5;
  z      = -1.e5;
  zx     = -1.e5;
  zy     = -1.e5;

}
//......................................................................
B2BasicReconSummary::B2BasicReconSummary(const B2BasicReconSummary& basicsum) :
  nhits(0), ntracks(0)
{ 
  Ntrack      = basicsum. Ntrack ;
  clstime     = basicsum. clstime ;
  CTtimecorr  = basicsum. CTtimecorr;
  clstimecorr = basicsum. clstimecorr;
  exptime     = basicsum. exptime;
  nhitclster  = basicsum. nhitclster;
  nactpln     = basicsum. nactpln;
  actinarow   = basicsum. actinarow;
  layerpe     = basicsum. layerpe;
  upstreamVETO= basicsum. upstreamVETO;
  upstreamedge= basicsum. upstreamedge;
  vinternal   = basicsum. vinternal;
  hitmod      = basicsum. hitmod;
  hitcyc      = basicsum. hitcyc;
  spill_flag  = basicsum. spill_flag;
  bunch_flag  = basicsum. bunch_flag;
  upstreamtpl = basicsum. upstreamtpl;
  ontime      = basicsum. ontime;
  vpe         = basicsum. vpe;
  trg_sec     = basicsum. trg_sec ;
  trgbit      = basicsum. trgbit ;
  vetodist       = basicsum. vetodist ;
  vetowtracking  = basicsum. vetowtracking ;
  edgewtracking  = basicsum. edgewtracking ;
  matchtrk       = basicsum. matchtrk ;
  hastrk         = basicsum. hastrk ;
  inarowbit      = basicsum. inarowbit ;
  ntracks = 0;
  nhits = 0;
  angle       = basicsum. angle;
  thetax      = basicsum. thetax;
  thetay      = basicsum. thetay;
  vertexz     = basicsum. vertexz;
  vertexxz    = basicsum. vertexxz;
  vertexyz    = basicsum. vertexyz;
  vertexx     = basicsum. vertexx;
  vertexy     = basicsum. vertexy;
  horn250     = basicsum. horn250;


  startxpln   = basicsum. startxpln;
  startypln   = basicsum. startypln;
  startxch    = basicsum. startxch;
  startych    = basicsum. startych;
  endxpln     = basicsum. endxpln;
  endypln     = basicsum. endypln;
  endxch      = basicsum. endxch;
  endych      = basicsum. endych;
  modfc       = basicsum. modfc;
  penIron     = basicsum. penIron;
  muE         = basicsum. muE;
  nuErec      = basicsum. nuErec;

  nhitx       = basicsum. nhitx;
  nhity       = basicsum. nhity;
  ntrackhitx  = basicsum. ntrackhitx;
  ntrackhity  = basicsum. ntrackhity;
  
  retracktest = basicsum. retracktest;
    //retrackx    = basicsum. retrackx;
    //retracky    = basicsum. retracky;
  
  x           = basicsum. x;
  y           = basicsum. y;
  z           = basicsum. z;
  zx          = basicsum. zx;
  zy          = basicsum. zy;


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



B2BasicReconSummary::~B2BasicReconSummary() 
{

}

//......................................................................


void B2BasicReconSummary::Clear(Option_t* option)
{
  for (int i=0; i<B2HIT_MAXHITS; ++i)
    fB2Hit[i] = TRef(NULL);
  nhits = 0;

  for (int i=0; i<B2RECON_MAXTRACKS; ++i)
    fTrack[i] = TRef(NULL);
  ntracks = 0;

}

//......................................................................

void B2BasicReconSummary::Print()
{

}
//......................................................................
void B2BasicReconSummary::AddB2Hit(B2HitSummary* hit) 
{

    if (nhits < B2HIT_MAXHITS) {
        fB2Hit[nhits] = TRef((B2HitSummary*) hit);
        ++nhits;
    }

}
//......................................................................


B2HitSummary* B2BasicReconSummary::GetB2Hit(int i) const
{ 

  return (B2HitSummary*)fB2Hit[i].GetObject();
}

//......................................................................

void B2BasicReconSummary::AddTrack(B2TrackSummary* trk) 
{

    if (ntracks < B2RECON_MAXTRACKS) {
        fTrack[ntracks] = TRef((B2TrackSummary*) trk);
        ++ntracks;
    }

}
//......................................................................


B2TrackSummary* B2BasicReconSummary::GetTrack(int i) const
{ 

  return (B2TrackSummary*)fTrack[i].GetObject();
}


//......................................................................





ClassImp(B2BasicReconSummary)

////////////////////////////////////////////////////////////////////////
